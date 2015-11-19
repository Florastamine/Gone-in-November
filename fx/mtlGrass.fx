texture entSkin1;

matrix matWorldViewProj;
matrix matWorldView;

vector vecFog;
vector vecTime;

technique tree
{
	pass p0
	{
		texture[0]=<entSkin1>;
		zWriteEnable=true;
		alphaTestEnable=true;
		cullMode=none;

		vertexShaderConstant[0]=<matWorldViewProj>;
		vertexShaderConstant[8]=<matWorldView>;
		vertexShaderConstant[16]=<vecFog>;
		vertexShaderConstant[32]=<vecTime>;
		vertexShaderConstant[33]={0.0f,0.5f,1.0f,2.0f};										// common constants
		vertexShaderConstant[34]={-0.16161616f,0.0083333f,-0.00019841f,0.000002755731f};	// {-1/3!,1/5!,-1/7!,1/9!}
		vertexShaderConstant[35]={4.0f,1.57079632f,3.14159265f,6.28318530f};				// {4.0,pi/2,pi,pi*2}
		vertexShaderConstant[36]={1.07f,0.0f,0.0f,0.0f};									// fixup factor

		vertexShaderConstant[40]={2.5f,1.0f,0.5f,0.1f};										// wave distort x
		vertexShaderConstant[41]={1.0f,1.5f,1.25f,0.1f};									// wave distort y
		vertexShaderConstant[42]={-1.0f,-1.01f,-1.04f,-0.01f};								// wave distort z
		vertexShaderConstant[43]={-0.1f,-0.2f,0.1f,0.05f};									// wave dir x
		vertexShaderConstant[44]={-0.005f,-0.01f,-0.005f,-0.0025f};							// wave dir y
		//vertexShaderConstant[45]={0.02f,0.04f,0.05f,0.1f};									// wave speed
		vertexShaderConstant[45]={0.005f,0.01f,0.01f,0.1f};									// wave speed
		vertexShaderConstant[46]={0.35f,0.1f,0.1f,0.04f};									// lighting wave scale
		vertexShaderConstant[47]={0.6f,0.8f,0.3f,0.0f};										// lighting scale bias

		vertexShader=
		asm
		{
			vs.1.1

			dcl_position  v0
			dcl_normal    v3
			dcl_texcoord0 v7
			dcl_texcoord1 v8

			mov r1.w,c16.w                  // r1.w=1
			dp4 r0,v0,c10                   // distance to camera position (v0 is the position without sinusodial warping!)
			add r0,r0,-c16.x                // distance-fog_start
			mad r0.x,-r0.x,c16.z,r1.w       // 1-(distance-fog_start)*(1/(fog_end-fog_start))
			max oFog,r0.x,c33.x           // clamp with custom max value

			mul r0,c43,v0.x                 // use vertex pos x as inputs to sinusoidal warp
			mad r0,c44,v0.y,r0              // use vertex pos y as inputs to sinusoidal warp

			mov r1,c32.w                    // get current time
			mad r0,r1,c45,r0                // add scaled time
			frc r0.xy,r0                    // take fraction of all 4 components
			frc r1.xy,r0.zwzw
			mov r0.zw,r1.xyxy

			mul r0,r0,c36.x                 // multiply by fixup factor (due to inaccuracy of taylor series)
			sub r0,r0,c33.y                 // subtract 0.5
			mul r1,r0,c35.w                 // range -pi to pi

			mul r2,r1,r1                    // wave_vec^2
			mul r3,r2,r1                    // wave_vec^3
			mul r5,r3,r2                    // wave_vec^5
			mul r7,r5,r2                    // wave_vec^7
			mul r9,r7,r2                    // wave_vec^9

			mad r0,r3,c34.x,r1              // wave_vec-(wave_vec^3)/3!
			mad r0,r5,c34.y,r0              // +(wave_vec^5)/5!
			mad r0,r7,c34.z,r0              // -(wave_vec^7)/7!
			mad r0,r9,c34.w,r0              // +(wave_vec^9)/9!

			dp4 r3.x,r0,c40
			dp4 r3.y,r0,c41
			dp4 r3.zw,r0,c42

			sub r4,c33.z,v7.y               // attenuate sinusoidal warping by (1-uv.y)^2
			mul r4,r4,r4
			mul r3,r3,r4

			mov r2.w,v0
			add r2.xyz,r3,v0                // add sinusoidal warping to grass position
			m4x4 oPos,r2,c0                 // transform position to clip space

			dp4 r1.x,r0,c46                 // scale and add sin waves together
			mad oD0,c47.xzxz,-r1.x,c47.y    // scale and bias color values (green is scaled more than red and blue)
			mov oT0.xy,v7.xy                // output uvs
		};

		pixelShader=
		asm
		{
			ps.1.3
			def c0,1,1,1,1
			def c1,0,0,0,0
			tex t0                  // sample t0
			mov r0,t0
			cnd r0.a,r0.a,c0,c1     // if(r0.a>0.5){r0.a=1;}else{r0.a=0;}
			//mul_x2 r0.rgb,r0,v0     // modulate with lighting
		};
	}
}
