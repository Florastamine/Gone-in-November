/////////////////////////////////////////////
/////////Chrome Shader by ello///////////////
/////////www.earthcontrol.de/////////////////
/////////////////////////////////////////////


float4x4 matWorldViewProj;
float4x4 matWorldInv;
float4x4 matWorldView;
float4x4 matWorld;

float4x4 matViewInv;
float4x4 matView;
float4 vecSkill41;
float4 mtlSkill1;
float4 vecSunDir;

float4 vecFog;

float DoFog(float3 Pos)
{
	// convert the vector position to view space to get it's depth (.z)
	float3 P = mul(Pos,matWorldView);
	// apply the linear fog formula
	return saturate((vecFog.y-P.z) * vecFog.z);
}

void mainVS(
in 	float4 inPos 	: POSITION,
in 	float3 inNormal: NORMAL,
in		float2 inXY		: TEXCOORD0,
out	float4 Pos 		: POSITION,
out 	float2 chrome		: TEXCOORD0,
out 	float2 uv		: TEXCOORD1,
out	float oNrm : COLOR0,
out	float  Fog		: FOG
)
{
	// transform the vector position to screen coordinates
	Pos = mul(inPos,matWorldViewProj);
	// rotate and normalize the normal
	float3 N = normalize(mul(inNormal,matWorldInv));
	// Add fog
	Fog = DoFog(inPos);
	// convert texture coordinates or do other stuff
	oNrm = mul(inNormal,matWorld); //pass normal
	chrome=mul(N,matView)*mtlSkill1.x; //mtlSkill1 contains the scale of the reflMap
	uv = inXY;
}

texture mtlSkin1;
sampler basemap = sampler_state 
{
	texture=(mtlSkin1);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture entSkin1;
sampler entskin = sampler_state 
{
	texture=(entSkin1);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


float4 mainPS(
in		float2 chrome		: TEXCOORD0,
in		float2 uv		: TEXCOORD1,
in	float iNrm : COLOR0
) : COLOR
{
	//change shinyness where "2" stands , play around whith it till it fits your needs
	//skill41-43 contains rgb values, skill44 contains power value
	float4 result=0.5*dot(tex2D(basemap,chrome),vecSkill41.w)-0.5; 
	//change the "2" below to adjust the shyniness, too
	result*=result/2+float4(vecSkill41.xyz,1)*tex2D(basemap,chrome); 
	float sunL = 0.5*dot(iNrm,-vecSunDir)+0.5; //sunLight

	//return (result-sunL+tex2D(entskin,uv))*pow(sunL,2); //dark metallic, mixed with entSkin1
	return (result+tex2D(entskin,uv))*sunL; //standard, mixed with entSkin1
}

technique better_looking_chrome
{
	pass base
	{
		VertexShader=compile vs_2_0 mainVS();
		PixelShader=compile ps_2_0 mainPS();
	}
}

technique fallback_chrome
{
	pass base
	{
		VertexShader=compile vs_1_1 mainVS();
		PixelShader=compile ps_1_4 mainPS();
	}
}
