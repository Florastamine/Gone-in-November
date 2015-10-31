texture TargetMap;
float4 vecSkill1;

sampler depthSampler = sampler_state
{
	texture = <TargetMap>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter= LINEAR;
	AddressU = clamp;
	AddressV = clamp;
};

float4 blurDepth(float2 Tex : TEXCOORD0):COLOR0
{
	float depth = tex2D(depthSampler,Tex).x;
	float finalDepth = depth;
	float newDepth = 0;
	float blurDepth = 0.5;
	float blurStr = vecSkill1.x; // 0.0018;
	
	newDepth = tex2D(depthSampler,Tex+float2(blurStr,0)).x;
	if(newDepth < depth + blurDepth && newDepth > depth-blurDepth) finalDepth += newDepth;
	else finalDepth += depth;
	
	newDepth = tex2D(depthSampler,Tex+float2(0,blurStr)).x;
	if(newDepth < depth + blurDepth && newDepth > depth-blurDepth) finalDepth += newDepth;
	else finalDepth += depth;
	
	newDepth = tex2D(depthSampler,Tex+float2(-blurStr,0)).x;
	if(newDepth < depth + blurDepth && newDepth > depth-blurDepth) finalDepth += newDepth;
	else finalDepth += depth;
	
	newDepth = tex2D(depthSampler,Tex+float2(0,-blurStr)).x;
	if(newDepth < depth + blurDepth && newDepth > depth-blurDepth) finalDepth += newDepth;
	else finalDepth += depth;
	
	
	newDepth = tex2D(depthSampler,Tex+float2(blurStr/2,blurStr/2)).x;
	if(newDepth < depth + blurDepth && newDepth > depth-blurDepth) finalDepth += newDepth;
	else finalDepth += depth;
	
	newDepth = tex2D(depthSampler,Tex+float2(-blurStr/2,-blurStr/2)).x;
	if(newDepth < depth + blurDepth && newDepth > depth-blurDepth) finalDepth += newDepth;
	else finalDepth += depth;
	
	newDepth = tex2D(depthSampler,Tex+float2(-blurStr/2,blurStr/2)).x;
	if(newDepth < depth + blurDepth && newDepth > depth-blurDepth) finalDepth += newDepth;
	else finalDepth += depth;
	
	newDepth = tex2D(depthSampler,Tex+float2(blurStr/2,-blurStr/2)).x;
	if(newDepth < depth + blurDepth && newDepth > depth-blurDepth) finalDepth += newDepth;
	else finalDepth += depth;
	
	finalDepth /= 9;
	
	return float4(finalDepth,0,0,0);
	
}

technique tBlurDepth
{
	pass p0
	{
		PixelShader = compile ps_2_a blurDepth();
	}
}