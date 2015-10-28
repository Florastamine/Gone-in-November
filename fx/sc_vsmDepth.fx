float4x4 matWorld;
float4x4 matView;
float4x4 matProj;

float light_atten_end = 4000*0.75;


texture entSkin1;
sampler colorSampler = sampler_state
{
    texture   = <entSkin1>;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct DepthVSOutput
{
	float4 position_screen : POSITION;
	float3 light_vec       : TEXCOORD0;
	float2 tex       : TEXCOORD1;
};

DepthVSOutput depth_VS(
		float4 position : POSITION,
		float2 inTex : TEXCOORD0,
		uniform float4x4 shadow_view,
		uniform float4x4 shadow_view_projection)
{
	DepthVSOutput output   = (DepthVSOutput)0;
	
	// First transform position into world space
	float4 position_world  = mul(position, matWorld);
    output.position_screen = mul(position_world, shadow_view_projection);
    output.light_vec       = mul(position_world, shadow_view).xyz;
    output.tex = inTex;
    return output;
}

struct DepthPSOutput
{
	float4 depth : COLOR;
};

DepthPSOutput depth_PS(DepthVSOutput input)
{
	DepthPSOutput output = (DepthPSOutput)0;
	
	
	clip(tex2D(colorSampler, input.tex).a-0.5);
	// Work out the depth of this fragment from the light, normalized to 0->1
	float2 depth;
    
    depth.x = length(input.light_vec) / light_atten_end;
    depth.y = depth.x * depth.x;
    
	
	/*
	float4 color = 0;
	color.r = depth.x;
	color.r = floor(color.r*127)/127;
   color.g = floor((depth.x-color.r)*127*127)/127;
   
   color.b = depth.y;
   color.b = floor(color.b*127)/127;
   color.a = floor((depth.x-color.b)*127*127)/127;
   
   output.depth = color.rgba;
   */
	
	output.depth = depth.xyxy;
	output.depth.z = 0;
	output.depth.w = tex2D(colorSampler, input.tex).a;
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
		CullMode = CCW;
		PixelShader = compile ps_2_0 depth_PS();
	}
}