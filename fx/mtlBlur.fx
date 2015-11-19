float4x4 matWorldViewProj;	
float4x4 matWorld;
float4 vecViewPos;

float4 vecSkill41;

texture entSkin1;

sampler basemap = sampler_state { Texture = <entSkin1>; };

void blur1VS( uniform float exten,
in float4 inNormal : NORMAL0,
in float4 inPosition : POSITION0,
in float4 inTexcoord : TEXCOORD0,
out float4 outPosition : POSITION0,
out float4 outTexcoord : TEXCOORD0)
{
	float3 PosWorld = mul(inPosition, matWorld);
	
	float focus_distance=10;
	
	float blurriness=0.3;
   float depth = (((distance(PosWorld, vecViewPos))*.01)-focus_distance)*blurriness;

	depth*=sign(depth)+2;

	inPosition.xyz += abs(depth)*inNormal/exten;
	outTexcoord = inTexcoord;
	outPosition = mul(inPosition, matWorldViewProj);
	
}

void blur1PS ( uniform float alpha,
	in float4 inTexcoord : TEXCOORD0,
	out float4 outColor : COLOR0){
	outColor = tex2D(basemap, inTexcoord.xy);
	outColor.a=alpha;
}


void mainVS( in float4 inNormal : NORMAL0,
	in float4 inPosition : POSITION0,
	in float4 inTexcoord : TEXCOORD0,
	out float4 outPosition : POSITION0,
	out float4 outTexcoord : TEXCOORD0)
{
	outTexcoord = inTexcoord;
	outPosition = mul(inPosition, matWorldViewProj);
 }

void mainPS(in float4 inTexcoord : TEXCOORD0, out float4 outColor : COLOR0) { outColor = tex2D(basemap, inTexcoord.xy); outColor.a=1; }

technique t1 {
	pass p0 {
		ZENABLE = TRUE;
		zwriteenable=true;
		AlphaBlendEnable = true;
		VertexShader = compile vs_1_1 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
	
	pass p1 {
		ZENABLE = TRUE;
		zwriteenable=true;
		AlphaBlendEnable = true;
		VertexShader = compile vs_1_1 blur1VS(1.4);
		PixelShader = compile ps_2_0 blur1PS(0.5);
	}
	pass p2 {
		ZENABLE = TRUE;
		zwriteenable=true;
		AlphaBlendEnable = true;
		VertexShader = compile vs_1_1 blur1VS(0.9);
		PixelShader = compile ps_2_0 blur1PS(0.3);
	}
} 
