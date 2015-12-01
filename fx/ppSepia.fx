/**
 * Sepia postprocessing shader v0.2 (2012-01-06)
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
 
float fRgbDot (float3 color, float c1, float c2, float c3)
{
	return dot(color, float3(c1, c2, c3));
}
 
float fColorLuma (float3 color)
{
	return fRgbDot(color, 0.299, 0.587, 0.114);
}
 
float3 flt3 (float f)
{
	return float3(f, f, f);
}

float3 lumaExposure (float3 color, float fLumaMin, float fLumaMax, float fDimming, float fBrightening)
{
	float l = fColorLuma(color);
	
	float i = smoothstep(fLumaMin, fLumaMax, l);
	
	float3 s = color - i * fDimming * (l * color);
	s += (1-i) * fBrightening * ((1-l) * color);
	
	return s;
}

float3 rgb2yiq (float3 rgb)
{
	static const
	float3x3 m = { {0.299,  0.587,  0.114},
	               {0.596, -0.274, -0.321},
	               {0.211, -0.523,  0.311}  };

	return mul(m, rgb);
}

float3 yiq2rgb (float3 yiq)
{
	static const
	float3x3 m = { {1,  0.956,  0.621},
	               {1, -0.272, -0.647},
	               {1, -1.107,  1.705}  };

	return mul(m, yiq);
}

texture TargetMap;
sampler2D smpImage = sampler_state
{
	Texture = <TargetMap>;
};

float4 vecAmbient; // tint
float4 vecDiffuse; // duo from
float4 vecSpecular; // duo to

float4 vecSkill1; // .y = alpha, .zw = luma min/max
float4 vecSkill5; // .x = dimming, .y = brightening, .zw = yiq I & Q

 
float4 PS_none (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	return float4(image, 1);
}

float4 PS_add (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;

	float l = fColorLuma(image);
	
	float3 s = flt3(l) + vecAmbient.rgb;
	s = lumaExposure(s, vecSkill1.z, vecSkill1.w, vecSkill5.x, vecSkill5.y);
	
	return float4(lerp(image, s, vecSkill1.y), 1);
}

float4 PS_matrix (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;

	static const
	float3x3 matSepia = { {0.393, 0.769, 0.189},
						  {0.349, 0.686, 0.168},
						  {0.272, 0.534, 0.131} };	
	
	float3 s = mul(matSepia, image);
	s = lumaExposure(s, vecSkill1.z, vecSkill1.w, vecSkill5.x, vecSkill5.y);
	
	return float4(lerp(image, s, vecSkill1.y), 1);
}

float4 PS_yiq (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;

	float3 yiq = rgb2yiq(image);
	
	yiq.yz = vecSkill5.zw; // set I and Q
	
	float3 s = yiq2rgb(yiq);
	s = lumaExposure(s, vecSkill1.z, vecSkill1.w, vecSkill5.x, vecSkill5.y);
	
	return float4(lerp(image, s, vecSkill1.y), 1);
}

float4 PS_duo (float2 texCoord : TEXCOORD0) : COLOR0
{
	float3 image = tex2D(smpImage, texCoord).rgb;
	
	float l = fColorLuma(image);
	
	float3 s = lerp(vecDiffuse.rgb, vecSpecular.rgb, l);	
	s = lumaExposure(s, vecSkill1.z, vecSkill1.w, vecSkill5.x, vecSkill5.y);
	
	return float4(lerp(image, s, vecSkill1.y), 1);
}

technique tNone
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_none();
	}
}

technique tTint
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_add();
	}
}

technique tMatrix
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_matrix();
	}
}

technique tYiq
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_yiq();
	}
}

technique tDuo
{
	pass p0
	{
		PixelShader = compile ps_2_0 PS_duo();
	}
}