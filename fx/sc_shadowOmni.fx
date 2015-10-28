//our world view projection matrix
matrix matWorldViewProj;
//our world matrix
matrix matWorld;
//float4 ViewPos : VIEWPOS = {0,0,0,0}; //our View Position
float4 vecViewPos;
float4 lightPos : LIGHTPOINT0_POSITION = {0,0,0,50000}; //our light position in object space
float lightrange = 50000; //Light Range in Object Space
float pStrength = 0.01f; //Parallax strength.
float SpecMP = 0.3f; //Specular Multipler
float LightColor : LIGHTPOINT0_COLOR = 1; //Light color. :)

//matrix matMtl;
float4 vecSkill1;

#define BIAS 0.9 //just leave to .98

//Load Textures
texture DiffMap; // Our diffuse Map.
texture entSkin1;
texture NormMap; // Our Normal Map.
texture entSkin3;
texture SpecMap; // Our Specular Map.
textureCUBE mtlSkin1;
textureCUBE cubeShadowMap; // Our Shadow Map. 

sampler2D Diffuse0 : TEXUNIT0 = sampler_state
{
     Texture = (entSkin1);
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
};
sampler2D Normal1 : TEXUNIT1 = sampler_state
{
    Texture = (entSkin3);
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};
sampler2D SpecSample : TEXUNIT2 = sampler_state
{
    Texture = (entSkin1);
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};

samplerCUBE cubeShadowMapSampler = sampler_state
{
	Texture = <mtlSkin1>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//application to vertex structure
struct a2v
{
     float4 position : POSITION0;
     float3 normal : NORMAL;
     float2 tex0 : TEXCOORD0;
     float3 tangent : TANGENT;
     float3 binormal : BINORMAL;
};

//vertex to pixel shader structure
struct v2p
{
     float4 position : POSITION0;
     float2 tex0 : TEXCOORD0;
     float2 tex1 : TEXCOORD1;
     float3 lightVec : TEXCOORD2;
     float3 ViewVec : TEXCOORD3;
     float att : TEXCOORD4;
	 float3 LightDir : TEXCOORD5;
	 float3 Position2D : TEXCOORD6;
};

//pixel shader to screen
struct p2f
{
     float4 color : COLOR0;
};

//Functions
float CalculateLyon(float3 VV, float3 LV, float3 NV)
{
     float3 HalfWay     = normalize(VV + LV);
     float3 Difference  = HalfWay - NV;
     float  SS          = saturate(dot(Difference, Difference) * 1.5);
     return pow(1 - SS, 3);
}

//VERTEX SHADER
void vs( in a2v IN, out v2p OUT )
{
     //getting to position to object space
    OUT.position = mul(IN.position, matWorldViewProj);
    OUT.Position2D = OUT.position;

    //getting the position of the vertex in the world
    float4 posWorld = mul(IN.position, matWorld);
	
    //getting vertex -> light vector
    float3 light = normalize(lightPos - posWorld);

    //Calculating the binormal and setting the Tangent Binormal and Normal matrix
    float3x3 TBNMatrix = float3x3(IN.tangent, IN.binormal , IN.normal);
    float3x3 WTTS   = mul(TBNMatrix, (float4x3)matWorld);

    //setting the lightVector
    OUT.lightVec = mul(WTTS, light);
	
    //Setting the ViewVector
    OUT.ViewVec = mul(WTTS, vecViewPos - posWorld);

    //calculate the attenuation
    OUT.att = (lightPos.xyz - posWorld);

	//Light Direction
	OUT.LightDir = (lightPos.xyz - posWorld) * BIAS;

    OUT.tex0 = IN.tex0;
    OUT.tex1 = IN.tex0;
}

//PIXEL SHADER
void ps( in v2p IN, out p2f OUT )
{
	vecSkill1.w = 5000;
	
    //normalize the light
    float3 light = normalize(IN.lightVec);

    //Normalize the view vector
    float3 rViewVec = normalize(IN.ViewVec);

    //Parallax stuff..
    float2 rUV = (tex2D(Normal1, IN.tex1).a * pStrength - (pStrength * 0.5)) * rViewVec.xy + IN.tex1;
	
    //calculate the Diffuse color 
    float4 color = tex2D(Diffuse0, rUV);
	
    //Uncompress the normal map
    float3 normal = 2.0f * tex2D(Normal1, rUV).rgb - 1.0f;

    //Calc Light att
    float D0 =  saturate(dot(normal, light)) * clamp(1 - length(IN.att) / lightrange, 0, 1);

    //Calulate Specular...
    float Spec     = tex2D(SpecSample, rUV).r * 4;
    float Specular  = CalculateLyon(rViewVec, light, normal);
	
	//Grab Distance
	float dist = length(IN.LightDir.xyz);
	//dist = ((IN.Position2D.z)/vecSkill1.w);
	
	//Shadow color in otherwords black.....
	float4 sFactor  = float4(0, 0, 0, 1);
	
	//Grab shadow map
	float shadowMapDepth = texCUBE(cubeShadowMapSampler, -float4(IN.LightDir.xyz, 1.0f)).r;
	
	//OUT.color = D0 * (color + ((Spec * Specular) * SpecMP));
	//OUT.color.xyz = shadowMapDepth;//IN.LightDir;
	//OUT.color.a = 1;
	
	
	//Compare Distance to our depth map.
    if(dist > shadowMapDepth)    
    {
		//Shadow = true so change pixel to sFactor color
		OUT.color = sFactor;
	}
	else
	{
		//Multiply the attenuation with the color + (specular texure * lyon specular * Specular Multiplyer
		OUT.color = 1;//D0 * (color + ((Spec * Specular) * SpecMP));
		//Make sure the alpha out is set to 1.
		OUT.color.a = 1.0f;
	}
	
	
}
//Techniques..
//Uncomment this if you want 3.0 or just stick with the 2.0 :)
// technique TSM3
// {
    // pass p0
    // {
        // vertexshader = compile vs_3_0 vs();
        // pixelshader = compile ps_3_0 ps();
    // }
// }
technique TSM2
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs();
        pixelshader = compile ps_2_0 ps();
    }
}