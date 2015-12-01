/**
 * Grayscale postprocessing shader v0.3 (2012-01-06)
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
 
texture TargetMap;
sampler2D smpImage = sampler_state
{
	Texture = <TargetMap>;
};

// if <0, colors are boosted by their luminance, if >1, colors are inverted
float4 vecSkill1; // .y = alpha

float fRgbDot (float3 color, float c1, float c2, float c3)
{
	return dot(color, float3(c1, c2, c3));
}

float fColorLuma (float3 color)
{
	return fRgbDot(color, 0.299, 0.587, 0.114);
}

float fRgbAvg (float3 color)
{
	return (color.r + color.g + color.b) / 3;
}

float fRgbMin (float3 f)
{
	return min(min(f.r, f.g), f.b);
}

float fRgbMax (float3 f)
{
	return max(max(f.r, f.g), f.b);
}

float fColorDesat (float3 f)
{
	return (fRgbMax(f) + fRgbMin(f)) * 0.5;
}

float3 flt3 (float f)
{
	return float3(f, f, f);
}

float4 PS_none (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	return float4(image, 1);
}

float4 PS_luma (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	float l = fColorLuma(image);
	return float4(lerp(image, flt3(l), vecSkill1.y), 1);
}

float4 PS_desat (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	float d = fColorDesat(image);
	return float4(lerp(image, flt3(d), vecSkill1.y), 1);
}

float4 PS_avg (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	float a = fRgbAvg(image);
	return float4(lerp(image, flt3(a), vecSkill1.y), 1);
}

float4 PS_min (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	float m = fRgbMin(image);
	return float4(lerp(image, flt3(m), vecSkill1.y), 1);
}

float4 PS_max (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	float m = fRgbMax(image);
	return float4(lerp(image, flt3(m), vecSkill1.y), 1);
}

float4 PS_r (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	return float4(lerp(image, flt3(image.r), vecSkill1.y), 1);
}

float4 PS_g (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	return float4(lerp(image, flt3(image.g), vecSkill1.y), 1);
}

float4 PS_b (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	return float4(lerp(image, flt3(image.b), vecSkill1.y), 1);
}

technique tNone
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_none();
	}
}

technique tLuma
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_luma();
	}
}

technique tDesat
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_desat();
	}
}

technique tAvg
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_avg();
	}
}

technique tMin
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_min();
	}
}

technique tMax
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_max();
	}
}

technique tR
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_r();
	}
}

technique tG
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_g();
	}
}

technique tB
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_b();
	}
}