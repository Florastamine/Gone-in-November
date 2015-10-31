Texture TargetMap;
sampler2D smpSource = sampler_state { texture = <TargetMap>;};

float4 vecSkill1;


float Luminance = 0.08f; // 0.08
float fMiddleGray = 0.18f; // 0.18
float fWhiteCutoff = 0.8f; // 0.8
float bloomStr = 8.0f;

float4 BrightPassFilter( in float2 Tex : TEXCOORD0 ) : COLOR0
{
	
	bloomStr = vecSkill1.x;
	Luminance = vecSkill1.y;
	fMiddleGray = vecSkill1.z;
	fWhiteCutoff = vecSkill1.w;
	
    float3 ColorOut = tex2D( smpSource, Tex );

	  
    ColorOut *= fMiddleGray / ( (Luminance) + 0.001f );
    ColorOut *= ( 1.0f + ( ColorOut / ( fWhiteCutoff * fWhiteCutoff ) ) );
    
    ColorOut -= 5.0f;
    	
    ColorOut = max( ColorOut, 0.0f );
    
    ColorOut /= ( (100.0f - bloomStr) + ColorOut );
    
    ColorOut.rgb *= 2;//pow(ColorOut.rgb,2)*2;
    ColorOut = clamp(ColorOut,0,1);
    
    
    
	
	 return float4( ColorOut, 1.0f );

}





technique highpass
{
	pass one
	{
		cullmode = CCW;
		PixelShader = compile ps_2_0 BrightPassFilter();
	}
}