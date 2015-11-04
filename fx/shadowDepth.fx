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
	
	//scene depth
	float4 Color = ((PSIn.Position2D.z-0)/vecSkill1.w);
	Color.r = ((PSIn.Position2D.z-0)/vecSkill1.w);//(Color.r + Color.g + Color.b)/3;
	
	Color.a = tex2D(ColorSampler,PSIn.Tex).a;
	
	//SSAO
	float depth = Color.r;
	Color.r = floor(Color.r*127)/127;
	Color.g = floor((depth-Color.r)*127*127)/127;
	Color.b = floor((depth-Color.r)*127*127*127)/127;
	
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
