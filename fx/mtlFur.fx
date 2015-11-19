float4x4 matWorldViewProj;

void mainVS
(
in float4 position:POSITION,
in float4 normal:NORMAL,
in float2 texCoord:TEXCOORD0,
out float4 pos:POSITION,
out float2 oT0:TEXCOORD0,
uniform float offset
)
{
	position.xyz+=normal.xyz*offset;
	//scale vertexposition in normals direction
	//changing the 100 value changes the 'distance' or height of the fur

	pos=mul(position,matWorldViewProj);
	oT0=texCoord*1; //scale for fur density 14--->original comment
	//this is the amount of tiling of the fur texture
}

//Define the material skins which we use;
texture entSkin1;
sampler basemap = sampler_state
{
	texture=(entSkin1);
};

texture mtlSkin1;
sampler noiseMap=sampler_state
{
	texture=(mtlSkin1);
};

texture mtlSkin2;
sampler furmap = sampler_state
{
	texture=(mtlSkin2);
};

// Next the main pixel shader (can't explain much on it though)
float4 mainPS
(
in float4 nrm:COLOR0,
in float2 texCoord:TEXCOORD0,
uniform float alpha
) : COLOR
{
	float4 base=tex2D(basemap,texCoord); //mtlskin1 sampler
	float4 fur=tex2D(furmap,texCoord); //mtlskin2 sampler
	float4 noise=tex2D(noiseMap,texCoord);
	base *= fur; //this is for color fur
	base.a=alpha*noise.rgb; //this is for making it transparent using the noisemap as for, 'how much and where' transparency should be(???)
	return base;
}

// Through these passes, the amount of fur-layers are determined, the more passes, the less fps (and even a changing effect, too much contrast)
technique test1
{
	pass p0
	{
		alphablendenable=true;
		
		VertexShader=compile vs_1_1 mainVS(0);
		PixelShader=compile ps_1_4 mainPS(1);
	}

	//fur part:
	pass p1
	{
		AlphaBlendEnable=true;
		destBlend=13;
		
		VertexShader=compile vs_1_1 mainVS(1); //the last numbers gives the height of the layer
		PixelShader=compile ps_1_4 mainPS(0.9); //the last numbers sets the transparency of this pass
	}

	pass p2
	{
		AlphaBlendEnable=true;
		destBlend=13;
		
		VertexShader=compile vs_1_1 mainVS(2);
		PixelShader=compile ps_1_4 mainPS(0.8);
	}

	pass p3
	{
		AlphaBlendEnable=true;
		destBlend=13; //7/9
		
		VertexShader=compile vs_1_1 mainVS(3);
		PixelShader=compile ps_1_4 mainPS(0.5);
	}

	pass p4
	{
		AlphaBlendEnable=true;
		zWriteEnable = True;
		destBlend=13;
		
		VertexShader=compile vs_1_1 mainVS(4);
		PixelShader=compile ps_1_4 mainPS(0.3);
	}
	
	pass p5
	{
		AlphaBlendEnable=true;
		zWriteEnable = True;
		destBlend=13;
		
		VertexShader=compile vs_1_1 mainVS(5);
		PixelShader=compile ps_1_4 mainPS(0.1);
	}
}
