float4x4 matView;
float4x4 matViewInv;
float4x4 matWorld;
float4x4 matWorldInv;
float4x4 matWorldView;
float4x4 matWorldViewProj;
float4x4 matProj;

float NearPlane = 1;
float FarPlane = 50000;
float4 vecLightPos[8];

texture entSkin1;
texture entSkin2;
texture mtlSkin1;

float3 vecViewPos;

sampler colorSampler = sampler_state
{
    Texture = (entSkin1);
    AddressU = WRAP;
    AddressV = WRAP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

sampler normalSampler = sampler_state
{
    Texture = (entSkin2);
    AddressU = WRAP;
    AddressV = WRAP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

sampler volSampler = sampler_state
{
    Texture = (mtlSkin1);
    AddressU = WRAP;
    AddressV = WRAP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

struct VS_IN
{
	float4 Position	:	POSITION;
	float3 Normal	:	NORMAL;
	float4 Tex : TEXCOORD0;
	float4 Tangent : TEXCOORD0;
};

struct VS_OUT
{
	float4 Position	:	POSITION;
	float2 Tex: TEXCOORD0;
	float4 Light : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 Binorm : TEXCOORD4;
	float3 ViewDir : TEXCOORD5;
};

VS_OUT parabol_VS(VS_IN IN)
{
	VS_OUT OUT;
	
	OUT.Tex = IN.Tex;
	
	float3 PosWorld = mul(IN.Position, matWorld);
	OUT.Light.xyz = vecLightPos[0] - PosWorld;
	OUT.Light.w = 1-distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
	OUT.ViewDir = matViewInv[3].xyz - PosWorld;
	float3 Binormal = cross(IN.Normal,IN.Tangent);
	OUT.Normal.xyz = mul(IN.Normal, matWorld).xyz;
	OUT.Tangent.xyz = mul(IN.Tangent, matWorldInv).xyz;
	OUT.Binorm.xyz = mul(Binormal, matWorldInv).xyz;
	
	OUT.Position = mul(IN.Position, matWorldViewProj);

	//OUT.Normal = mul(IN.Normal,matWorldViewProj);
	return OUT;
}
 

float4 parabol_PS(VS_OUT IN):COLOR0
{
	float4 color = 0;
	
	float3 Ln = normalize(IN.Light.xyz);
   float3 Nn = normalize(IN.Normal);
   float3 Tn = normalize(IN.Tangent);
   float3 Bn = normalize(IN.Binorm);
   float3 Nb = 0;
   float3 Vn = 0;
   float3 Hn = 0;
	float4 lighting = 0;
	
	float3 bumpNormal = (tex2D(normalSampler,float2(IN.Tex.x,1-IN.Tex.y)).xyz-(0.5).xxx);
	Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   Nb = normalize(Nb);
   Vn = normalize(IN.ViewDir);
   Hn = normalize(Vn + Ln);
   lighting = lit(dot(Ln,Nb),dot(Hn,Nb),50);
	
	
	
	
	//color.xyz = normalize(IN.Normal);
	color.xyz = lighting.y*tex2D(colorSampler,IN.Tex.xy).xyz;
	//color.a = tex2D(colorSampler,IN.Tex).a;
	
	color.xyz = texCUBE(volSampler,float4((IN.Normal),1));
	color.a = color.xyz;
	
	return color;
}

technique parabol
{
	pass p0
	{
		alphablendenable = true;
		//cullmode = none;
		VertexShader = compile vs_2_0 parabol_VS();
		PixelShader = compile ps_2_0 parabol_PS();
		//VertexShader = compile vs_2_0 main();
	}
}




   
