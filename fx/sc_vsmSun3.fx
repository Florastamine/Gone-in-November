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
     float3 WPos            : TEXCOORD5;
};

struct SScenePixelToFrame
{
    float4 Color : COLOR0;
};

//------- Constants --------

float4x4 matWorldViewProj;
float4x4 matMtl;
float4x4 matWorld;
float4x4 matWorldInv;
float4x4 matEffect1;
float4 xLightPos = {-427,538,654,1};
float xLightPower = 1;
float xMaxDepth = 800;
float light_vsm_epsilon = 0.0002;
float light_shadow_bias = 0.0025;

//------- Texture Samplers --------


 Texture xColoredTexture;

sampler ColoredTextureSampler = sampler_state { texture = <xColoredTexture> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = mirror; AddressV = mirror;};

Texture mtlSkin1;

sampler ShadowMapSampler = sampler_state
{
	texture = <mtlSkin1>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter=LINEAR;
	AddressU = clamp;
	AddressV = clamp;
};

Texture mtlSkin2;

sampler ShadowAlphaSampler = sampler_state
{
	texture = <mtlSkin2>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter=LINEAR;
	AddressU = clamp;
	AddressV = clamp;
};

texture entSkin3;

//NormalMap
sampler normalSampler = sampler_state 
{ 
   Texture = <entSkin3>;
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

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
	Output.WPos = mul(inPos,matWorld);
   
    return Output;    
}

//------- Pixel Shaders --------


 float DotProduct(float4 LightPos, float3 Pos3D, float3 Normal)
 {
     float3 LightDir = normalize(LightPos - Pos3D);
     return dot(LightDir, Normal);
 }

float doVSM(float2 depthmap, float lightDepth)
{
     	//depthmap.y = (depthmap.x * depthmap.x);
     	float lit_factor = (lightDepth <= depthmap.x);
    	
    	// Variance shadow mapping
    	float E_x2 = depthmap.y;
    	float Ex_2 = depthmap.x * depthmap.x;
    	float variance = min(max(E_x2 - Ex_2, 0.0) + light_vsm_epsilon, 1.0);
    	float m_d = (depthmap.x - lightDepth);
    	float p = variance / (variance + m_d * m_d);
    	
    	// Adjust the light color based on the shadow attenuation
    	return 1-max(lit_factor, p);
}

SScenePixelToFrame ShadowedScenePixelShader(SSceneVertexToPixel PSIn)
{
    SScenePixelToFrame Output = (SScenePixelToFrame)0;

    float2 ProjectedTexCoords;
    ProjectedTexCoords[0] = PSIn.ShadowMapSamplingPos.x/PSIn.ShadowMapSamplingPos.w/2.0f +0.5f;
    ProjectedTexCoords[1] = -PSIn.ShadowMapSamplingPos.y/PSIn.ShadowMapSamplingPos.w/2.0f +0.5f;

/*
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
*/
	//Output.Color = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
	//Output.Color = PSIn.RealDistance.x;


	float3 dir_to_light = xLightPos.xyz - PSIn.WPos;
   float dist_to_light = length(dir_to_light);
   float lightDist = (dist_to_light / xMaxDepth) - light_shadow_bias;

	float blurStr = 0.0015;
	float shadowDepth = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
	
	
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr,0)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr,0)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(0,blurStr)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(0,-blurStr)).x;
	
	
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr/2,blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr/2,-blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr/2,blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr/2,-blurStr/2)).x;
	
	
	shadowDepth /= 9;
	//shadowDepth.y = shadowDepth.x*shadowDepth.x;
	
	
	float shadow1 = doVSM(float2(shadowDepth,shadowDepth*shadowDepth),PSIn.RealDistance.x);
   shadow1 = (1-shadow1);
    
    float lighting = DotProduct(xLightPos, PSIn.Position3D, PSIn.Normal);
    Output.Color = //(shadow1 + ((1-shadow1)*0.3))
    					 //*(lighting + ((1-lighting)*0.3))
    					 (clamp(shadow1*lighting,0,1) + ((1-clamp(shadow1*lighting,0,1))*0.25))
    					 //(shadow1 + ((1-shadow1)*0.3))
    					 *(1-tex2D(ShadowAlphaSampler, ProjectedTexCoords));
    Output.Color += tex2D(ShadowAlphaSampler, ProjectedTexCoords);
    
    //Output.Color = tex2D(ShadowAlphaSampler, ProjectedTexCoords);
    
    return Output;
}

//------- Techniques --------

technique ShadowedScene
{
    pass Pass0
    {
    	cullmode = CCW;
        VertexShader = compile vs_2_0 ShadowedSceneVertexShader();
        PixelShader = compile ps_2_0 ShadowedScenePixelShader();
    }
}
