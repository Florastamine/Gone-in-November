
float xMaxDepth = 10000;
float4 xLightPos = {7680,-901,4398,2000};
float xLightPower = 0;

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
Texture entSkin1;

sampler colorSampler = sampler_state { 
	texture = <entSkin1> ;
 	magfilter = LINEAR;
   minfilter = LINEAR;
   mipfilter=LINEAR;
   AddressU = clamp;
	AddressV = clamp;
};

Texture mtlSkin2;

sampler ShadowMapSampler = sampler_state { 
	texture = <mtlSkin2> ;
 	magfilter = LINEAR;
   minfilter = LINEAR;
   mipfilter=LINEAR;
   AddressU = clamp;
	AddressV = clamp;
};

Texture mtlSkin3;

sampler shadowAlphaSampler = sampler_state { 
	texture = <mtlSkin3> ;
 	magfilter = LINEAR;
   minfilter = LINEAR;
   mipfilter=LINEAR;
   AddressU = clamp;
	AddressV = clamp;
};


float4x4 matWorldViewProj;
float4x4 matWorld;
float4x4 matEffect1;
float4x4 matMtl;

/*
SSceneVertexToPixel ShadowedSceneVertexShader( float4 inPos : POSITION)
{
    SSceneVertexToPixel Output = (SSceneVertexToPixel)0;
    
    Output.Position = mul(inPos, matWorldViewProj);
    Output.ShadowMapSamplingPos = mul(inPos, matMtl);
    return Output;    
}

SScenePixelToFrame ShadowedScenePixelShader(SSceneVertexToPixel PSIn)
{
    SScenePixelToFrame Output = (SScenePixelToFrame)0;

    float2 ProjectedTexCoords;
    ProjectedTexCoords[0] = PSIn.ShadowMapSamplingPos.x/PSIn.ShadowMapSamplingPos.w/2.0f +0.5f;
    ProjectedTexCoords[1] = -PSIn.ShadowMapSamplingPos.y/PSIn.ShadowMapSamplingPos.w/2.0f +0.5f;
    Output.Color = tex2D(ShadowMapSampler, ProjectedTexCoords);

    return Output;
}
*/
SSceneVertexToPixel ShadowedSceneVertexShader( float4 inPos : POSITION, float2 inTexCoords : TEXCOORD0, float3 inNormal : NORMAL)

{
    SSceneVertexToPixel Output = (SSceneVertexToPixel)0;
    
    Output.Position = mul(inPos, matWorldViewProj);
    Output.ShadowMapSamplingPos = mul(inPos, matMtl);

     Output.RealDistance = Output.ShadowMapSamplingPos.z/xMaxDepth;
     Output.TexCoords = inTexCoords;
     Output.Normal = mul(inNormal, matWorld);
     Output.Position3D = inPos;

    
    return Output;    
}

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
    //Output.Color = tex2D(ShadowMapSampler, ProjectedTexCoords);
    Output.Color = 1;//tex2D(ShadowMapSampler, ProjectedTexCoords).r + tex2D(ShadowMapSampler, ProjectedTexCoords).g/127;
    Output.Color.a = tex2D(colorSampler, PSIn.TexCoords).a;

	if ((saturate(ProjectedTexCoords.x) == ProjectedTexCoords.x) && (saturate(ProjectedTexCoords.y) == ProjectedTexCoords.y))
     {    
         //float StoredDepthInShadowMap = tex2D(ShadowMapSampler, ProjectedTexCoords).x;    
         float StoredDepthInShadowMap = tex2D(ShadowMapSampler, ProjectedTexCoords).r + tex2D(ShadowMapSampler, ProjectedTexCoords).g/127;    
         if ((PSIn.RealDistance.x - 1.0f/100.0f) <= StoredDepthInShadowMap)    
         {
         	float LightTextureFactor = tex2D(shadowAlphaSampler, ProjectedTexCoords).r;
             float DiffuseLightingFactor = DotProduct(xLightPos, PSIn.Position3D, PSIn.Normal);
             float4 ColorComponent = 1;//tex2D(ColoredTextureSampler, PSIn.TexCoords);
             Output.Color = LightTextureFactor;//LightTextureFactor;//1-(ColorComponent*DiffuseLightingFactor*xLightPower);
         }
     }
	Output.Color = 1-Output.Color;
    return Output;
}


technique ShadowedScene
{
    pass Pass0
    {
        VertexShader = compile vs_2_0 ShadowedSceneVertexShader();
        PixelShader = compile ps_2_0 ShadowedScenePixelShader();
    }
}


/*
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
    float4 ShadowMapSamplingPos : TEXCOORD2;    

     float4 RealDistance            : TEXCOORD1;
     float2 TexCoords            : TEXCOORD0;
     float3 Normal                : TEXCOORD3;
     float3 Position3D            : TEXCOORD4;

};

struct SScenePixelToFrame
{
    float4 Color : COLOR0;
};

//------- Constants --------

matrix matWorldViewProj;
float4x4 xLightWorldViewProjection;
matrix matEffect1;
matrix matWorldView;
matrix matWorld;
float4 xLightPos = {5469,-213,2820,5000};
float xLightPower = 1;
float xMaxDepth = 5000;

//------- Texture Samplers --------


 Texture xColoredTexture;
 Texture mtlSkin1;

sampler ColoredTextureSampler = sampler_state { texture = <mtlSkin1> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = mirror; AddressV = mirror;};

Texture xShadowMap;
Texture TargetMap;
Texture mtlSkin2;

sampler ShadowMapSampler = sampler_state { texture = <mtlSkin2> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = clamp; AddressV = clamp;};
//------- Vertex Shaders --------

 SSceneVertexToPixel ShadowedSceneVertexShader( float4 inPos : POSITION, float2 inTexCoords : TEXCOORD0, float3 inNormal : NORMAL)

{
    SSceneVertexToPixel Output = (SSceneVertexToPixel)0;
    
    Output.Position = mul(inPos, matWorldViewProj);
    Output.ShadowMapSamplingPos = mul(inPos, matEffect1);

     Output.RealDistance = Output.ShadowMapSamplingPos.z/xMaxDepth;
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
         if ((PSIn.RealDistance.x - 1.0f/100.0f) <= StoredDepthInShadowMap)    
         {
             float DiffuseLightingFactor = DotProduct(xLightPos, PSIn.Position3D, PSIn.Normal);
             float4 ColorComponent = 1;//tex2D(ColoredTextureSampler, PSIn.TexCoords);
             Output.Color = ColorComponent*DiffuseLightingFactor*xLightPower;
         }
     }    

	//Output.Color = tex2D(ShadowMapSampler, ProjectedTexCoords);
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
*/
