Texture TargetMap; // scene 1
Texture mtlSkin1; //scene 2

sampler2D firstScene= sampler_state { texture = <TargetMap>;};
sampler2D secondScene = sampler_state { texture = <mtlSkin1>;};

float4 vecSkill1;
float4 vecTime;

float4 combinePS (float2 Tex : TEXCOORD0) : COLOR
{
	float4 Color = tex2D(firstScene,Tex);
	Color += tex2D(secondScene,Tex);
	
	return Color;
}

technique ntt
{
	pass one
	{
		PixelShader = compile ps_2_0 combinePS();
	}
}