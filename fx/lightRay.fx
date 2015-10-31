Texture TargetMap;
Texture mtlSkin1;
sampler2D ColorSampler = sampler_state { texture = <TargetMap>;};
//sampler2D sceneSampler = sampler_state { texture = <TargetMap>;};

float4 vecViewPort; // contains viewport pixel size in zw components
float4 vecSkill1;

/*
#define NUM_SAMPLES 40
float illuminationDecay = 4;
float Weight = 0.25;
//float Decay = 0.9;
float Exposure = 1;
float Gamma = 1;


float4 RadialBlurPS( float2 texCoord : TEXCOORD0 ) : COLOR0 
{

	float2 sceneTexCoord = texCoord;
	
	//texCoord *= 4;
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
	color /= 40;
	//color += tex2D(sceneSampler,sceneTexCoord).xyz;
	//color = depth;
	//return (1-tex2D(ColorSampler,texCoord))*tex2D(sceneSampler, texCoord);
	return float4((color*Exposure), 1);
}

technique radialBlur
{
	pass one
	{
		
		PixelShader = compile ps_3_0 RadialBlurPS();
	}
}
*/



#define NUM_SAMPLES 20
float illuminationDecay = 2;
float Weight = 0.25;
float Decay = 0.9;
float Exposure = 1;
float Gamma = 1;


float4 RadialBlurPS( float2 texCoord : TEXCOORD0 ) : COLOR0 
{
	float2 sceneTexCoord = texCoord;
	
	//texCoord *= 4;
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
	//color += tex2D(sceneSampler,sceneTexCoord).xyz;
	//color = depth;
	//return (1-tex2D(ColorSampler,texCoord))*tex2D(sceneSampler, texCoord);
	return float4((color*Exposure), 1);
}

technique radialBlur
{
	pass one
	{
		
		PixelShader = compile ps_2_0 RadialBlurPS();
	}
}
