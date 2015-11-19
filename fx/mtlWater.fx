/******************************************************
By: Eric Hendrickson-Lambert (Steempipe)

v11.08.04.1

11/08/04: Cleaned up code for release.

Note:	Pixelshader1.1 & DX9.00c Runtimes needed.

It will take experimenting with
scaling, shifting, etc~ depending on
what normalmap is used.  The ripple
code is still in very early stages.  

-See the main WDL for loading the material effect
and the d3d_shaderversion function.

If used, credit would be nice... these take alot
of time to work out.  Thanks.	

******************************************************/		

float4x4 matWorldViewProj;
float4x4 matWorld;
float4x4 matWorldView;

float3 vecViewPos;
float4 vecTime;
float4 vecFog;

float	RippleScale = 0.2;
float RippleSpeed = 0.003;

Texture mtlSkin1;	// Normalmap
Texture mtlSkin2;       // Cubemap


sampler s_Bump = sampler_state
{
	Texture = (mtlSkin1);
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

sampler s_Cube = sampler_state
{
	Texture = (mtlSkin2);    
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = CLAMP;
	AddressV  = CLAMP;
	AddressW  = CLAMP;
};

struct VS_IN
{
	float4 Pos:  	POSITION;
	float3 Normal:	NORMAL;
	float2 Bump:		TEXCOORD0;
	float3 Tangent :	TEXCOORD2;
};


struct VS_OUT
{
	float4 Pos:  POSITION;
	float  Fog:  FOG;
	float2 Bump:	TEXCOORD0;
	float4 TanToCube0:	TEXCOORD1;
	float4 TanToCube1:	TEXCOORD2;
	float4 TanToCube2:	TEXCOORD3;
};


VS_OUT WaterBump_VS(VS_IN IN)
{
	VS_OUT Out;

	Out.Pos	= mul(IN.Pos, matWorldViewProj);
	
	Out.Bump= (IN.Bump.xy * RippleScale) + IN.Bump.xy + (vecTime.w * 0.1*RippleSpeed);

	float3x3 ObjToTan;
	
	ObjToTan[0] = RippleScale * IN.Tangent;
	ObjToTan[1] = RippleScale * cross(IN.Tangent, IN.Normal);
	ObjToTan[2] = IN.Normal;

	Out.TanToCube0.xyz = mul(ObjToTan, matWorld[0].xyz);
	Out.TanToCube1.xyz = mul(ObjToTan, matWorld[1].xyz);
	Out.TanToCube2.xyz = mul(ObjToTan, matWorld[2].xyz);


	float3 PositionWorld = mul(IN.Pos, matWorld);
	float3 ViewerDir = normalize(vecViewPos - PositionWorld);

	Out.TanToCube0.w = ViewerDir.x;
	Out.TanToCube1.w = ViewerDir.y;
	Out.TanToCube2.w = ViewerDir.z;

	float ofog = 1 - (distance(PositionWorld, vecViewPos) - vecFog.x) * vecFog.z;
	Out.Fog = ofog;
	
	return Out;
}

technique WaterBump
{
	pass One
	{
		Sampler[0]	= (s_Bump);
		Sampler[3]	= (s_Cube);
		
		AlphaBlendEnable = True;
		ZwriteEnable = True;
		ZEnable = True;

		//  Main point is that pixelShaderConstant 'W' will control
		//  the alpha tranparency.  The XYZ will adjust
		//  some coloring.
		PixelShaderConstant[0] = {0.80,0.80,0.92,0.88}; // x, y, z, w

		VertexShader = compile vs_1_1 WaterBump_VS();
		
		PixelShader =
		asm
		{
			ps.1.1

			tex t0		// Normalmap
			texm3x3pad   t1,  t0_bx2
			texm3x3pad   t2,  t0_bx2
			texm3x3vspec t3,  t0_bx2

			mul r0, t3, c0
			
		};
	}
}
