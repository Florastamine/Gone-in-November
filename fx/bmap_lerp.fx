float4 vecSkill1;
texture TargetMap;
texture mtlSkin1;
sampler smpSource1 = sampler_state { Texture = <TargetMap>; MipFilter = Linear; MinFilter = Anisotropic; MagFilter = Anisotropic; };
sampler smpSource2 = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; MinFilter = Anisotropic; MagFilter = Anisotropic; };

float4 LerpPS (
	in float4 inTex: TEXCOORD0 ) : COLOR0
	{
		float4 color1 = tex2D ( smpSource1, inTex.xy );
		float4 color2 = tex2D ( smpSource2, inTex.xy );
		return lerp ( color1, color2, vecSkill1.x );
	}

technique Lerp
{
	pass p0
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader  = compile ps_3_0 LerpPS();
	}
}
