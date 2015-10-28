texture TargetMap; //Bloom
texture mtlSkin1; //Original Scene
texture mtlSkin2; //Gamma

float4 vecSkill1;

float4 vecViewPort;

sampler originalImageSampler = sampler_state
{
   texture = (mtlSkin1);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
};

sampler blurImageSampler = sampler_state
{
   texture = (TargetMap);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
};

sampler gammaImageSampler = sampler_state
{
   texture = (mtlSkin2);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = wrap;
   AddressV = wrap;
};

float4 main(float2 tc: TEXCOORD0, float2 tc2: TEXCOORD1) : COLOR{
	float fExposureLevel= 2.0f;

	float4 original = tex2D(originalImageSampler, tc/vecSkill1.w);
	float4 blur = tex2D(blurImageSampler, tc/vecSkill1.w);
	float4 gamma = tex2D(gammaImageSampler, tc*vecSkill1.w);
	
	
	float4 color = lerp(original, blur, 0.45);

	tc -= float2(2.0f,1.5f); // Put coordsin -1/2 to 1/2 range
	tc /= 5;
	// Square of distance from origin (center of screen)
	float vignette = 1.0 -dot(tc, tc);
	// Multiply by vignette to the fourth
	color = color * vignette;
	color += color*(1-gamma.x)*fExposureLevel; // Apply simple exposure level
	
	//color = blur;
	//color*= 1.2;

	return pow(color, 1.0); // Apply gamma and return

}

technique HDR {
pass p1 {
	VertexShader = null;
	PixelShader = compile ps_2_0 main();
	}
}