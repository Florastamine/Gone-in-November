struct SMapVertexToPixel
{
    float4 Position     : POSITION;    
    float3 Position2D    : TEXCOORD0;
    float2 Tex : TEXCOORD1;
};

struct SMapPixelToFrame
{
    float4 Color : COLOR0; 
};

struct SSceneVertexToPixel
{
    float4 Position             : POSITION;
    float4 ShadowMapSamplingPos : TEXCOORD0;    

     float4 RealDistance            : TEXCOORD1;
     float2 TexCoords            : TEXCOORD2;
     float3 Normal                : TEXCOORD3;
     float3 Position3D            : TEXCOORD4;

};

struct SScenePixelToFrame
{
    float4 Color : COLOR0;
};

//------- Constants --------

float4x4 matWorldViewProj;
float4x4 matMtl;
float4x4 matWorldView;
float4x4 matEffect1;
float4x4 matEffect2;
float4x4 matWorld;
float4 vecSkill1;
float4 xLightPos = {827,738,654,1};
float xLightPower = 1;
float xMaxDepth = 2800;

//------- Texture Samplers --------


 Texture entSkin1;

sampler colorSampler = sampler_state { texture = <entSkin1> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = mirror; AddressV = mirror;};

Texture xShadowMap;

sampler ShadowMapSampler = sampler_state { texture = <xShadowMap> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = clamp; AddressV = clamp;};
//------- Vertex Shaders --------

SMapVertexToPixel ShadowMapVertexShader( float4 inPos : POSITION, float2 InTex:TEXCOORD0)
{
    SMapVertexToPixel Output = (SMapVertexToPixel)0;

    //Parallel Projection
    Output.Position = mul(inPos, mul(matWorldView,matEffect1));
    Output.Position2D = mul(inPos, matWorldView);
    
    Output.Tex = InTex;
    
    //float4 position_world  = mul(position, matWorld);
    
    return Output;    
}

//------- Pixel Shaders --------


 float DotProduct(float4 LightPos, float3 Pos3D, float3 Normal)
 {
     float3 LightDir = normalize(LightPos - Pos3D);
     return dot(LightDir, Normal);
 }
 

SMapPixelToFrame ShadowMapPixelShader(SMapVertexToPixel PSIn)
{
    SMapPixelToFrame Output = (SMapPixelToFrame)0;

	Output.Color = 0;
    Output.Color.x = PSIn.Position2D.z/matEffect2[0].w;
    
    Output.Color.w = tex2D(colorSampler,PSIn.Tex).a;
  
    return Output;
}

technique ShadowMap
{
    pass Pass0
    {
    	alphablendenable = true;
    	cullmode = none;
        VertexShader = compile vs_2_0 ShadowMapVertexShader();
        PixelShader = compile ps_2_0 ShadowMapPixelShader();
    }
}
