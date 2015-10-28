Texture TargetMap;
sampler2D ColorSampler = sampler_state { texture = <TargetMap>;};

float4 vecViewPort; // contains viewport pixel size in zw components
float4 vecSkill1;

#define NUM_SAMPLES 20
float illuminationDecay = 2;
float Weight = 0.25;
float Decay = 0.9;
float Exposure = 1;
float Gamma = 1;

float2 poission[7] = {
   -0.00695914,  0.00457137,
   -0.00203345,  0.00620716,
    0.00962340, -0.00194983,
    0.00507431,  0.0064425,
    0.00896420,  0.00412458,
   -0.00321940, -0.00932615,
   -0.00791559, -0.00597705,
};

float4 RadialBlurPS( float2 texCoord : TEXCOORD0 ) : COLOR0 
{
	vecSkill1.z = vecSkill1.z/10;
	texCoord -= vecViewPort.zw;
	float4 OrgScene = tex2D(ColorSampler,texCoord);
	half2 deltaTexCoord = (texCoord - vecSkill1.xy);
	deltaTexCoord *= 1.0f / NUM_SAMPLES * vecSkill1.z;
	half3 color = tex2D(ColorSampler,texCoord);
	
	for(int i = 0; i < NUM_SAMPLES; i++)
	{
		texCoord -= deltaTexCoord;
		half3 sample = tex2D(ColorSampler, texCoord);
		sample *= illuminationDecay;
		color += sample;
		illuminationDecay -= 0.1;
	}
	color /= 20;
	return float4(color*Exposure,1);
}

technique radialBlur
{
	pass one
	{
		
		PixelShader = compile ps_2_0 RadialBlurPS();
	}
}