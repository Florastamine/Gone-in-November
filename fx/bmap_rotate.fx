float4 vecSkill1;
float4 vecSkill5;
float4 vecSkill9;
texture TargetMap;
sampler smpSource = sampler_state { Texture = <TargetMap>; MipFilter = Linear; MinFilter = Anisotropic; MagFilter = Anisotropic; AddressU = Border; AddressV = Border; };

float4 RotatePS (
	in float4 inTex: TEXCOORD0 ) : COLOR0
	{
		float2 coor = inTex.xy - vecSkill5.zw - 0.5f;
		coor.x *= vecSkill9.z;
		coor.xy *= vecSkill9.xy;
		float x = ( coor.x * vecSkill1.x ) + ( coor.y * vecSkill1.y );
		coor.y = ( coor.x * vecSkill1.z ) + ( coor.y * vecSkill1.w );
		coor.x = x;
		coor.x /= vecSkill9.z;
		coor.xy += 0.5f + float2(vecSkill5.x,vecSkill5.y);
		return tex2D ( smpSource, coor );
	}

technique Rotate
{
	pass p0
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader  = compile ps_2_0 RotatePS();
	}
}
