/*********************************************************************NVMH3****
File: $Id: //sw/devtools/ShaderLibrary/main/HLSL/velvety.fx#1 $

Copyright NVIDIA Corporation 2007
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
*AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF
NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

% A simple combination of vertex and pixel shaders with velvety edge effects.
% Great for any model that needs that "feeling of softness."

keywords: material organic
date: 070304

******************************************************************************/

//// UN-TWEAKABLES - AUTOMATICALLY-TRACKED TRANSFORMS ////////////////

float4x4 matWorldInv;
float4x4 matWorldViewProj;
float4x4 matWorld;
float4x4 matViewInv;

float4 vecFog;
float4 vecViewPos;

/************* TWEAKABLES **************/

float3 vecSunPos;
float3 vecLight;
float3 SpecColor = {0.7f, 0.7f, 0.75f};
float3 SubColor = {0.2f, 0.2f, 1.0f};
float RollOff = 0.3;

///

texture entSkin1;

sampler2D ColorSampler = sampler_state {
    Texture = <entSkin1>;
    MinFilter = Linear;
    MipFilter = Linear;
    MagFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};  

/************* DATA STRUCTS **************/

/* data from application vertex buffer */
struct appdata {
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
    float4 Tangent	: TEXCOORD2;
};

/* data passed from vertex shader to pixel shader */
struct vertexOutput {
    float4 HPosition	: POSITION;
    float Fog : FOG;
    float2 UV		: TEXCOORD0;
    float3 LightVec	: TEXCOORD1;
    float3 WorldNormal	: TEXCOORD2;
    float3 WorldTangent	: TEXCOORD3;
    float3 WorldBinormal : TEXCOORD4;
    float3 WorldView	: TEXCOORD5;
};

/* data passed from vertex shader to pixel shader */
struct shadedVertexOutput {
    float4 HPosition	: POSITION;
    float Fog : FOG;
    float2 UV		: TEXCOORD0;
    float4 diffCol	: COLOR0;
    float4 specCol	: COLOR1;
};

/*********** vertex shader ******/

shadedVertexOutput velvetVS(appdata IN)
{
    shadedVertexOutput OUT;
    float4x4 WorldInvTrans = transpose(matWorldInv);
    float3 Nn = normalize(mul(IN.Normal,WorldInvTrans).xyz);
    float4 Po = float4(IN.Position.xyz,1);
    OUT.HPosition = mul(Po,matWorldViewProj);
    float3 Pw = mul(Po,matWorld).xyz;
    float3 Ln = normalize(vecSunPos - Pw);
    float ldn = dot(Ln,Nn);
    float diffComp = max(0,ldn);
    float3 diffContrib = diffComp * vecLight;
    float subLamb = smoothstep(-RollOff,1.0,ldn) - smoothstep(0.0,1.0,ldn);
    subLamb = max(0.0,subLamb);
    float3 subContrib = subLamb * SubColor;
    OUT.UV = IN.UV.xy;
    float3 Vn = normalize(matViewInv[3].xyz - Pw);
    float vdn = 1.0-dot(Vn,Nn);
    float3 vecColor = vdn.xxx;
    OUT.diffCol = float4((subContrib+diffContrib).xyz,1);
    OUT.specCol = float4((vecColor*SpecColor).xyz,1);
    
    // calculate Fog
   OUT.Fog = 1 - (distance(Pw, vecViewPos) - vecFog.x) * (vecFog.z);
    
    return OUT;
}

/*********** Generic Vertex Shader ******/

vertexOutput std_VS(appdata IN)
{
    vertexOutput OUT = (vertexOutput)0;
    float4x4 WorldInvTrans = transpose(matWorldInv);
    float3 binormal = cross(IN.Normal,IN.Tangent);
    OUT.WorldNormal = mul(IN.Normal,WorldInvTrans).xyz;
    OUT.WorldTangent = mul(IN.Tangent,WorldInvTrans).xyz;
    OUT.WorldBinormal = mul(binormal,WorldInvTrans).xyz;
    float4 Po = float4(IN.Position.xyz,1);
    float3 Pw = mul(Po,matWorld).xyz;
    OUT.LightVec = (vecSunPos - Pw);
#ifdef FLIP_TEXTURE_Y
    OUT.UV = float2(IN.UV.x,(1.0-IN.UV.y));
#else /* !FLIP_TEXTURE_Y */
    OUT.UV = IN.UV.xy;
#endif /* !FLIP_TEXTURE_Y */
    OUT.WorldView = normalize(matViewInv[3].xyz - Pw);
    OUT.HPosition = mul(Po,matWorldViewProj);
    
   // calculate Fog
   OUT.Fog = 1 - (distance(Pw, vecViewPos) - vecFog.x) * (vecFog.z);
    
    return OUT;
}

/** pixel shader  **/

void velvet_shared(vertexOutput IN,
			out float3 DiffuseContrib,
			out float3 SpecularContrib)
{
    float3 Ln = normalize(IN.LightVec.xyz);
    float3 Nn = normalize(IN.WorldNormal);
    float3 Vn = normalize(IN.WorldView);
    float3 Hn = normalize(Vn + Ln);
    float ldn = dot(Ln,Nn);
    float diffComp = max(0,ldn);
    float3 diffContrib = diffComp * vecLight;
    float subLamb = smoothstep(-RollOff,1.0,ldn) - smoothstep(0.0,1.0,ldn);
    subLamb = max(0.0,subLamb);
    float3 subContrib = subLamb * SubColor;
    float vdn = 1.0-dot(Vn,Nn);
    float3 vecColor = vdn.xxx;
    DiffuseContrib = (subContrib+diffContrib).xyz;
    SpecularContrib = (vecColor*SpecColor).xyz;
}

float4 velvetPS(vertexOutput IN) : COLOR {
    float3 diffContrib;
    float3 specContrib;
	velvet_shared(IN,diffContrib,specContrib);
    float3 result = diffContrib + specContrib;
    return float4(result,1);
}

float4 velvetPS_t(vertexOutput IN) : COLOR {
    float3 diffContrib;
    float3 specContrib;
	velvet_shared(IN,diffContrib,specContrib);
    float3 map = tex2D(ColorSampler,IN.UV.xy).xyz;
    float3 result = specContrib + (map * diffContrib);
    return float4(result,1);
}

float4 velvetPS_pass(shadedVertexOutput IN) : COLOR {
    float4 result = IN.diffCol + IN.specCol;
    return float4(result.xyz,1);
}

float4 velvetPS_pass_t(shadedVertexOutput IN) : COLOR {
    float4 map = tex2D(ColorSampler,IN.UV.xy);
    float4 result = IN.specCol + (map * IN.diffCol);
    return float4(result.xyz,1);
}

technique Textured
{
	pass p0
	{
		VertexShader = compile vs_2_0 std_VS();
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		CullMode = None;
		PixelShader = compile ps_2_a velvetPS_t();
	}
}
