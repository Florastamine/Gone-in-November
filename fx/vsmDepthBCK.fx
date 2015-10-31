float4x4 matWorld;
float4x4 matView;
float4x4 matProj;

float light_atten_end = 10000;

struct DepthVSOutput
{
	float4 position_screen : POSITION;
	float3 light_vec       : TEXCOORD0;
};

DepthVSOutput depth_VS(
		float4 position : POSITION,
		uniform float4x4 shadow_view,
		uniform float4x4 shadow_view_projection)
{
	DepthVSOutput output   = (DepthVSOutput)0;
	
	// First transform position into world space
	float4 position_world  = mul(position, matWorld);
    output.position_screen = mul(position_world, shadow_view_projection);
    output.light_vec       = mul(position_world, shadow_view).xyz;
    return output;
}

struct DepthPSOutput
{
	float4 depth : COLOR;
};

DepthPSOutput depth_PS(DepthVSOutput input)
{
	DepthPSOutput output = (DepthPSOutput)0;
	
	// Work out the depth of this fragment from the light, normalized to 0->1
	float2 depth;
    depth.x = length(input.light_vec) / light_atten_end;
    depth.y = depth.x * depth.x;

	output.depth = depth.xyxy;
    return output;
}

technique depthmap
{
	pass RenderShadowMap
	{
		//VertexShader = compile vs_2_0 depth_VS(u_shadow_view, mul(u_shadow_view, u_shadow_projection));
		VertexShader = compile vs_2_0 depth_VS(matView, mul(matView, matProj));
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		CullMode = None;
		PixelShader = compile ps_2_0 depth_PS();
	}
}