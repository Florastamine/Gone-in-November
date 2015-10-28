/******************************************************************************************************
Terrain-Shader by Wolfgang "BoH_Havoc" Reichardt

Entity Textures:
	Skin1 = Global Colormap
	Skin2 = Tile-Colormask  (RGB) + Shadow (A)
	Skin3 = Global Normalmap
	Skin4 = Tile-Normalmap (RGB) + Specularmap (A) (Quad-Texture)

Material Textures:
	Skin1 = Tile-Color (Quad-Texture)

Usage:

	
******************************************************************************************************/

/***************************************TWEAKABLES*****************************************************/

float shadowAlpha = 0.25; //shadowalpha of bumpmapping
float texShadowAlpha = 0.25; //shadowalpha of shadowtexture



/***************************************SHADER*CODE****************************************************/

texture entSkin1; //colormap
texture entSkin2; //global normalmap + Shadow
texture entSkin3; //colormask
texture entSkin4; //normalmaps
texture mtlSkin1; //colormaps

matrix matWorld;
matrix matWorldInv;
matrix matViewInv;
matrix matWorldViewProj;
float4x4 matMtl;
float4x4 matEffect1;
float4x4 matEffect2;

float4 vecSunPos;
float4 vecSunColor;
float4 vecLightPos[8];
float4 vecViewPos;
float4 vecFog;

#define DOSUN

sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler maskSampler = sampler_state 
{ 
   Texture = <entSkin2>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler globalNormSampler = sampler_state 
{ 
   Texture = <entSkin3>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler normSampler = sampler_state 
{ 
   Texture = <entSkin4>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler tileColorSampler = sampler_state 
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

float light_vsm_epsilon = 0.00003;
float light_shadow_bias = 0.0025;

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

void terrain_VS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0,
   in float3 InTangent : TEXCOORD0,
   in float2 InShadow : TEXCOORD1,
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
	#ifdef DOSUN
	OutViewDir = (vecSunPos) - mul(InPos, matWorld);
	#else
	OutViewDir = (vecLightPos[0]) - mul(InPos, matWorld); 
	#endif
	//
	
	//Misc Output
	float3 Binormal = cross(InNormal,InTangent);
	OutWorldNormal.xyz = mul(InNormal, matWorld).xyz;
	OutWorldTangent.xyz = mul(InTangent, matWorld).xyz;
	OutWorldBinorm.xyz = mul(Binormal, matWorldInv).xyz;
	OutWorldPos = PosWorld;
	//
	
	//Fog
	OutFog = 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z);
   //
   
   
   //Shadowmapping
	float4 ShadowMapSamplingPos = mul(mul(InPos,matWorld), mul(matMtl,matEffect1));
	float4 RealDistance = mul(mul(InPos,matWorld), matMtl).z/matEffect2[0].w;
	
	OutShadow.x = ShadowMapSamplingPos.x;
	OutShadow.y = ShadowMapSamplingPos.y;
	OutShadow.z = ShadowMapSamplingPos.w;
	OutShadow.w = RealDistance.x;
	//
}

float4 terrain_PS(
	float2 Tex : TEXCOORD0,
	float4 InLight: TEXCOORD1,
	float3 InViewDir: TEXCOORD2,
	float3 InWorldNormal	: TEXCOORD3,
	float3 InWorldTangent	: TEXCOORD4,
	float3 InWorldBinorm	: TEXCOORD5,
	float3 InWorldPos	: TEXCOORD6,
	in float4 InShadow	: TEXCOORD7
):COLOR0
{
	float4 Color = tex2D(colorSampler, Tex);
	
	float4 colorMask = tex2D(maskSampler, Tex);
	
	float2 Tile = Tex*40;
	Tile = frac(Tile)*(0.5-2*0.004)+0.004;
	
	float4 color = tex2D(colorSampler, Tex);
	
	float4 color1 = tex2D(tileColorSampler, Tile)*colorMask.r;
	float4 color2 = tex2D(tileColorSampler, Tile+float2(0.5, 0.0))*colorMask.g;
	float4 color3 = tex2D(tileColorSampler, Tile+float2(0.0, 0.5))*colorMask.b;
		
	float4 normal1 = tex2D(normSampler, Tile)*colorMask.r;
	float4 normal2 = tex2D(normSampler, Tile+float2(0.5, 0.0))*colorMask.g;
	float4 normal3 = tex2D(normSampler, Tile+float2(0.0, 0.5))*colorMask.b;
		
	float3 Ln = normalize(InLight.xyz);
   float3 Nn = normalize(InWorldNormal);
   float3 Tn = normalize(InWorldTangent);
   float3 Bn = normalize(InWorldBinorm);
   float3 Nb = 0;
   float3 Vn = 0;
   float3 Hn = 0;
   float4 lighting = 0;
	
	float gloss = normal1.a + normal2.a + normal3.a;	
	
	float3 bumpNormal = (normal1.rgb+normal2.rgb+normal3.rgb);
	//bumpNormal += tex2D(globalNormSampler, Tex).xyz;
	bumpNormal = lerp(bumpNormal, tex2D(globalNormSampler, Tex).xyz, 0.5);
	Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   Nb = normalize(Nb);
   Vn = normalize(InViewDir);
   Hn = normalize(Vn + Ln);
   lighting = lit(dot(Ln,Nb),dot(Hn,Nb),1);
   
   
   
   
   
   float2 ProjectedTexCoords;
   ProjectedTexCoords[0] = InShadow.x/InShadow.z/2.0f +0.5f;
   ProjectedTexCoords[1] = -InShadow.y/InShadow.z/2.0f +0.5f;

	float blurStr = matEffect2[1].x;
	float shadowDepth = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
	
	float shadow1 = doVSM(float2(shadowDepth,shadowDepth*shadowDepth),InShadow.w);
   shadow1 = (1-shadow1);
   
   shadow1 *=(1-tex2D(ShadowAlphaSampler, ProjectedTexCoords));
   shadow1 += tex2D(ShadowAlphaSampler, ProjectedTexCoords);
   lighting.y *= shadow1;
   
   
   
   
   
   
   
	Color.rgb = saturate(clamp(lighting.y, shadowAlpha, 1)*vecSunColor*2);
	//Color.rgb = lighting.y+lighting.z;
	
	Color.rgb *= lerp(color, (color1+color2+color3), 0.5);
	//Color.rgb *= color;
	//Color.rgb *= color*(color1+color2+color3);
	//Color.rgb *= 2;
	//Color.rgb = clamp(Color.rgb,0,1);
	Color.rgb += lighting.z*gloss*Color.rgb;
	Color.rgb = clamp(Color.rgb, 0, 1);
	Color.rgb *= clamp(colorMask.a,texShadowAlpha,1);

	Color.a = 1;
	//Color.rgb = (color1+color2+color3);
	/*
	//VELVETY
	Vn = normalize(matViewInv[3].xyz - InWorldPos);
	half3 velvety1;
   float vdn = 0.4-dot(Vn,Nn);
   float3 vecColor = float4(vdn.xxx,1.0);
   velvety1 = float4((vecColor).xyz,1);
   velvety1 = clamp(velvety1,0,1);
   Color.rgb += velvety1*gloss;
	//
	*/
	
	return float4(Color.rgb,1);
}

technique terrain
{
	pass p0
	{
		//cullmode = none;
		//zwriteenable = true;
		//alphablendenable = true;
		VertexShader = compile vs_2_0 terrain_VS();
		PixelShader = compile ps_2_a terrain_PS();
	}
}