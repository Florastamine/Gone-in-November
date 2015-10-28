struct SMapVertexToPixel
{
    float4 Position     : POSITION;    
    float3 Position2D    : TEXCOORD0;
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
float4x4 matWorld;
float4 xLightPos = {-427,538,654,1};
float xLightPower = 1;
float xMaxDepth = 1000;

//------- Texture Samplers --------


 Texture xColoredTexture;

sampler ColoredTextureSampler = sampler_state { texture = <xColoredTexture> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = mirror; AddressV = mirror;};

Texture xShadowMap;

sampler ShadowMapSampler = sampler_state { texture = <xShadowMap> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = clamp; AddressV = clamp;};
//------- Vertex Shaders --------

SMapVertexToPixel ShadowMapVertexShader( float4 inPos : POSITION)
{
    SMapVertexToPixel Output = (SMapVertexToPixel)0;
    
    //Output.Position = mul(inPos, matWorldViewProj);
    //Output.Position2D = Output.Position;
    
    //Parallel Projection
    Output.Position = mul(inPos, mul(matWorldView,matEffect1));
    Output.Position2D = mul(inPos, matWorldView);
    
    
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

    Output.Color = PSIn.Position2D.z/xMaxDepth;

    return Output;
}

//------- Techniques --------


technique ShadowMap
{
    pass Pass0
    {
        VertexShader = compile vs_2_0 ShadowMapVertexShader();
        PixelShader = compile ps_2_0 ShadowMapPixelShader();
    }
}