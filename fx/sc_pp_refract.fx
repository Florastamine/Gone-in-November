Texture TargetMap;
Texture mtlSkin1; //bumpMap
Texture mtlSkin2; //Depthmap

sampler2D orgScene= sampler_state { texture = <TargetMap>;};

sampler2D bumpSampler = sampler_state
{
   texture = (mtlSkin1);
   MinFilter = linear; 
   MagFilter = linear; 
   MipFilter = linear; 
   AddressU = Wrap;
   AddressV = Wrap;
};

sampler2D  depthSampler = sampler_state { texture = <mtlSkin2>;};

float4 vecSkill1;
float4 vecTime;

float4 HeatPS (float2 Tex : TEXCOORD0) : COLOR
{
	if(vecSkill1.x == 0) vecSkill1.x = 1;
	if(vecSkill1.z == 0) vecSkill1.z = 1;
	float4 bump = tex2D(bumpSampler, float2(Tex.x, Tex.y + vecSkill1.y*(vecTime.w/40))) * 2 - 1;
	bump = normalize(bump);
	float2 mid = (Tex + bump.xy * 0.15f * vecSkill1.x);
	float4 refraction = tex2D(orgScene, mid);
	
	float depth = tex2D(depthSampler,Tex).x*vecSkill1.z;
	depth = clamp(depth,0,1);
	refraction.rgb *= 1-depth;
	refraction.rgb += depth * tex2D(orgScene,Tex).rgb;

	return(refraction);
}

technique emboss
{
	pass one
	{
		PixelShader = compile ps_2_0 HeatPS();
	}
}