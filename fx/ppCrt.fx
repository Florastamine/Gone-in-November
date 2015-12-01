/**
 * Cathode ray tube (CRT) postprocessing shader v0.1 (2013-01-24)
 *
 * Copyright (C) 2013 by Christian Behrenberg
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


// NOTES
//
// PS_dotNBloom contains ported and modified parts from the "Dot 'n bloom"-shader from
// Hans-Kristian Arntzen (Themaister), which he has released at github as public domain
// code @ https://gist.github.com/1004382
//
// PS_RgbBars, PS_MagentaGreenMask & PS_RgbTriad contains code which was inspired by the
// discussion @ http://board.byuu.org/viewtopic.php?f=10&t=147, especially caligari's
// Phosphor shader code.


float4 vecViewPort; // .xy = size xy, .zw = 1/xy

float4 vecSkill1; // .y = alpha
float4 vecSkill5; // .y = gamma, .z = intensity, .w = scale

//  hot apple pi
float fPI = 3.1415926535897932384626433832795; // 180° (diet pi)
float fPI2 = 6.283185307179586476925286766559; // 360°


// general functions

float fRgbDot (float3 color, float c1, float c2, float c3)
{
	return dot(color, float3(c1, c2, c3));
}

float fColorLuma (float3 color)
{
	return fRgbDot(color, 0.299, 0.587, 0.114);
}

// like fmod, but also for negative numbers
float nfmod (float f, float fmax, float m)
{
	float n = step(f, 0);
	float s = sign(f);

	return m * n + s * fmod(n * fmax + s * f, m);
}


// sampler

texture TargetMap;
sampler2D smpImage = sampler_state
{
	Texture = <TargetMap>;
};


// subroutine prototypes
float3 colorDotBloom (float2 dots, float2 texCoord, float2 dotSize, float fGamma, float fIntensity);
float3 crtMaskSample (sampler2D smp, float2 dir, float2 texCoord);
float3 rgbRgbBars (float3 color, float2 screenCoord);
float3 rgbMgBars (float3 color, float2 screenCoord);
float3 rgbTriad (float3 color, float2 screenCoord);


// pixel shaders

float4 PS_none (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	return float4(image, 1);
}

float4 PS_scanline (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	float3 color = crtMaskSample(smpImage, float2(1, 0), texCoord);

	float lines = vecViewPort.y / (vecSkill5.w + vecSkill5.w);

	// do scanlining by overlaying a sine wave with <lines> periods
	float wave = sin(texCoord.y * lines * fPI2);
	float3 scanline = color * (color * (wave + vecSkill5.z) + 0.5);

	// add gamma
	scanline *= exp(vecSkill5.y);

	return float4(lerp(image, scanline, vecSkill1.y - 0.5), 1);
}

float4 PS_dotNBloom (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;

	float lines = vecViewPort.y / (vecSkill5.w * 4);
	float intensity = vecSkill5.z / 12;
	float gamma = -2.4 + vecSkill5.y;

	float2 dotDim = float2((vecViewPort.x * vecViewPort.w) * lines, lines);
	float2 dotSize = 1.0 / dotDim;

	float3 crt = 0;
	for (int ix = -1; ix < 2 ; ix++)
		for (int iy = -1; iy < 2; iy++)
			crt += colorDotBloom(dotDim, texCoord, float2(ix * dotSize.x, iy * dotSize.y), gamma, intensity);

	float3 mid = colorDotBloom(dotDim, texCoord, 0, gamma, intensity);
	crt = lerp(1.2 * mid, crt, 0.65);

	return float4(lerp(image, crt, vecSkill1.y), 1);
}

float4 PS_RgbBars (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 color = crtMaskSample(smpImage, float2(0, 1), texCoord);
	color = rgbRgbBars(color, texCoord * vecViewPort.xy / vecSkill5.w);

	return float4(lerp(tex2D(smpImage, texCoord), color * exp(vecSkill5.y), vecSkill1.y), 1);
}

float4 PS_MagentaGreenMask (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 color = crtMaskSample(smpImage, float2(0, 1), texCoord);
	color = rgbMgBars(color, texCoord * vecViewPort.xy / vecSkill5.w);

	return float4(lerp(tex2D(smpImage, texCoord), color * exp(vecSkill5.y), vecSkill1.y), 1);
}

float4 PS_RgbTriad (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 color = crtMaskSample(smpImage, float2(-1, 1), texCoord);
	color = rgbTriad(color, texCoord * vecViewPort.xy / vecSkill5.w);

	return float4(lerp(tex2D(smpImage, texCoord), color * exp(vecSkill5.y), vecSkill1.y), 1);
}


// subroutines

float3 colorDotBloom (float2 dots, float2 texCoord, float2 dotSize, float fGamma, float fIntensity)
{
	float3 image = tex2D(smpImage, texCoord + dotSize).rgb;
	float luma = fColorLuma(image);
	float dist = distance(frac(texCoord * dots), sign(dotSize) + 0.5);
	return image * exp(fGamma * dist * lerp(1.0 + fIntensity, 1.0 - fIntensity, luma));
}

float3 rgbRgbBars (float3 color, float2 screenCoord)
{
	float fh = nfmod(screenCoord.x, vecViewPort.x, 3);

	float h0 = smoothstep(0, 0.3, fh) - smoothstep(0.7, 1.1, fh);
	float h1 = smoothstep(0.9, 1.3, fh) - smoothstep(1.7, 2.1, fh);
	float h2 = smoothstep(1.9, 2.3, fh) - smoothstep(2.7, 3, fh);

	return color * (color * (vecSkill5.z * 0.5 + float3(h0, h1, h2)) + vecSkill5.z);
}

float3 rgbMgBars (float3 color, float2 screenCoord)
{
	float f = nfmod(screenCoord.x, vecViewPort.x, 2);

	float t0 = smoothstep(0, 0.2, f) - smoothstep(0.8, 1, f);
	float t1 = smoothstep(0.8, 1.0, f) - smoothstep(1.8, 2.0, f);

	return color * (color * (vecSkill5.z * 0.5 + float3(t0, t1, t0)) + vecSkill5.z);
}


float3 rgbTriad (float3 color, float2 screenCoord)
{
	float f = nfmod(screenCoord.x, vecViewPort.x, 2);
	f = nfmod(screenCoord.y + step(1, f) * 3, vecViewPort.x, 6.0);

	float t0 = smoothstep(0, 0.5, f) - smoothstep(1.5, 2.75, f);
	float t1 = smoothstep(1.75, 2.5, f) - smoothstep(3.5, 4.75, f);
	float t2 = smoothstep(3.75, 4.5, f) - smoothstep(5.5, 6, f);

	return color * (color * (vecSkill5.z * 0.5 + float3(t0, t1, t2)) + vecSkill5.z);
}


float3 crtMaskSample (sampler2D smp, float2 dir, float2 texCoord)
{
	float3 color = tex2D(smp, texCoord).rgb * 0.75;

	color += tex2D(smp, texCoord + vecViewPort.zw * dir) * 0.25;
	color += tex2D(smp, texCoord - vecViewPort.zw * dir) * 0.25;

	return color;
}


// techniques

technique tNone
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_none();
	}
}

technique tScanline
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_scanline();
	}
}

technique tDotNBloom
{
	pass p0
	{
		PixelShader = compile ps_3_0 PS_dotNBloom();
	}
}

technique tRgbMask
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_RgbBars();
	}
}

technique tMagentaGreenMask
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_MagentaGreenMask();
	}
}

technique tRgbtriad
{
	pass p0
	{
		PixelShader = compile ps_3_0 PS_RgbTriad();
	}
}