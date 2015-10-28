
texture TargetMap; //Blurred Scene + Depth
texture mtlSkin1; // Scene
texture mtlSkin2; // Depthmap

float4 vecSkill1;
float4 vecViewPort;

sampler blurredSampler = sampler_state
{
   texture = (TargetMap);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
};

sampler sceneSampler = sampler_state
{
   texture = (mtlSkin1);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
};

sampler depthSampler = sampler_state
{
   texture = (mtlSkin2);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
};


float4 main(float2 tc: TEXCOORD0) : COLOR{
	
	float4 depth = tex2D(depthSampler, tc/vecSkill1.w);
	float4 blurred = tex2D(blurredSampler, tc/vecSkill1.w);
	float4 scene = tex2D(sceneSampler, tc/vecSkill1.w);
	float4 dof;
	
	depth = clamp(depth,0,1); //32bit floating point "fallback"
	
	dof.rgb = (scene.rgb*(depth.b))+(blurred.rgb*(1-depth.b));
	dof.a = 1;
	
	return dof;
}


technique DoF {
pass p1 {
	VertexShader = null;
	PixelShader = compile ps_2_0 main();
	}
}