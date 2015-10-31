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
float specularExp = 50; // vecSkill1.y | sharpness of the specular lighting
float velvStrength = 0.4; // vecSkill1.z | velvety Strength
float3 teamColor = {1.0f, 0.50f, 0.0f}; // vecSkill5.xyz | teamcolor
float3 lumColor = {1.2f, 1.8f, 5.2f}; // vecSkill9.xyz | color of the luminancemap
//

//misc
float shadowAlpha = 0.0; // range: 0-1 | 0 = pure black shadow | 1 = no shadow at all
float3 velvColor = {1.4f, 1.4f, 1.4f}; // velvety color
float bumpStrength = 4; // bumpmapping strength
//

//Luminance Map Animation
float texRotSpeed = 0; // Rotation Speed
float texXShiftSpeed = 0; // Shift Speed X
float texYShiftSpeed = 0; // Shift Speed Y
float RepeatS = 1.0; // tiling
float RepeatT = 1.0; // tiling
float Angle = 0.0; // pan at start
float OffsetS = 0.0; // offset at startup
float OffsetT = 0.0; // offset at startup
float RotCenterS = 0.5; // rotation center
float RotCenterT = 0.5; // rotation center
//

//dynamic shadowmapping
float light_vsm_epsilon = 0.00003;
float light_shadow_bias = 0.0025;
//

// UNCOMMENT/COMMENT TO ACTIVATE/DEACTIVATE

//Use Diffuse Lighting
#define DIFFLIGHT

//Use dynamic Soft Shadows
#define DYNSHADOW

//Use Shadowmap
//#define SHADOWMAPPING

//Use Specular Lighting
#define SPECLIGHT

//Use Environment Mapping
//#define ENVMAPPING

//Use Environment Illumination Mapping
//#define ENVILLUM

//Use Velvety Effect
//#define VELVETY

//Use Normalmapping
#define NORMALMAPPING

//Use Luminance Mapping
//#define LUMMAPPING

//Use Team Color
//#define TCOLOR

//Above Effects will react to sunlight (only needed if you have dynamic lights in your level)
#define DOSUN

//Use PixelShader 2.a | set this if you get an errormessage ("code uses too many many arithmetic instruction slots")
#define PS2A





/***************************************SHADER*CODE****************************************************/

// Matrices
float4x4 matWorldViewProj;
float4x4 matWorldInv;
float4x4 matViewInv;
float4x4 matWorld;

float4x4 matEffect1;
float4x4 matEffect2;
float4x4 matEffect3;

// Passed by the engine
float4 vecTime;
float4 vecFog;
float4 vecViewPos;
float4 vecViewDir;
float4 vecSunPos;
float4 vecSunColor;
float4 vecLightPos[8];
float4 vecLightColor[8];
float4 vecSkill1;
float4 vecSkill5;
float4 vecSkill9;

float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float4 vecLight;
float fPower;
float fAmbient;
float fAlbedo;

//Textures
texture entSkin1; // Colormap
texture entSkin2; // Mask texture
texture entSkin3; // Normalmap
texture entSkin4; // Shadowmap
texture mtlSkin1; // Environment Map

// Color map sampler
sampler ColorMapSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

// Mask sampler
sampler MaskSampler = sampler_state 
{ 
   Texture = <entSkin2>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//NormalMap
sampler NormalMapSampler = sampler_state 
{ 
   Texture = <entSkin3>;
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//ShadowMap
sampler ShadowMapSampler = sampler_state 
{ 
   Texture = <entSkin4>;
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

// Environment map sampler
sampler EnvMapSampler = sampler_state
{
	Texture = <mtlSkin1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

// ShadowDepth map sampler
texture sc_map_shadowDepthSun_bmap;

sampler dynShadowMapSampler = sampler_state
{
	Texture = <sc_map_shadowDepthSun_bmap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//ShadowAlpha sampler
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
 
// Vertex Shader
void DiffuseVS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0,
   in float3 InTangent : TEXCOORD0,
   in float2 InShadow : TEXCOORD1,
   out float4 OutPos: POSITION,
   out float4 OutColor : COLOR0,
   out float4 OutTex: TEXCOORD0, 
   out float4 OutLight: TEXCOORD1,
   out float3 OutViewDir: TEXCOORD2,
   out float3 OutWorldNormal	: TEXCOORD3,
	out float3 OutWorldTangent	: TEXCOORD4,
	out float4 OutWorldBinorm	: TEXCOORD5,
	out float4 OutWorldPos	: TEXCOORD6,
	out float4 OutLumUV	: TEXCOORD7,
   out float OutFog : FOG) 
{ 
	// Transform the vertex from object space to clip space: 
   OutPos = mul(InPos, matWorldViewProj); 
	// Pass the texture coordinate to the pixel shader: 
   OutTex.xy = InTex;
   // Pass the shadow coordinate to the pixel shader: 
   OutTex.zw = InShadow;
   
   float3 PosWorld = mul(InPos, matWorld);
  	
   //Light
   #ifdef DOSUN
   OutLight.xyz = vecSunPos - PosWorld;;
   OutLight.w = 100000;//distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
   #else
   OutLight.xyz = vecLightPos[0] - PosWorld;
   OutLight.w = 0;
   if(vecLightPos[0].w < 100000) OutLight.w = 1-distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
   #endif
  	//

	//Specular Lighting
	
	/*
	#ifdef DOSUN
	OutViewDir = (vecSunPos) - mul(InPos, matWorld);
	#else
	OutViewDir = (vecLightPos[0]) - mul(InPos, matWorld); 
	#endif
	*/
	
	OutViewDir = matViewInv[3].xyz - PosWorld;
	//
	
	//Environment Mapping
	float3 Binormal = cross(InNormal,InTangent);
	OutWorldNormal.xyz = mul(InNormal, matWorld).xyz;
	OutWorldTangent.xyz = mul(InTangent, matWorldInv).xyz;
	OutWorldBinorm.xyz = mul(Binormal, matWorldInv).xyz;
	OutWorldPos.xyz = PosWorld;
	//
	
	//Luminance Map Animation
	float a = radians(Angle + (vecTime.w/2)*texRotSpeed);
   float ca = cos(a);
   float sa = sin(a);
   float2 off = float2(RotCenterS,RotCenterT);
   float2 nuv = InTex.xy - off;
   float2 ruv = float2(nuv.x*ca-nuv.y*sa,nuv.x*sa+nuv.y*ca);
   nuv = ruv + off;
   OutLumUV.xy = float2(max(0.001,RepeatS) * nuv.x + OffsetS + ((vecTime.w/200)*texXShiftSpeed),
		    				max(0.001,RepeatT) * nuv.y + OffsetT + ((vecTime.w/200)*texYShiftSpeed));
	//
	
	
	//Dynamic Shadowmapping
	float4 ShadowMapSamplingPos = mul(mul(InPos,matWorld), mul(matEffect3,matEffect1));
	float4 RealDistance = mul(mul(InPos,matWorld), matEffect3).z/matEffect2[0].w;
	
	OutWorldBinorm.w = ShadowMapSamplingPos.x;
	OutWorldPos.w = ShadowMapSamplingPos.y;
	OutLumUV.z = ShadowMapSamplingPos.w;
	OutLumUV.w = RealDistance.x;
	//
	
	//Fog
	OutFog = 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z);
   //
   
   //dynamic Lights
   OutColor = fAmbient; // Add ambient and sun light
   #ifdef DOSUN
	for (int i=0; i<8; i++)  // Add 8 dynamic lights
		OutColor += DoLight(PosWorld,OutWorldNormal.xyz,i);
	#else
	for (int i=1; i<8; i++)  // Add 7 dynamic lights
		OutColor += DoLight(PosWorld,OutWorldNormal.xyz,i);
	#endif
	//
}

// Pixel Shader: 
float4 DiffusePS( 
	in float4 InColor : COLOR0,
   in float4 InTex: TEXCOORD0, 
   in float4 InLight : TEXCOORD1,
   in float4 InViewDir: TEXCOORD2,
   in float3 InWorldNormal	: TEXCOORD3,
	in float3 InWorldTangent	: TEXCOORD4,
	in float4 InWorldBinorm	: TEXCOORD5,
	in float4 InWorldPos	: TEXCOORD6,
	in float4 InLumUV	: TEXCOORD7
): COLOR 
{ 
	float3 Ln = normalize(InLight.xyz);
   float3 Nn = normalize(InWorldNormal.xyz);
   float3 Tn = normalize(InWorldTangent.xyz);
   float3 Bn = normalize(InWorldBinorm.xyz);
   float3 Nb = 0;
   float3 Vn = 0;
   float3 Hn = 0;
   float4 lighting = 0;
   float4 envLight = 0;
   float shadow1 = 1;

	//check if vecSkill1 has been set. If not, use default values
	#ifdef DIFFLIGHT
		if(vecSkill1.x == 0) vecSkill1.x = shadowAlpha;
	#endif
	#ifdef SPECLIGHT
		if(vecSkill1.y == 0) vecSkill1.y = specularExp;
	#endif
	#ifdef VELVETY
		if(vecSkill1.z == 0) vecSkill1.z = velvStrength;
	#endif

	#ifdef TCOLOR
 		//check if vecSkill9 has been set. If not, use default values
 		if(vecSkill5.x + vecSkill5.y + vecSkill5.z == 0) vecSkill5.xyz = teamColor;
 	#endif
		
	#ifdef LUMMAPPING
 		//check if vecSkill5 has been set. If not, use default values
 		if(vecSkill9.x + vecSkill9.y + vecSkill9.z == 0) vecSkill9.xyz = lumColor;
 	#endif
 	
 	#ifdef DYNSHADOW
 		float4 InShadow = float4(InWorldBinorm.w, InWorldPos.w, InLumUV.zw);

 		float2 ProjectedTexCoords;
	   ProjectedTexCoords[0] = InShadow.x/InShadow.z/2.0f +0.5f;
   	ProjectedTexCoords[1] = -InShadow.y/InShadow.z/2.0f +0.5f;

		float blurStr = matEffect2[1].x;
		float shadowDepth = tex2D(dynShadowMapSampler, ProjectedTexCoords).x;
	
		shadow1 = doVSM(float2(shadowDepth,shadowDepth*shadowDepth),InShadow.w);
   	shadow1 = (1-shadow1);
   
   	shadow1 *=(1-tex2D(ShadowAlphaSampler, ProjectedTexCoords));
   	shadow1 += tex2D(ShadowAlphaSampler, ProjectedTexCoords);
   	shadow1 = clamp(shadow1,0,1);
 	#endif
 
 	//get color
 	float4 Color = tex2D(ColorMapSampler,InTex.xy); 
	//get effectmask
	float4 Mask = tex2D(MaskSampler,InTex.xy);
   //get luminancemap
   float luminance = tex2D(NormalMapSampler,InTex.xy).a;
   
   float3 Light1 = Color.xyz;
   
   
   
   #ifdef TCOLOR
   	Light1 += Mask.a * vecSkill5.xyz;
   	Light1 = clamp(Light1,0,1);
   #endif
   
   float3 bumpNormal = 0;
   #ifdef NORMALMAPPING
   	bumpNormal= bumpStrength * (tex2D(NormalMapSampler,InTex.xy).xyz-(0.5).xxx);
   #endif
   
   #ifdef DIFFLIGHT
   	Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   	Nb = normalize(Nb);
   	Vn = normalize(InViewDir);
   	Hn = normalize(Vn + Ln);
   	lighting = lit(dot(Ln,Nb),dot(Hn,Nb),fPower);
   #else
   	#ifdef SPECLIGHT
   		Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   		Nb = normalize(Nb);
   		Vn = normalize(InViewDir);
   		Hn = normalize(Vn + Ln);
   		lighting = lit(dot(Ln,Nb),dot(Hn,Nb),fPower);
   	#endif
   #endif
   
   lighting.y *= shadow1;
   
   #ifdef ENVMAPPING
		Nb = Nn + ((InWorldNormal) * Tn + (InWorldNormal) * Bn);
		Nb = normalize(Nb);
		Vn = normalize(matViewInv[3].xyz - InWorldPos.xyz);
	
		float3 reflVect = reflect(Vn,Nb);
		float3 reflColor = texCUBE(EnvMapSampler,float4(-reflVect, 1)).rgb;
   
   	float3 Light1BCK = Light1;
   	Light1 -= Mask.g;
   	Light1 = clamp(Light1,0,1);
   	Light1 += reflColor*Mask.g;
   	//Light1 = lerp(Light1,reflColor,Mask.g);
   #else
   	#ifdef VELVETY
   		Vn = normalize(matViewInv[3].xyz - InWorldPos.xyz);
   	#endif
   #endif
   
   /*
   #ifdef DIFFLIGHT
   #ifdef DOSUN
   	Light1.xyz *= clamp((lighting.y*vecSunColor*2),shadowAlpha,1);
   #else
   	Light1.xyz *= clamp((lighting.y*vecLightColor[0]*2*InLight.a),shadowAlpha,1);
   #endif
   #endif
   */
   
   #ifdef ENVILLUM
   	envLight = texCUBE(EnvMapSampler,float4(Nb, 1));
   #endif
   
   #ifdef SHADOWMAPPING
   	lighting.y *= tex2D(ShadowMapSampler,InTex.zw);
   #endif
   
   #ifdef DIFFLIGHT
   #ifdef DOSUN
   	Light1 *=
   	(vecAmbient.xyz * vecLight.xyz)*2 + 
		clamp(vecDiffuse.xyz * (vecSunColor.xyz * lighting.y),0,1)*2;// +
		//(vecSpecular.xyz * pow((vecSunColor * dot(In.WorldNormal.xyz,normalize(In.ViewDir + In.Light.xyz))),fPower))+ 
		//(vecSpecular * (vecSunColor * diff.z))+ 
		//(vecEmissive.xyz);
	#else
		Light1 *=
   	(vecAmbient.xyz * vecLight.xyz)*2 + 
		clamp(vecDiffuse.xyz * (vecLightColor[0].xyz * lighting.y),0,1)*2;// +
		//(vecSpecular.xyz * pow((vecSunColor * dot(In.WorldNormal.xyz,normalize(In.ViewDir + In.Light.xyz))),fPower))+ 
		//(vecSpecular * (vecSunColor * diff.z))+ 
		//(vecEmissive.xyz);
	#endif
	Light1 += (InColor.rgb+(envLight.rgb*vecLight.xyz)) * Color.rgb;
	#endif
   
   Light1 += vecEmissive.xyz;
   
   #ifdef SPECLIGHT
   	#ifdef DOSUN
     		Light1.xyz += lighting.z*Mask.r*vecSpecular.xyz*lighting.y;
     	#else
     		Light1.xyz += lighting.z*Mask.r*InLight.a*vecSpecular.xyz*lighting.y;
     	#endif
   #endif
   
   #ifdef VELVETY
   	half3 velvety1;
   	float vdn = vecSkill1.z-dot(Vn,Nn);
   	float3 vecColor = float4(vdn.xxx,1.0);
   	velvety1 = float4((vecColor*velvColor).xyz,1);
   	velvety1 = clamp(velvety1,0,1);
   	Light1 += velvety1*Mask.b;
   #endif
     
   #ifdef LUMMAPPING
    Light1 += luminance * (tex2D(NormalMapSampler,InLumUV.xy).a * vecSkill9.xyz);
   #endif  
    
      //Light1 = reflColor.rgb; 
   
   return float4(Light1,Color.a);
} 
 
technique t01
{ 

   pass P0 
   {
		VertexShader = compile vs_2_0 DiffuseVS();
   	#ifdef PS2A
      	PixelShader  = compile ps_2_a DiffusePS();
      #else
      	PixelShader  = compile ps_2_0 DiffusePS();
      #endif

      
      		
   }   
}