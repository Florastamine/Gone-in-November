
//-------------------------------------------------------------------------------------------------
//Global Variables and Constants
//-------------------------------------------------------------------------------------------------
float4x4 worldMat; 
float4x4 matWorld;
float4x4 worldViewProjMat;
float4x4 matWorldViewProj;

const float specPower = 64.0f;

float4 lightPosition = {0,0,0,5000}; //varying position of Omni light

#define zOffset 0.5f 

//-------------------------------------------------------------------------------------------------
//Structures
//-------------------------------------------------------------------------------------------------

struct lightFuncOutput
{
    float4 diffuseResult;
    float4 specularResult;
};
//-------------------------------------------------------------------------------------------------
//Vertex Shader
//-------------------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 position  :  POSITION;
	float3 worldPos  :  TEXCOORD0;
	float3 normalW   :  TEXCOORD1;
	float3 cam2Vert  :  TEXCOORD2;
};

struct VS_OUTPUT_DEPTH
{
    float4 oPositionLight : POSITION;
    float3 lightVec       : TEXCOORD0;
};

VS_OUTPUT_DEPTH depthMap_VS( float4 inPosition : POSITION )
{
    VS_OUTPUT_DEPTH output;
    
    float4 positionW = mul( inPosition, matWorld );
    output.oPositionLight = mul( inPosition, matWorldViewProj );
    
    output.lightVec = lightPosition - positionW.xyz; 

    return output;
}
//-------------------------------------------------------------------------------------------------
//Pixel Shader
//-------------------------------------------------------------------------------------------------
float4 depthMap_PS( VS_OUTPUT_DEPTH In ) : COLOR0
{
	//float4 result = In.lightVec.z/1500;
    return length(In.lightVec) + zOffset;
    //return result;
    //return result;
}
//-------------------------------------------------------------------------------------------------
//Techniques
//-------------------------------------------------------------------------------------------------
//The technique which performs the first step of the algorithm
technique depthMap
{
    pass P0
    {          
        VertexShader = compile vs_2_0 depthMap_VS( );
        PixelShader  = compile ps_2_0 depthMap_PS( ); 
    }
}
