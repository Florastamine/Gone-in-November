Texture TargetMap;
float4 vecSkill1;

sampler orgScene = sampler_state
{
   texture = (TargetMap);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
};


float4 vecViewPort; // contains viewport pixel size in zw components
float4 DownsamplePS( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	
	float4 Color = tex2D(orgScene,Tex.xy*vecSkill1.x);
	return Color;
}

technique emboss
{
	pass one
	{
		
		PixelShader = compile ps_2_0 DownsamplePS();
	}
}