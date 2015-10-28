Texture TargetMap;
Texture mtlSkin1; //Depthmap
sampler2D orgScene= sampler_state { texture = <TargetMap>;};
sampler2D Depthmap= sampler_state { texture = <mtlSkin1>;};
float4 vecSkill1;

float4 vecViewPort; // contains viewport pixel size in zw components
float4 DownsamplePS( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	
	float4 Color = tex2D(orgScene,Tex.xy*vecSkill1.x);
	Color.a = tex2D(Depthmap,Tex.xy*vecSkill1.x).b;
	return Color;
}

technique emboss
{
	pass one
	{
		alphablendenable=false;
		PixelShader = compile ps_2_0 DownsamplePS();
	}
}