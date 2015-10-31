Texture mtlSkin1; //scene
Texture mtlSkin2; //scene+bloom

sampler orgScene = sampler_state
{
   texture = (mtlSkin1);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
};

sampler bloomScene = sampler_state
{
   texture = (mtlSkin2);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
};


float4 vecViewPort; // contains viewport pixel size in zw components
float4 DownsamplePS( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	
	float4 Color = tex2D(orgScene,Tex.xy*4);
	float3 bloom = tex2D(bloomScene,Tex.xy*4);
	bloom = bloom.x;
	float sceneGamma = (Color.r+Color.g+Color.b)/3;
	
	//Color.r = lerp((Color.r+Color.g+Color.b)/3,bloom,0.996);
	//Color.r = lerp((Color.r+Color.g+Color.b)/12,bloom,0.998);
	Color.r = lerp((Color.r+Color.g+Color.b)/3,bloom,0.9999);

	Color.g = sceneGamma;
		
	return Color;
}

technique hdrGamma
{
	pass one
	{
		
		PixelShader = compile ps_2_0 DownsamplePS();
	}
}