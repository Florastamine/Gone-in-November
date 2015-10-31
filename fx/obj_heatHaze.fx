/******************************************************************************************************
Heat-Haze-Shader by Wolfgang "BoH_Havoc" Reichardt

Entity Textures:
	Skin1 = Normalmap
	Skin2 = Colormap 

Usage:
	Uncomment/Comment the #defines to add/remove an effect.
	vecSkill1.y sets the movement of the normalmap
	
******************************************************************************************************/


/***************************************TWEAKABLES*****************************************************/

//Refraction Strength
float refractStrength = 0.15;

//Use Colormap (entSkin2) to color the entity
#define USECOLOR

/***************************************SHADER*CODE****************************************************/

float4x4 matWorldViewProj;
float4x4 matWorld;

float4 vecSkill1;
float4 vecSkill5;
float4 vecTime;

texture entSkin1;

sampler normalmapSampler = sampler_state
{
	Texture = <entSkin1>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;   
	//MinFilter = POINT;
	//MagFilter = POINT;
	//MipFilter = POINT;   
	AddressU  = wrap;
	AddressV  = wrap;
	
};

texture entSkin2;

sampler colorSampler = sampler_state
{
	Texture = <entSkin2>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;   
	//MinFilter = POINT;
	//MagFilter = POINT;
	//MipFilter = POINT;   
	AddressU  = wrap;
	AddressV  = wrap;
	
};

texture mtlSkin1;

sampler refractionSampler=sampler_state
{
	texture=<mtlSkin1>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    addressu=clamp;
    addressv=clamp;
};

void heat_VS( in float4 inPos:POSITION, //input vertex position
					in float2 inTex:TEXCOORD0, //input texcoords
				   out float4 outPos:POSITION, //ouput transformed position
				   out float4 outTex:TEXCOORD0, //output texcoords
				   out float4 outProjectedTex:TEXCOORD1 //output projected texcoords
				   )
{
	
	outPos=mul(inPos,matWorldViewProj);
	//save the vertex information for further projected texture coordinate computation
	outProjectedTex=outPos;
	
	float4 ShadowMapSamplingPos = mul(inPos, matWorldViewProj);

	outProjectedTex.x = ShadowMapSamplingPos.x;
	outProjectedTex.y = ShadowMapSamplingPos.y;
	outProjectedTex.z = ShadowMapSamplingPos.w;
	
	//outProjectedTex = ShadowMapSamplingPos;
	
	//ouput texcoords
	outTex.xy=inTex+(vecTime.w*0.001*vecSkill1.y);
	outTex.zw=inTex;
}


float4 heat_PS(in float4 inTex:TEXCOORD0, //input texture coords
					in float4 inProjectedTex:TEXCOORD1 //projectedTexcoords, needed to perform reflections,refractions
):COLOR0
{
	clip(vecSkill1.x);
	
	/*
	inProjectedTex.xy/=inProjectedTex.w;
	inProjectedTex.xy=inProjectedTex.xy*0.5f*float2(1,-1)+0.5f;
	*/
	float3 pixelNormal=tex2D(normalmapSampler,inTex.xy).xyz;
   pixelNormal.yz=pixelNormal.zy;
   pixelNormal.xyz=(pixelNormal.xyz-0.5f)*2;
   
   
   
   float2 ProjectedTexCoords;
   ProjectedTexCoords[0] = inProjectedTex.x/inProjectedTex.z/2.0f +0.5f;
   ProjectedTexCoords[1] = -inProjectedTex.y/inProjectedTex.z/2.0f +0.5f;
   
   float4 refraction=tex2D(refractionSampler,ProjectedTexCoords.xy-refractStrength*pixelNormal.xz);
	
	float4 color = 1;
	
	//color.rgb = pixelNormal.rgb;
	color = refraction;
	
	#ifdef USECOLOR
		color.rgb *= tex2D(colorSampler,inTex.zw-refractStrength*pixelNormal.xz).xyz;
	#endif
	
	color.a = tex2D(normalmapSampler,inTex.zw).a;
	color.rgb *= vecSkill5.xyz;
	
   return color;
};

technique heat
{

	pass p0
	{
		CULLMODE = CCW;
		VertexShader  = compile vs_2_0 heat_VS();
		PixelShader = compile ps_2_0 heat_PS();
	}
}