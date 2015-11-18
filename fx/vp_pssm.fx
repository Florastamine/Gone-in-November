//////////////////////////////////////////////////////////////////////
// PSSM shadow mapping
// (c) oP group 2010  Version 1.1
//////////////////////////////////////////////////////////////////////
//#define BONES // activate GPU bones (reduces the frame rate by 5%)
//#define FAST

#ifdef BONES
#include <bones>
#endif
#include <pos>

const bool AUTORELOAD;

float4x4 matViewProj;
float4x4 matView;
float4x4 matTex[4]; // set up from the pssm script

#define PCFSAMPLES_NEAR	5	// higher values for smoother shadows (slower)
#define PCFSAMPLES_FAR	3

float pssm_splitdist_var[5];
float pssm_numsplits_var = 3;
float pssm_res_var = 1024;
float pssm_transparency_var = 0.7;
float d3d_alpharef_var;

texture entSkin1;
texture mtlSkin1;
texture mtlSkin2;
texture mtlSkin3;
texture mtlSkin4;

sampler sBaseTex = sampler_state { Texture = <entSkin1>; MipFilter = linear; };

sampler sDepth1 = sampler_state {
	texture = <mtlSkin1>;
	MinFilter = point; MagFilter = point; MipFilter = none;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};
sampler sDepth2 = sampler_state {
	texture = <mtlSkin2>;
	MinFilter = point; MagFilter = point; MipFilter = none;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};
sampler sDepth3 = sampler_state {
	texture = <mtlSkin3>;
	MinFilter = point; MagFilter = point; MipFilter = none;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};
sampler sDepth4 = sampler_state {
	texture = <mtlSkin4>;
	MinFilter = point; MagFilter = point; MipFilter = none;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};
//////////////////////////////////////////////////////////////////

// PCF soft-shadowing
float DoPCF(sampler2D sMap, float4 vShadowTexCoord, int iSqrtSamples)
{
	float fShadowTerm = 0.0f;
	float fRadius = (iSqrtSamples - 1.0f) / 2;
	
	for(float y = -fRadius; y <= fRadius; y++)
	{
		for(float x = -fRadius; x <= fRadius; x++)
		{
			float2 vOffset = float2(x,y)/pssm_res_var;
			float fDepth = tex2D(sMap, vShadowTexCoord.xy + vOffset).x;
			float fSample = (vShadowTexCoord.z < fDepth);
			
			// Edge tap smoothing
			float xWeight = 1, yWeight = 1;
			if(x == -fRadius)
				xWeight = 1 - frac(vShadowTexCoord.x * pssm_res_var);
			else if(x == fRadius)
				xWeight = frac(vShadowTexCoord.x * pssm_res_var);
			
			if(y == -fRadius)
				yWeight = 1 - frac(vShadowTexCoord.y * pssm_res_var);
			else if(y == fRadius)
				yWeight = frac(vShadowTexCoord.y * pssm_res_var);
			
			fShadowTerm += fSample * xWeight * yWeight;
		}
	}
	
	return fShadowTerm / (iSqrtSamples * iSqrtSamples);
}

// Calculates the shadow occlusion using bilinear PCF
float DoFastPCF(sampler2D sMap, float4 vShadowTexCoord)
{
	// transform to texel space
	float2 vShadowMapCoord = pssm_res_var * vShadowTexCoord.xy;
	
	// read in bilerp stamp, doing the shadow checks
	float4 fSamples;
	float pixelSize = 1.0/pssm_res_var;
	
	fSamples.x = (tex2Dlod(sMap, float4(vShadowTexCoord.xy, 0, 0)).x < vShadowTexCoord.z) ? 0.0f: 1.0f;  
	fSamples.y = (tex2Dlod(sMap, float4(vShadowTexCoord.xy + float2(pixelSize, 0), 0, 0)).x < vShadowTexCoord.z) ? 0.0f: 1.0f;  
	fSamples.z = (tex2Dlod(sMap, float4(vShadowTexCoord.xy + float2(0, pixelSize), 0, 0)).x < vShadowTexCoord.z) ? 0.0f: 1.0f;  
	fSamples.w = (tex2Dlod(sMap, float4(vShadowTexCoord.xy + float2(pixelSize, pixelSize), 0, 0)).x < vShadowTexCoord.z) ? 0.0f: 1.0f;  
	
	float4 vLerps;
	vLerps.xy = frac(vShadowMapCoord.xy);
	vLerps.zw = 1-vLerps.xy;
	vLerps = vLerps.zxzx*vLerps.wwyy;
	
	return dot(fSamples, vLerps);                                
}

void renderShadows_VS(
  in float4 inPos: POSITION,
  in float2 inTex: TEXCOORD0,
#ifdef BONES
	in int4 inBoneIndices: BLENDINDICES,
	in float4 inBoneWeights: BLENDWEIGHT,
#endif	
  out float4 outPos: POSITION,
  out float4 TexCoord[4]: TEXCOORD,
  out float2 outTex: TEXCOORD4,
  out float fDistance: TEXCOORD5
  )
{
// calculate world position
#ifdef BONES
  float4 PosWorld = DoPos(DoBones(inPos, inBoneIndices, inBoneWeights));
#else  
  float4 PosWorld = DoPos(inPos);
#endif  
  outPos = mul(PosWorld, matViewProj);
// store view space position
  fDistance = mul(PosWorld, matView).z;
  outTex = inTex;
// coordinates for shadow maps
  for(int i = 0; i < pssm_numsplits_var; i++)
    TexCoord[i] = mul(PosWorld, matTex[i]);
}


float4 renderShadows_PS(
  float4 TexCoord[4] : TEXCOORD,
  float2 inTex: TEXCOORD4,
  float fDistance: TEXCOORD5
  ) : COLOR
{
	float fShadow;
	float2 shifted[4];
	shifted[0] = TexCoord[0].xy*2.0-1.0;
	shifted[1] = TexCoord[1].xy*2.0-1.0;
	shifted[2] = TexCoord[2].xy*2.0-1.0;
	shifted[3] = TexCoord[3].xy*2.0-1.0;
	float4 inSplit = float4(dot(shifted[0], shifted[0]), dot(shifted[1], shifted[1]), dot(shifted[2], shifted[2]), dot(shifted[3], shifted[3]));
	inSplit = (inSplit < 1.0);
	
#ifdef FAST
  if(inSplit.x > 0.5)
 	  fShadow = DoFastPCF(sDepth1, TexCoord[0]);
  else if(inSplit.y > 0.5)
 	  fShadow = DoFastPCF(sDepth2, TexCoord[1]);
  else if(inSplit.z > 0.5)
 	  fShadow = DoFastPCF(sDepth3, TexCoord[2]);
  else
 	  fShadow = DoFastPCF(sDepth4, TexCoord[3]);
#else
  if(inSplit.x > 0.5)
 	  fShadow = DoPCF(sDepth1, TexCoord[0], PCFSAMPLES_NEAR);
  else if(inSplit.y > 0.5)
 	  fShadow = DoPCF(sDepth2, TexCoord[1], PCFSAMPLES_FAR);
  else if(inSplit.z > 0.5)
 	  fShadow = DoPCF(sDepth3, TexCoord[2], PCFSAMPLES_FAR);
  else
 	  fShadow = DoPCF(sDepth4, TexCoord[3], PCFSAMPLES_FAR);
#endif

	float alpha = tex2Dlod(sBaseTex, float4(inTex.xy, 0.0f, 0.0f)).a * pssm_transparency_var;
	clip(alpha - d3d_alpharef_var/255.f); // for alpha transparent textures
	return float4(0, 0, 0, clamp(1 - 2.5*fShadow, 0, alpha));
}

technique renderShadows
{
  pass p0
  {
		ZWriteEnable = True;
		AlphaBlendEnable = False;

		VertexShader = compile vs_3_0 renderShadows_VS();
		PixelShader = compile ps_3_0 renderShadows_PS();
  }
}
