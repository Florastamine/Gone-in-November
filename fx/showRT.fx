Texture TargetMap;
Texture mtlSkin1;
		sampler2D smpSource = sampler_state { texture = <mtlSkin1>;MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear; };
		float4 vecViewPort; // contains viewport pixel size in zw components
		float4 vecSkill1;
		float4 BlackoutPS( float2 Tex : TEXCOORD0 ) : COLOR0 
		{
			float4 Color = tex2D(smpSource,(Tex.xy));
			//Color.rgb = Color.r;
			//Color.a = 1;
			return Color;
		}

		technique emboss { pass one { PixelShader = compile ps_2_0 BlackoutPS(); } }