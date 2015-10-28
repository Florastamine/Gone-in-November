/*
struct SMapVertexToPixel
{
	float4 Position     : POSITION;    
	float3 Position2D    : TEXCOORD0;
};

struct SMapPixelToFrame
{
	float4 Color : COLOR0;
};

struct SSceneVertexToPixel
{
	float4 Position             : POSITION;
	float4 ShadowMapSamplingPos : TEXCOORD0;    

	float4 RealDistance            : TEXCOORD1;
	float2 TexCoords            : TEXCOORD2;
	float3 Normal                : TEXCOORD3;
	float3 Position3D            : TEXCOORD4;

};

struct SScenePixelToFrame
{
	float4 Color : COLOR0;
};

//------- Constants --------

//float4x4 xLightWorldViewProjection;
matrix matWorldViewProj;
float xMaxDepth = 10000;

//------- Texture Samplers --------


Texture xColoredTexture;

sampler ColoredTextureSampler = sampler_state { texture = <xColoredTexture> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = mirror; AddressV = mirror;};

Texture xShadowMap;

sampler ShadowMapSampler = sampler_state { texture = <xShadowMap> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = clamp; AddressV = clamp;};
//------- Vertex Shaders --------

SMapVertexToPixel ShadowMapVertexShader( float4 inPos : POSITION)
{
	SMapVertexToPixel Output = (SMapVertexToPixel)0;
	
	Output.Position = mul(inPos, matWorldViewProj);
	Output.Position2D = Output.Position;
	
	return Output;    
}

//------- Pixel Shaders --------
SMapPixelToFrame ShadowMapPixelShader(SMapVertexToPixel PSIn)
{
	SMapPixelToFrame Output = (SMapPixelToFrame)0;

	Output.Color = PSIn.Position2D.z/xMaxDepth;
	//float depth = Output.Color.r;
	//Output.Color.r = floor(Output.Color.r*127)/127;
	//Output.Color.g = floor((depth-Output.Color.r)*127*127)/127;

	return Output;
}

//------- Techniques --------

technique ShadowMap
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 ShadowMapVertexShader();
		PixelShader = compile ps_2_0 ShadowMapPixelShader();
	}
}*/

matrix matWorldViewProj;
matrix matWorld;
float4 vecSkill1; //x focal plane pos | y focal Plane depth / area of sharpness
float4 vecSkill5; //x waterplane pos | y max waterdepth

texture entSkin1;

sampler2D ColorSampler = sampler_state
{
	Texture = <entSkin1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct SMapVertexToPixel
{
	float4 Position     : POSITION;    
	float3 Position2D    : TEXCOORD0;
	float2 Tex : TEXCOORD1;
	float2 PositionY : TEXCOORD2;
};

struct SMapPixelToFrame
{
	float4 Color : COLOR0;
};



SMapVertexToPixel depthmapVS( float4 inPos : POSITION, float2 inTex : TEXCOORD0)
{
	SMapVertexToPixel Output = (SMapVertexToPixel)0;
	
	Output.Position = mul(inPos, matWorldViewProj);
	Output.Position2D = Output.Position;
	Output.Tex = inTex;
	
	//create waterdepth
	float4 temp=mul(inPos,matWorld);
	Output.PositionY=temp.y;
	//modify the geometry y coord , to take into account plane height
	Output.PositionY.y-=vecSkill5.x;
	//
	
	return Output;    
}


float4 depthmapPS(SMapVertexToPixel PSIn) : COLOR0
{
	vecSkill1.w = 5000;
	//if(vecSkill1.z > 1) clip(-1);
	
	//scene depth
	float4 Color = ((PSIn.Position2D.z-0)/vecSkill1.w);
	Color.r = ((PSIn.Position2D.z-0)/vecSkill1.w);//(Color.r + Color.g + Color.b)/3;
	
	//
	
	/*
	//waterdepth
	float pixelYDepth=(PSIn.PositionY.y/vecSkill5.y+1);
	//clamp values to 0,1
	pixelYDepth=saturate(pixelYDepth);
	Color.g = pixelYDepth;
	//
	
	//dof depth
	Color.b = 1-((PSIn.Position2D.z-vecSkill1.y)/vecSkill1.x) * ((PSIn.Position2D.z-vecSkill1.y)/vecSkill1.x);
	if(vecSkill1.z > 0) Color.b = vecSkill1.z;
	//Color.b = 0;
	//
	*/
	
	Color.a = tex2D(ColorSampler,PSIn.Tex).a;
	
	
	//SSAO
	float depth = Color.r;
	Color.r = floor(Color.r*127)/127;
	Color.g = floor((depth-Color.r)*127*127)/127;
	Color.b = floor((depth-Color.r)*127*127*127)/127;
	//Color.gb = 0;
	return Color;
}

technique Depthmap
{
	pass Pass0
	{	
		VertexShader = compile vs_1_1 depthmapVS();
		PixelShader = compile ps_2_0 depthmapPS();
	}
}
