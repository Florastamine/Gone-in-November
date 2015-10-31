/******************************************************************************************************
"All-in-One" Shader by Wolfgang "BoH_Havoc" Reichardt

Entity Textures:
	Skin1 = Colormap 
	Skin2 = Effectmask
	Skin3 = Normalmap (RGB) + Luminancemask (A)
	Skin4 = Shadowmap (uses 2nd UV-Map)
	
	Effectmask.r = Specular Intensity
	Effectmask.g = Environmentmapping Intensity
	Effectmask.b = Velvety Intensity
	Effectmask.a = TeamColor Alpha
	
Material Textures:
	Skin1 = Environment Map
	
Usage:
	Uncomment/Comment the #defines to add/remove an effect.
	vecSkill1.x sets the diffuse shadow alpha (mtl.skill1)
	vecSkill1.y sets the specular power (mtl.skill2)
	vecSkill1.y sets the velvety power (mtl.skill3)
	vecSkill5.xyz sets the teamcolor (mtl.skill5, mtl.skill6, mtl.skill7)
	vecSkill9.xyz sets the luminance color (mtl.skill9, mtl.skill10, mtl.skill11)

******************************************************************************************************/

/***************************************TWEAKABLES*****************************************************/

// Default Values (will be used if vecSkill1, vecSkill5 or vecSkill9 hasn't been set): 
float specularExp = 15; // vecSkill1.y | sharpness of the specular lighting
//

//misc
float shadowAlpha = 0.1; // range: 0-1 | 0 = pure black shadow | 1 = no shadow at all
//

/***************************************SHADER*CODE****************************************************/
//#include <lights>
// Matrices

float4x4 matWorldViewProj;
float4x4 matWorldInv;
float4x4 matViewInv;
float4x4 matWorld;

float4x4 matEffect1;
float4x4 matEffect2;
float4x4 matEffect3;

// Passed by the engine

float4 vecFog;
float4 vecViewPos;
float4 vecViewDir;
float4 vecLightPos[8];
float4 vecLightColor[8];
float4 vecSunColor;
float4 vecSunDir;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float4 vecLight;
float fPower;
float fAmbient;
float fAlbedo;

float4 vecSkill1;

float light_vsm_epsilon = 0.00003;
float light_shadow_bias = 0.0025;

//Textures
texture entSkin1; // Colormap
texture entSkin2; // Mask texture
texture entSkin3; // Normalmap
texture mtlSkin1; // Environment Map

// Color map sampler
sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

// ShadowDepth map sampler
texture sc_map_shadowDepthSun_bmap;

sampler ShadowMapSampler = sampler_state
{
	Texture = <sc_map_shadowDepthSun_bmap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

Texture sc_map_shadowAlpha_bmap;

sampler ShadowAlphaSampler = sampler_state
{
	texture = <sc_map_shadowAlpha_bmap>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter=LINEAR;
	AddressU = clamp;
	AddressV = clamp;
};


///////////////////////HELPER FUNCTIONS///////////////////////////////////////
// calculate the light attenuation factor
float DoLightFactor(float4 Light,float3 Pos)
{
   float fac = 0.f;
   if (Light.w > 0.f) {    
      float LD = length(Light.xyz-Pos)/Light.w;
      if (LD < 1.f)
         fac = saturate(1.f - LD);

   }
   return fac; // get the distance factor
}

// calculate the light attenuation factor on the front side
float DoLightFactor(float4 Light,float3 P,float3 N)
{
	float3 D = Light.xyz-P; // ray pointing from the light to the surface
	float NdotL = dot(N,normalize(D));   // angle between surface and light ray
	
	if (NdotL >= 0.f) 
	   return saturate(NdotL*8)*DoLightFactor(Light,P);
	else
	   return 0.f;
}

float DoLightFactorN(float4 Light,float3 P,float3 N)
{
	float3 D = Light.xyz-P; // ray pointing from the light to the surface
	float NdotL = dot(N,normalize(D));   // angle between surface and light ray
	
	if (NdotL >= 0.f) 
	   return 2 * NdotL * DoLightFactor(Light,P);
	else
	   return 0.f;
}

float4 DoPointLight(float3 P, float3 N, float4 Light, float4 LightColor)
{
	return LightColor * DoLightFactorN(Light,P,N);
}

float4 DoLight(float3 P, float3 N, int i)
{
	
	if(vecLightPos[i].w < 100000) return DoPointLight(P,N,vecLightPos[i],vecLightColor[i]);
	else return 0;
}

float doVSM(float2 depthmap, float lightDepth)
{
     	//depthmap.y = (depthmap.x * depthmap.x);
     	float lit_factor = (lightDepth <= depthmap.x);
    	
    	// Variance shadow mapping
    	float E_x2 = depthmap.y;
    	float Ex_2 = depthmap.x * depthmap.x;
    	float variance = min(max(E_x2 - Ex_2, 0.0) + light_vsm_epsilon, 1.0);
    	float m_d = (depthmap.x - lightDepth);
    	float p = variance / (variance + m_d * m_d);
    	
    	// Adjust the light color based on the shadow attenuation
    	return 1-max(lit_factor, p);
}
///////////////////////END OF HELPER FUNCTIONS///////////////////////////////////////


struct outVS
{
	float4 Color: COLOR0; 
   float4 Pos: POSITION; 
   float2 Tex: TEXCOORD0; 
   float4 Light: TEXCOORD1;
   float3 ViewDir: TEXCOORD2;
   float3 WorldNormal	: TEXCOORD3;
	float4 Shadow	: TEXCOORD4;
   float Fog : FOG;
};

// Vertex Shader
outVS DiffuseVS( 
   float4 InPos: POSITION, 
   float3 InNormal: NORMAL, 
   float2 InTex: TEXCOORD0,
   float3 InTangent : TEXCOORD0
) 
{ 
	outVS Out;
	// Transform the vertex from object space to clip space: 
   Out.Pos = mul(InPos, matWorldViewProj); 
	// Pass the texture coordinate to the pixel shader: 
   Out.Tex.xy = InTex;
   
   float3 PosWorld = mul(InPos, matWorld);
  	
   //Light
   Out.Light.xyz = vecLightPos[0] - PosWorld;
   Out.Light.w = 0;
   if(vecLightPos[0].w < 100000) Out.Light.w = 1-distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
  	//

	//Specular Lighting
	Out.ViewDir = matViewInv[3].xyz - PosWorld;
	//
	
	//Environment Mapping
	Out.WorldNormal.xyz = mul(InNormal, matWorld).xyz;
	//
	
	//Shadowmapping
	float4 ShadowMapSamplingPos = mul(mul(InPos,matWorld), mul(matEffect3,matEffect1));
	float4 RealDistance = mul(mul(InPos,matWorld), matEffect3).z/matEffect2[0].w;
	
	Out.Shadow.x = ShadowMapSamplingPos.x;
	Out.Shadow.y = ShadowMapSamplingPos.y;
	Out.Shadow.z = ShadowMapSamplingPos.w;
	Out.Shadow.w = RealDistance.x;
	//
	
	Out.Color = fAmbient; // Add ambient and sun light
	for (int i=0; i<8; i++)  // Add 8 dynamic lights
		Out.Color += DoLight(PosWorld,Out.WorldNormal.xyz,i);
/*
	Out.Color =
	(vecAmbient * vecLight) + 
	(vecDiffuse * (vecSunColor * dot(Out.WorldNormal.xyz,-vecSunDir))); 
	(vecSpecular * pow((vecSunColor * dot(Out.WorldNormal.xyz,normalize(Out.ViewDir + Out.Light.xyz))),fPower))+ 
	(vecEmissive);
	Out.Color = clamp(Out.Color,0,1);
*/	
	
	
	//Fog
	Out.Fog = 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z);
   //
   
   return Out;
}



float4 DiffusePS(outVS In): COLOR
{
	/*
	float diff = dot(In.WorldNormal.xyz,-vecSunDir);
	float4 lighting = 
	(vecAmbient * vecLight) + 
	clamp(vecDiffuse * (vecSunColor * diff),0,1) +
	(vecSpecular * pow((vecSunColor * dot(In.WorldNormal.xyz,normalize(In.ViewDir + In.Light.xyz))),fPower))+ 
	(vecEmissive);
	*/
	//lighting
	
	float4 color = tex2D(colorSampler,In.Tex);
	float2 ProjectedTexCoords;
   ProjectedTexCoords[0] = In.Shadow.x/In.Shadow.z/2.0f +0.5f;
   ProjectedTexCoords[1] = -In.Shadow.y/In.Shadow.z/2.0f +0.5f;

	float blurStr = matEffect2[1].x;
	float shadowDepth = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
	
	float shadow1 = doVSM(float2(shadowDepth,shadowDepth*shadowDepth),In.Shadow.w);
   shadow1 = (1-shadow1);
   
   shadow1 *=(1-tex2D(ShadowAlphaSampler, ProjectedTexCoords));
   shadow1 += tex2D(ShadowAlphaSampler, ProjectedTexCoords);
   //shadow1 *= lighting.y;
   shadow1 = clamp(shadow1,0,1);
   
   float diff = dot(In.WorldNormal.xyz,-vecSunDir)*shadow1;
   //float4 diff = lit(dot(normalize(In.Light.xyz),normalize(In.WorldNormal.xyz)),dot(normalize(In.ViewDir + In.Light.xyz),normalize(In.WorldNormal.xyz)),fPower)*shadow1;
	float4 lighting = 
	(vecAmbient * vecLight)*2 + 
	clamp(vecDiffuse * (vecSunColor * diff),0,1)*color*2 +
	(vecSpecular * pow((vecSunColor * dot(In.WorldNormal.xyz,normalize(In.ViewDir + In.Light.xyz))),fPower))+ 
	//(vecSpecular * (vecSunColor * diff.z))+ 
	(vecEmissive);

   /*
	shadow1 *= clamp(lighting.rgb,0,1);
   shadow1 = clamp(shadow1,0,1)+(1-lighting.rgb);

	//color.rgb = ((envLight.xyz+((clamp(shadow1,0,1) + ((1-clamp(shadow1,0,1))*0.2))*vecLightColor[0].xyz)) * texCol) + lighting.z*mask.r*shadow1;
   color.a = tex2D(colorSampler,In.Tex).a;
	color.rgb = shadow1*lighting.rgb*2*tex2D(colorSampler,In.Tex).xyz;
	*/
	//color.a = tex2D(colorSampler,In.Tex).a;
	color.rgb = (lighting+In.Color)*tex2D(colorSampler,In.Tex).xyz;
	return color;
	//float4 Color = tex2D(colorSampler,In.Tex);
	//return Color*In.Color*2;
}


technique t01
{ 

   pass P0 
   {
		VertexShader = compile vs_2_0 DiffuseVS();
     	PixelShader  = compile ps_2_a DiffusePS();
     	zWriteEnable = true;
		alphaTestEnable = true;
   }

}