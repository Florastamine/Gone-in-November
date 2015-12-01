/**
 * Film grain postprocessing shader v0.1 (2013-01-06)
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
 
float4 vecTime; // .x = time_step, .w = total ticks
float4 vecViewPort; // .xy = size xy, .zw = 1/xy

float4 vecSkill1; // .y = alpha
float4 vecSkill5; // .xy = norm xy, .zw = scale xy
float4 vecSkill9; // .xy = rand xy, .zw = size xy
float4 vecSkill13; // .z = brightness
float4 vecSkill17; // sequence mask
 
// The following blending mode functions were taken from the macros of Romain Dura's
// "Photoshop & misc math"-GLSL file, which he posted on his website. The same license
// applies as can be seen in his GLSL file. See: http://mouaif.wordpress.com/?p=94

float fBlendOverlay (float fBase, float fBlend)
{
	if (fBase < 0.5)
		return 2.0 * fBase * fBlend;
	else
		return clamp(1.0 - 2.0 * (1.0 - fBase) * (1.0 - fBlend), 0, 1);
}

float3 colorBlendOverlay (float3 base, float3 blend)
{
	return float3(fBlendOverlay(base.r, blend.r),
	              fBlendOverlay(base.g, blend.g),
	              fBlendOverlay(base.b, blend.b));
}

float3 flt3 (float f)
{
	return float3(f, f, f);
}

float random (float2 n)
{
	return 0.5 + 0.5 * frac(sin(dot(n.xy, float2(vecTime.w * 12.9898, 72.233))) * 43758.5453);
}

texture TargetMap;
sampler2D smpImage = sampler_state
{
	Texture = <TargetMap>;
};

texture mtlSkin1;
sampler2D smpGrain = sampler_state
{
	Texture = <mtlSkin1>;
};

float4 PS_none (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	return float4(image, 1);
}

float4 PS_tile (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	
	float2 grainCoord = (((texCoord * vecViewPort.xy) + vecSkill9.xy) / vecSkill9.zw) * vecSkill5.zw;
	float3 grain = tex2D(smpGrain, grainCoord).rgb + vecSkill13.z;
	
	float3 g = colorBlendOverlay(image, grain);
	
	return float4(lerp(image, g, vecSkill1.y), 1);
}

float4 PS_seq (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	
	float2 grainCoord = (((texCoord * vecViewPort.xy) + vecSkill9.xy) / vecSkill9.zw) * vecSkill5.zw;
	float grain = dot(tex2D(smpGrain, grainCoord).rgba, vecSkill17.xyzw) + vecSkill13.z;
	
	float3 g = colorBlendOverlay(image, flt3(grain));
	
	return float4(lerp(image, g, vecSkill1.y), 1);
}

float4 PS_full (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	
	float2 norm = vecSkill5.xy;
	float2 scale = vecSkill5.zw;
	
	float2 randCoord = frac(texCoord.xy + vecViewPort.xy / vecSkill9.xy);

	float2 sampleCoords = (randCoord - 0.5) * norm;
	
	// make coordinate distortion free
	sampleCoords *= scale;
	
	// retransform coordinate so that image fits screen
	sampleCoords += 0.5 * norm;
	
	sampleCoords = min(max(vecViewPort.zw, frac(sampleCoords)), 1-vecViewPort.zw);
	
	// sample pixel and return
	float3 grain = tex2D(smpGrain, sampleCoords).rgb + vecSkill13.z;
	
	float3 g = colorBlendOverlay(image, grain);
	return float4(lerp(image, g, vecSkill1.y), 1);
}

float4 PS_stream (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	
	float2 norm = vecSkill5.xy;
	float2 scale = vecSkill5.zw;
	
	float2 sampleCoords = (texCoord.xy - 0.5) * norm;
	
	// make coordinate distortion free
	sampleCoords *= scale;
	
	// retransform coordinate so that image fits screen
	sampleCoords += 0.5 * norm;
	
	sampleCoords = min(max(vecViewPort.zw, frac(sampleCoords)), 1-vecViewPort.zw);
	
	// sample pixel and return
	float3 grain = tex2D(smpGrain, sampleCoords).rgb + vecSkill13.z;
	
	float3 g = colorBlendOverlay(image, grain);
	return float4(lerp(image, g, vecSkill1.y), 1);
}

float4 PS_random (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	
	float r = 0.5 + (0.5 - random(texCoord)) * 0.333;
	float3 grain = flt3(r) + vecSkill13.z;
	
	float3 g = colorBlendOverlay(image, grain);
	return float4(lerp(image, g, vecSkill1.y), 1);
}

technique tNone
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_none();
	}
}

technique tTile
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_tile();
	}
}

technique tSeq
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_seq();
	}
}

technique tFull
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_full();
	}	
}

technique tStream
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_stream();
	}	
}

technique tRandom
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_random();
	}	
}