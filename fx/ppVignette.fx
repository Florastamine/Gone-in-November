/**
 * Vignette postprocessing shader v0.1 (2012-12-15)
 *
 * Copyright (C) 2012 by Christian Behrenberg
 * http://www.christian-behrenberg.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to per-
 * mit persons to whom the Software is furnished to do so, subject to the following con-
 * ditions:
 *  
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, IN-
 * CLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LI-
 * ABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
 
float vignetteRatio (float2 coord, float2 center, float start, float end, float alpha)
{
	float dist = distance(coord, center);
	float ratio = smoothstep(start, end, dist);
	ratio *= alpha;
	
	return ratio;
}
 
texture TargetMap;
sampler2D smpImage = sampler_state
{
	Texture = <TargetMap>;
};

float4 vecSkill1; // .xy = center, .z = start, .w = end
float4 vecSkill5; // .x = alpha
float4 vecAmbient; // color

float4 PS (float2 texCoord : TEXCOORD0) : COLOR0
{
	// calculate vignette ratio
	float f = vignetteRatio(texCoord, vecSkill1.xy, vecSkill1.z, vecSkill1.w, vecSkill5.x);
	
	// sample image and mix with color by vignette ratio
	float3 image = tex2D(smpImage, texCoord).rgb;
	float3 result = lerp(image, vecAmbient.rgb, f);
	
	// return vignett'ed image
	return float4(result, 1);
}

technique ppVignette
{
	pass p0
	{
		AlphaBlendEnable = false;
		
		VertexShader = null;
		PixelShader = compile ps_2_0 PS();
	}
}