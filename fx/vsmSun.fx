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
float velvStrength = 0.4; // vecSkill1.z | velvety Strength
float3 teamColor = {1.0f, 0.0f, 0.0f}; // vecSkill5.xyz | teamcolor
float3 lumColor = {1.2f, 1.8f, 5.2f}; // vecSkill9.xyz | color of the luminancemap
//

//misc
float shadowAlpha = 0.5; // range: 0-1 | 0 = pure black shadow | 1 = no shadow at all
float3 velvColor = {1.4f, 1.4f, 1.4f}; // velvety color
float bumpStrength = 2; // bumpmapping strength
//

/***************************************SHADER*CODE****************************************************/

// Matrices
float4x4 matWorldViewProj;
float4x4 matWorldInv;
float4x4 matViewInv;
float4x4 matWorld;
float4x4 matMtl;
float4x4 matEffect1;
float4x4 matEffect2;

// Passed by the engine
float4 vecFog;
float4 vecViewPos;
float4 vecViewDir;
float4 vecLightPos[8];
float4 vecLightColor[8];
float4 vecSunColor;
float4 vecSunDir;
float4 vecSkill1;

float light_vsm_epsilon = 0.00003;
//float light_vsm_epsilon = 0.000003;
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

// Mask sampler
sampler maskSampler = sampler_state 
{ 
   Texture = <entSkin2>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//NormalMap
sampler normalSampler = sampler_state 
{ 
   Texture = <entSkin3>;
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

// Environment map sampler
sampler envSampler = sampler_state
{
	Texture = <mtlSkin1>;
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
 
// Vertex Shader
void DiffuseVS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0,
   in float3 InTangent : TEXCOORD0,
   out float4 OutPos: POSITION, 
   out float2 OutTex: TEXCOORD0, 
   out float4 OutLight: TEXCOORD1,
   out float3 OutViewDir: TEXCOORD2,
   out float3 OutWorldNormal	: TEXCOORD3,
	out float3 OutWorldTangent	: TEXCOORD4,
	out float3 OutWorldBinorm	: TEXCOORD5,
	out float3 OutWorldPos	: TEXCOORD6,
	out float4 OutShadow	: TEXCOORD7,
   out float OutFog : FOG) 
{ 
	// Transform the vertex from object space to clip space: 
   OutPos = mul(InPos, matWorldViewProj); 
	// Pass the texture coordinate to the pixel shader: 
   OutTex.xy = InTex;
   
   float3 PosWorld = mul(InPos, matWorld);
  	
   //Light
   OutLight.xyz = vecLightPos[0] - PosWorld;
   OutLight.w = 0;
   if(vecLightPos[0].w < 100000) OutLight.w = 1-distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
  	//

	//Specular Lighting
	OutViewDir = matViewInv[3].xyz - PosWorld;
	//
	
	//Environment Mapping
	float3 Binormal = cross(InNormal,InTangent);
	OutWorldNormal.xyz = mul(InNormal, matWorld).xyz;
	OutWorldTangent.xyz = mul(InTangent, matWorldInv).xyz;
	OutWorldBinorm.xyz = mul(Binormal, matWorldInv).xyz;
	OutWorldPos = PosWorld;
	//
	
	//Shadowmapping
	float4 ShadowMapSamplingPos = mul(mul(InPos,matWorld), mul(matMtl,matEffect1));
	float4 RealDistance = mul(mul(InPos,matWorld), matMtl).z/matEffect2[0].w;
	
	OutShadow.x = ShadowMapSamplingPos.x;
	OutShadow.y = ShadowMapSamplingPos.y;
	OutShadow.z = ShadowMapSamplingPos.w;
	OutShadow.w = RealDistance.x;
	//

	//Fog
	OutFog = 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z);
   //
}

// Pixel Shader: 
float4 DiffusePS( 
   in float4 InTex: TEXCOORD0, 
   in float4 InLight : TEXCOORD1,
   in float4 InViewDir: TEXCOORD2,
   in float3 InWorldNormal	: TEXCOORD3,
	in float3 InWorldTangent	: TEXCOORD4,
	in float3 InWorldBinorm	: TEXCOORD5,
	in float3 InWorldPos	: TEXCOORD6,
	in float4 InShadow	: TEXCOORD7
	
): COLOR 
{ 
	float3 Ln = normalize(InLight.xyz);
   float3 Nn = normalize(InWorldNormal);
   float3 Tn = normalize(InWorldTangent);
   float3 Bn = normalize(InWorldBinorm);
   float3 Nb = 0;
   float3 Vn = normalize(InViewDir);
   float3 Hn = normalize(Vn + Ln);

	float3 texCol = tex2D(colorSampler,InTex).xyz;
	float4 normal = tex2D(normalSampler,InTex.xy);
	float4 mask = tex2D(maskSampler,InTex);
	//texCol += mask.a * teamColor;
	float4 color = 1;

	//---- LIGHTING ----
   float3 bumpNormal= 0;//bumpStrength * (normal.xyz-(0.5).xxx);
   Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   Nb = normalize(Nb);
   float4 lighting = lit(dot(Ln,Nb),dot(Hn,Nb),specularExp);
   //
   
   //---- ENVIRONMENT MAPPING ----
	//Vn = normalize(matViewInv[3].xyz - position);
	float3 reflVect = reflect(Vn,normalize(InWorldNormal));
	float3 reflColor = texCUBE(envSampler,float4(-reflVect, 1)).rgb * mask.g;
 	//
 	
 	//---- ENVIRONMENT ILLUMINATION MAPPING ----
	float4 envLight = texCUBE(envSampler,float4(Nb, 1));
	//envLight.xyz = lerp(envLight.xyz,reflColor.xyz,0.5);
	//
	
	//---- VELVETY ----
   float velvety = 1-dot(Vn,Nb);
   //
   
   //---- SHADOW ----
   
   float2 ProjectedTexCoords;
   ProjectedTexCoords[0] = InShadow.x/InShadow.z/2.0f +0.5f;
   ProjectedTexCoords[1] = -InShadow.y/InShadow.z/2.0f +0.5f;

	float blurStr = matEffect2[1].x;
	float shadowDepth = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
	
	/*
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr,0)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr,0)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(0,blurStr)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(0,-blurStr)).x;
	*/
	/*
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr/2,blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr/2,-blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr/2,blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr/2,-blurStr/2)).x;
	*/
	
	//shadowDepth /= 5;
	
	
	
	float shadow1 = doVSM(float2(shadowDepth,shadowDepth*shadowDepth),InShadow.w);
   shadow1 = (1-shadow1);
   
   shadow1 *=(1-tex2D(ShadowAlphaSampler, ProjectedTexCoords));
   shadow1 += tex2D(ShadowAlphaSampler, ProjectedTexCoords);
   //shadow1 *= lighting.y;
   
   //envLight.xyz += reflColor.xyz;
   //envLight.xyz *= shadow1;
   envLight.xyz = 0;
   color.rgb = ((envLight.xyz+((clamp(shadow1,0,1) + ((1-clamp(shadow1,0,1))*0.2))*vecLightColor[0].xyz)) * texCol) + lighting.z*mask.r*shadow1;
   color.a = tex2D(colorSampler,InTex).a;
   //color.rgb = envLight.xyz;
   //color.rgb = ((envLight.xyz+((clamp(shadow1*lighting.y,0,1) + ((1-clamp(shadow1*lighting.y,0,1))*0.05))*vecLightColor[0].xyz)) * texCol) + lighting.z*mask.r;
   //color.rgb *= clamp(shadow1,0,1) + ((1-clamp(shadow1,0,1))*0.05);
   //color.rgb = ((envLight.xyz) * texCol)+reflColor*mask.r;
   //color.rgb += normal.a * lumColor;
   //shadowAlpha
   //color.rgb = clamp(shadow1,0,1);
   color.rgb = ((clamp(shadow1,0,1) + ((1-clamp(shadow1,0,1))*shadowAlpha)));
   //color.a = 1-color.rgb;
   //color.rgb += reflColor.xyz;
   
   //color.rgb = texCol;//((envLight.xyz+((clamp(shadow1,0,1) + ((1-clamp(shadow1,0,1))*0.0))*vecLightColor[0].xyz)) * texCol);
   
   //color.rgb = clamp(color.rgb,0,1);
   //color.rgb += envLight.xyz;
   //color.rgb = texCol;
   
   
   //fogman
   	shadow1 *= lighting.y;
   	//color.rgb = ((clamp(shadow1,0,1) + ((1-clamp(shadow1,0,1))*shadowAlpha)))*vecSunColor.xyz;
   //fogman end
   return color;
} 




technique t01
{ 

   pass P0 
   {
		VertexShader = compile vs_2_0 DiffuseVS();
     	PixelShader  = compile ps_2_a DiffusePS();
     	
     	//shademode = gouraud;
     	//texture[0] = 0;
   }

}