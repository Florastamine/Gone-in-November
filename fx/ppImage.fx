/**
 * Uniform background image postprocessing shader v0.1c (2013-01-06)
 *
 * Copyright (C) 2012, 2013 by Christian Behrenberg
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

texture mtlSkin1;
sampler2D smpImage = sampler_state
{
	Texture = <mtlSkin1>;
};

float4 vecSkill1; // .xy = norm, .zw = scale
float4 vecSkill5; // .xy = offset

float4 PS (float2 texCoord : TEXCOORD0) : COLOR0
{
	float2 norm = vecSkill1.xy;
	float2 scale = vecSkill1.zw;

	float2 sampleCoords = (texCoord - 0.5) * norm;
	
	// make coordinate distortion free
	sampleCoords *= scale;
	
	sampleCoords -= vecSkill5.xy * scale * norm;
	
	sampleCoords.x = min(sampleCoords.x, 0.5 * norm.x * scale.x);
	sampleCoords.y = min(sampleCoords.y, 0.5 * norm.y * scale.y);
	
	// retransform coordinate so that image fits screen
	sampleCoords += 0.5 * norm;
	
	// sample pixel and return
	float3 color = tex2D(smpImage, sampleCoords).rgb;
	return float4(color, 1);
}

technique ppImage
{
	pass p0
	{
		AlphaBlendEnable = false;
		PixelShader = compile ps_2_0 PS();
	}
}