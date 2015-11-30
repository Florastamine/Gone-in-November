float4x4 matWorld;
float4x4 matWorldViewProj;

float4 vecViewPort;
float4 vecTime;
float4 vecSkill41;
float4 vecSkill1;

texture entSkin1;
texture mtlSkin1;

sampler smpTexture = sampler_state
{
	Texture = <entSkin1>;
};

sampler smpDiffuse = sampler_state
{
	Texture = <mtlSkin1>;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float3 wpos : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

VS_OUT vs(
	float4 pos : POSITION,
	float2 tex : TEXCOORD0,
	float3 normal : NORMAL)
{
	VS_OUT o;
	o.pos = mul(pos, matWorldViewProj);
	
	o.wpos = mul(pos, matWorld).xyz;
	o.normal = mul(normal, (float3x3)matWorld);
	return o;
}

float4 ps(VS_OUT i) : COLOR0
{
	// Determine the blend weights for the 3 planar projections.  
	// N_orig is the vertex-interpolated normal vector.  
	float3 blend_weights = abs(i.normal);   // Tighten up the blending zone:  
	blend_weights = (blend_weights - 0.2) * 7;  
	blend_weights = max(blend_weights, 0);      // Force weights to sum to 1.0 (very important!)  
	blend_weights /= (blend_weights.x + blend_weights.y + blend_weights.z ).xxx;   
	// Now determine a color value and bump vector for each of the 3  
	// projections, blend them, and store blended results in these two  
	// vectors:  

	// Compute the UV coords for each of the 3 planar projections.  
	// tex_scale (default ~ 1.0) determines how big the textures appear.  
	float2 coord1 = i.wpos.zy * (0.125 / 8) + float2(0, vecSkill1.x * vecTime.w);  
	float2 coord2 = i.wpos.zx * (0.125 / 8) + float2(0, vecSkill1.y * vecTime.w);  
	float2 coord3 = i.wpos.xy * (0.125 / 8) + float2(0, vecSkill1.z * vecTime.w);  
	// This is where you would apply conditional displacement mapping.  
	//if (blend_weights.x > 0) coord1 = . . .  
	//if (blend_weights.y > 0) coord2 = . . .  
	//if (blend_weights.z > 0) coord3 = . . .  
	// Sample color maps for each projection, at those UV coords.  
	float4 col1 = tex2D(smpTexture, coord1);  
	float4 col2 = tex2D(smpTexture, coord2);  
	float4 col3 = tex2D(smpTexture, coord3);  
	 // Finally, blend the results of the 3 planar projections.  
	float4 color = col1.xyzw * blend_weights.xxxx +  
					col2.xyzw * blend_weights.yyyy +  
					col3.xyzw * blend_weights.zzzz;

	color.rgb *= vecSkill41.x;
	color.a = 1;
	return color;
}

technique 
{
	pass
	{
		Lighting = false;
		AlphaBlendEnable = true;
		AlphaTestEnable = false;
		
		SrcBlend = One;
		DestBlend = One;
		
		ZEnable = true;
		ZWriteEnable = false;
		
		ZFunc = LessEqual;
		FillMode = Solid;
		CullMode = CCW;
		
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}
