/******************************************************************************************************
Heat-Haze-Shader by Wolfgang "BoH_Havoc" Reichardt

Entity Textures:
	Skin1 = Normalmap
	Skin2 = Colormap 

Usage:
	Uncomment/Comment the #defines to add/remove an effect.
	
******************************************************************************************************/


/***************************************TWEAKABLES*****************************************************/

float refractStrength = 0.5;

//Use Colormap (entSkin2) to color the entity
#define USECOLOR

/***************************************SHADER*CODE****************************************************/

float4x4 matWorldViewProj;
float4x4 matWorld;

float4 vecSkill1;

texture entSkin1;
texture entSkin2;

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
				   out float2 outTex:TEXCOORD0, //output texcoords
				   out float4 outProjectedTex:TEXCOORD1 //output projected texcoords
				 )
{
	
	outPos=mul(inPos,matWorldViewProj);
	//save the vertex information for further projected texture coordinate computation
	outProjectedTex=outPos;
	//ouput texcoords
	outTex=inTex;
}


float4 heat_PS(in float2 inTex:TEXCOORD0, //input texture coords
					  in float4 inProjectedTex:TEXCOORD1 //projectedTexcoords, needed to perform reflections,refractions
					  ):COLOR0
{
	clip(vecSkill1.x);
	
	inProjectedTex.xy/=inProjectedTex.w;
	inProjectedTex.xy=inProjectedTex.xy*0.5f*float2(1,-1)+0.5f;
	float4 pixelNormal=tex2D(normalmapSampler,inTex);
   pixelNormal.yz=pixelNormal.zy;
   pixelNormal.xyz=(pixelNormal.xyz-0.5f)*2;
   
   float4 refraction=tex2D(refractionSampler,inProjectedTex.xy-refractStrength*pixelNormal.xz);
	
	float4 color = 1;
	color = refraction/2;
	
	#ifdef USECOLOR
		color.rgb *= tex2D(colorSampler,inTex).xyz;
	#endif
	
	color.a = pixelNormal.a;
	
   return color;
};

technique heat
{

	pass p0
	{
		CULLMODE = CW;
		VertexShader  = compile vs_2_0 heat_VS();
		PixelShader = compile ps_2_0 heat_PS();
	}

	pass p1
	{
		CULLMODE = CCW;
		alphablendenable=true;
   	srcblend=one;
    	destblend=one;
		VertexShader  = compile vs_2_0 heat_VS();
		PixelShader = compile ps_2_0 heat_PS();
	}
}