float4 vecSkill1;

Texture TargetMap;

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
float4 vecTime;
float4 DownsamplePS( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	
	float4 Color = tex2D(orgScene,Tex.xy*4);
	
	float gamma = Color.r;
	
	//Color.r = Color.r + (Color.g - Color.r) * ( 1 - pow( 0.98f, 2.1001*(vecTime.x)) );
	Color.r = Color.r + (Color.g - Color.r) * ( 1 - pow( 0.98f, vecSkill1.x*(vecTime.x)) );
	
	return Color;
}

technique thdrGamma
{
	pass one
	{
		alphablendenable = false;
		PixelShader = compile ps_2_0 DownsamplePS();
	}
}