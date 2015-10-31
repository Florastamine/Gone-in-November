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
float4x4 matWorld;
float4x4 matEffect1;
float4 xLightPos = {-427,538,654,1};
float xLightPower = 1;
float xMaxDepth = 1000;

//------- Texture Samplers --------


 Texture xColoredTexture;

sampler ColoredTextureSampler = sampler_state { texture = <xColoredTexture> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = mirror; AddressV = mirror;};

Texture mtlSkin1;

sampler ShadowMapSampler = sampler_state { texture = <mtlSkin1> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = clamp; AddressV = clamp;};
//------- Vertex Shaders --------
 SSceneVertexToPixel ShadowedSceneVertexShader( float4 inPos : POSITION, float2 inTexCoords : TEXCOORD0, float3 inNormal : NORMAL)

{
    SSceneVertexToPixel Output = (SSceneVertexToPixel)0;
    
    Output.Position = mul(inPos, matWorldViewProj);
    //Output.ShadowMapSamplingPos = mul(mul(inPos,matWorld), matMtl);
    //Output.RealDistance = Output.ShadowMapSamplingPos.z/xMaxDepth;
    
    //Parallel Projection
    Output.ShadowMapSamplingPos = mul(mul(inPos,matWorld), mul(matMtl,matEffect1));
	 Output.RealDistance = mul(mul(inPos,matWorld), matMtl).z/xMaxDepth;
     
     Output.TexCoords = inTexCoords;
     Output.Normal = mul(inNormal, matWorld);
     Output.Position3D = inPos;

    
    return Output;    
}

//------- Pixel Shaders --------


 float DotProduct(float4 LightPos, float3 Pos3D, float3 Normal)
 {
     float3 LightDir = normalize(LightPos - Pos3D);
     return dot(LightDir, Normal);
 }

SScenePixelToFrame ShadowedScenePixelShader(SSceneVertexToPixel PSIn)
{
    SScenePixelToFrame Output = (SScenePixelToFrame)0;

    float2 ProjectedTexCoords;
    ProjectedTexCoords[0] = PSIn.ShadowMapSamplingPos.x/PSIn.ShadowMapSamplingPos.w/2.0f +0.5f;
    ProjectedTexCoords[1] = -PSIn.ShadowMapSamplingPos.y/PSIn.ShadowMapSamplingPos.w/2.0f +0.5f;


     if ((saturate(ProjectedTexCoords.x) == ProjectedTexCoords.x) && (saturate(ProjectedTexCoords.y) == ProjectedTexCoords.y))
     {    
         float StoredDepthInShadowMap = tex2D(ShadowMapSampler, ProjectedTexCoords).x;    
         if ((PSIn.RealDistance.x - 1.0f/200.0f) <= StoredDepthInShadowMap)    
         {
             float DiffuseLightingFactor = DotProduct(xLightPos, PSIn.Position3D, PSIn.Normal);
             float4 ColorComponent = 1;//tex2D(ColoredTextureSampler, PSIn.TexCoords);
             Output.Color = ColorComponent*DiffuseLightingFactor*xLightPower;
         }
     }    

	//Output.Color = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
	//Output.Color = PSIn.RealDistance.x;

    return Output;
}

//------- Techniques --------

technique ShadowedScene
{
    pass Pass0
    {
        VertexShader = compile vs_2_0 ShadowedSceneVertexShader();
        PixelShader = compile ps_2_0 ShadowedScenePixelShader();
    }
}
