/*********************************************************************NVMH3****
Path:  NVSDK\Common\media\cgfx
File:  ocean.fx

Copyright NVIDIA Corporation 2003
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
*AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


Comments:
	Simple ocean shader with animated bump map and geometric waves
	Based partly on "Effective Water Simulation From Physical Models", GPU Gems

******************************************************************************/

float bumpHeight = 1.0;
float2 textureScale = { 10.0, 10.0 };
float2 bumpSpeed = { 0.0, 0.005 };
float fresnelBias = 0.1;
float fresnelPower = 4.0;
float hdrMultiplier = 0.5;
float4 deepColor = {0.0f, 0.0f, 0.1f, 1.0f};
float4 shallowColor = {0.0f, 0.5f, 0.5f, 1.0f};
float4 reflectionColor = {1.0f, 1.0f, 1.0f, 1.0f};

float reflectionAmount = 1.0f;
float waterAmount = 1.0f;

float waveSpeed = 0.3;
float waveAmp = 2.0f;
float waveFreq = 0.1;

float4x4 matWorldViewProj;
float4x4 matWorld;
float4x4 matViewInv;

float4 vecViewPos;
float4 vecTime;

texture mtlSkin1;
texture mtlSkin2;

sampler2D normalMapSampler = sampler_state
{
	Texture = (mtlSkin1);
	MagFilter = Linear;	
	MinFilter = Point;
	MipFilter = None;
	AddressU = Wrap;
   AddressV = Wrap;
};

samplerCUBE envMapSampler = sampler_state
{
	Texture = (mtlSkin2);
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
   AddressU = Clamp;
   AddressV = Clamp;
};


struct a2v
{
	float4 Position : POSITION;   // in object space
	float2 TexCoord : TEXCOORD0;
	float3 Normal   : NORMAL;
};

struct v2f
{
	float4 Position  : POSITION;  // in clip space
	float2 TexCoord  : TEXCOORD0;
	float3 TexCoord1 : TEXCOORD1; // first row of the 3x3 transform from tangent to cube space
	float3 TexCoord2 : TEXCOORD2; // second row of the 3x3 transform from tangent to cube space
	float3 TexCoord3 : TEXCOORD3; // third row of the 3x3 transform from tangent to cube space

	float2 bumpCoord0 : TEXCOORD4;
	float2 bumpCoord1 : TEXCOORD5;
	float2 bumpCoord2 : TEXCOORD6;
	
	float3 eyeVector  : TEXCOORD7;
};


// wave functions
struct Wave
{
  float freq;  // 2*PI / wavelength
  float amp;   // amplitude
  float phase; // speed * 2*PI / wavelength
  float2 dir;
};

#define NWAVES 2
Wave wave[NWAVES] = {
	{ 1.0, 1.0, 0.5, float2(-1, 0) },
	{ 2.0, 0.5, 1.3, float2(-0.7, 0.7) }
};

float evaluateWave(Wave w, float2 pos, float t)
{
  return w.amp * sin( dot(w.dir, pos)*w.freq + t*w.phase);
}

// derivative of wave function
float evaluateWaveDeriv(Wave w, float2 pos, float t)
{
  return w.freq*w.amp * cos( dot(w.dir, pos)*w.freq + t*w.phase);
}

// sharp wave functions
float evaluateWaveSharp(Wave w, float2 pos, float t, float k)
{
  return w.amp * pow(sin( dot(w.dir, pos)*w.freq + t*w.phase)* 0.5 + 0.5 , k);
}

float evaluateWaveDerivSharp(Wave w, float2 pos, float t, float k)
{
  return k*w.freq*w.amp * pow(sin( dot(w.dir, pos)*w.freq + t*w.phase)* 0.5 + 0.5 , k - 1) * cos( dot(w.dir, pos)*w.freq + t*w.phase);
}

v2f BumpReflectWaveVS(a2v IN)
{
	v2f OUT;
	
	wave[0].freq = waveFreq;
	wave[0].amp = waveAmp;

	wave[1].freq = waveFreq*2.0;
	wave[1].amp = waveAmp*0.5;

	float4 P = IN.Position;

	// sum waves	
	P.y = 0.0;
	float ddx = 0.0, ddy = 0.0;
	for(int i = 0; i < NWAVES; i++)
	{
		P.y += evaluateWave(wave[i], P.xz, vecTime.w*waveSpeed);
		float deriv = evaluateWaveDeriv(wave[i], P.xz, vecTime.w*waveSpeed);
		ddx += deriv * wave[i].dir.x;
		ddy += deriv * wave[i].dir.y;
	}

	// compute tangent basis
	float3 B = float3(1, ddx, 0);
	float3 T = float3(0, ddy, 1);
	float3 N = float3(-ddx, 1, -ddy);

	OUT.Position = mul(P, matWorldViewProj);
	
	// pass texture coordinates for fetching the normal map
	OUT.TexCoord.xy = IN.TexCoord*textureScale;

	OUT.bumpCoord0.xy = IN.TexCoord*textureScale + vecTime.w*bumpSpeed*0.1;
	OUT.bumpCoord1.xy = IN.TexCoord*textureScale*2.0 + vecTime.w*bumpSpeed*0.1*4.0;
	OUT.bumpCoord2.xy = IN.TexCoord*textureScale*4.0 + vecTime.w*bumpSpeed*0.1*8.0;

	// compute the 3x3 tranform from tangent space to object space
	float3x3 objToTangentSpace;
	
	// first rows are the tangent and binormal scaled by the bump scale
	objToTangentSpace[0] = bumpHeight * normalize(T);
	objToTangentSpace[1] = bumpHeight * normalize(B);
	objToTangentSpace[2] = normalize(N);

	OUT.TexCoord1.xyz = mul(objToTangentSpace, matWorld[0].xyz);
	OUT.TexCoord2.xyz = mul(objToTangentSpace, matWorld[1].xyz);
	OUT.TexCoord3.xyz = mul(objToTangentSpace, matWorld[2].xyz);

	// compute the eye vector (going from shaded point to eye) in cube space
	float4 worldPos = mul(P, matWorld);
	OUT.eyeVector = matViewInv[3] - worldPos; // view inv. transpose contains eye position in world space in last row
	return OUT;
}


// Pixel Shader
float4 OceanPS20(v2f IN) : COLOR
{
	// sum normal maps
	half4 t0 = tex2D(normalMapSampler, IN.bumpCoord0.xy)*2.0-1.0;
	half4 t1 = tex2D(normalMapSampler, IN.bumpCoord1.xy)*2.0-1.0;
	half4 t2 = tex2D(normalMapSampler, IN.bumpCoord2.xy)*2.0-1.0;
	half3 N = t0.xyz + t1.xyz + t2.xyz;

	half3x3 m; // tangent to world matrix
	m[0] = IN.TexCoord1;
	m[1] = IN.TexCoord2;
	m[2] = IN.TexCoord3;
	half3 Nw = mul(m, N.xyz);
	Nw = normalize(Nw);

	// reflection
	float3 E = normalize(IN.eyeVector);
	half3 R = reflect(-E, Nw);

	half4 reflection = texCUBE(envMapSampler, R);
	
	// hdr effect (multiplier in alpha channel)
	reflection.rgb *= (1.0 + reflection.a*hdrMultiplier);

	// fresnel - could use 1D tex lookup for this
	half facing = 1.0 - max(dot(E, Nw), 0);
	half fresnel = fresnelBias + (1.0-fresnelBias)*pow(facing, fresnelPower);

	half4 waterColor = lerp(deepColor, shallowColor, facing);

	return waterColor*waterAmount + reflection*reflectionColor*reflectionAmount*fresnel;
}

technique PS20
{
	pass p0
	{
		Zenable = true;
		ZWriteEnable = true;
		CullMode = None;

		VertexShader = compile vs_2_0 BumpReflectWaveVS();
  		PixelShader = compile ps_2_0 OceanPS20();
	}
}
