Texture mtlSkin2; //scene
Texture mtlSkin1; //depth

sampler2D firstScene= sampler_state { texture = <mtlSkin1>;};
sampler2D secondScene = sampler_state { texture = <mtlSkin2>;};

float4 combinePS (float2 Tex : TEXCOORD0) : COLOR
{
	float4 Color = tex2D(firstScene,Tex*2);
	Color.rgb = Color.r;
	if(Color.r > 0.005) Color.r = 1;
	Color.rgb = 1-Color.r;
	//Color = tex2D(secondScene,Tex);
	Color.rgb *= tex2D(secondScene,Tex*2).rgb;
	return Color;
}

technique ntt
{
	pass one
	{
		PixelShader = compile ps_2_0 combinePS();
	}
}