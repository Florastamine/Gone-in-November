Texture TargetMap; // scene 1
Texture mtlSkin1; //scene 2

sampler2D sceneSampler = sampler_state { texture = <TargetMap>;};
sampler2D firstSampler = sampler_state { texture = <mtlSkin1>;};
sampler2D secondSampler = sampler_state { texture = <mtlSkin1>;};
sampler2D thirdSampler = sampler_state { texture = <mtlSkin1>;};
sampler2D fourthSampler = sampler_state { texture = <mtlSkin1>;};

float4 vecSkill1;

float4 combinePS (float2 Tex : TEXCOORD0) : COLOR
{
	float4 Color =
	(tex2D(firstSampler,Tex)*vecSkill1.x)
	//+(tex2D(secondSampler,Tex)*vecSkill1.y/1.5)
	//+(tex2D(thirdSampler,Tex)*vecSkill1.z/1.5)
	//+(tex2D(fourthSampler,Tex)*vecSkill1.w/1.5)
	+tex2D(sceneSampler,Tex);
	//Color = tex2D(secondScene,Tex);
	return Color;
}

technique ntt
{
	pass one
	{
		PixelShader = compile ps_2_0 combinePS();
	}
}