/******************************************************************************************************
Water Shader by Wolfgang "BoH_Havoc" Reichardt

Entity Textures:
	Skin1 = Normalmap
	Skin2 = Foam
	Skin3 = Depthmap (optional)
	
******************************************************************************************************/


/***************************************TWEAKABLES*****************************************************/

//Use a static depthmap instead of a dynamic one (entSkin3)
//#define DEPTHMAP

//Waves
float Displacement = 8.0f;
float Speed = 0.06f;
float TurbDensity = 0.27f;
float ColorRange = 1.0f;
float ColorSharpness = 3.5f;
//

//reflectcolor
float4 reflectColor = {1, 1, 1, 1};
//refractcolor
float4 refractColor = {1, 1, 1, 1};
//the distance over which the water surface will be fully reflective, must be specified in squared distance
float fullReflectionDistance = 1.0;
//a parameter controlling the global transparency of water, must be in range[0,1]
float globalTransparency = 0.25;
//the strength of disturbing water reflection and refraction
float reflectionStrength = 0.25; //0.25
//the distance over which the foam is seen in xz space
float maxFoamDistance = 900000;
//a tile coefficient which specifies how to scale the original UV coordinates
//to obtain new coorditnates for foam texture
float foamTiling = 10;
//a factor which is applied to perturb the foam texture (just like reflection disturbing)
float foamDistortion = 0.15;
//a value which specifies additional offset for foam texture, which can be used
//as a time value, which is increasing, making the foam move
float foamTranslation = 0.001;
//a coefficient that scales the foam transparency at low depth
float shoreFoamScale = 1.0;
//a coefficient that scales the foam transparency at high depth
float foamScale = 0.7;
//this param specifies at which height the foam must start to be visible in range [0,1]
float foamStartHeight = 0.95;
//the strength of sunlight reflection
float sunlightStrength = 0.5;
//the area of sunlight reflection
float sunlightArea = 400;
//a param specifying how transparent is the water near shore, when depth texture is used
float shoreTransparency = 1.0;
//scale of the bump texture
float bumpTexScale = 4;

/***************************************SHADER*CODE****************************************************/

float4 vecFog;
float4 vecViewPort;

texture entSkin1; //normalmap
texture entSkin2; //foam
texture entSkin3; //static depthmap (optional)

sampler normalmapSampler = sampler_state
{
	Texture = <entSkin1>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;   
	//MinFilter = POINT;
	//MagFilter = POINT;
	//MipFilter = POINT;   
	AddressU  = wrap;
	AddressV  = wrap;
	
};

sampler foamSampler = sampler_state
{
	Texture = <entSkin2>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;   
	AddressU  = wrap;
	AddressV  = wrap;
	
};

texture mtlSkin1;
texture mtlSkin2;
texture mtlSkin3;
texture mtlSkin4;


sampler reflectionSampler=sampler_state
{

	texture=<mtlSkin1>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    addressu=clamp;
    addressv=clamp;

};

sampler refractionSampler=sampler_state
{
	texture=<mtlSkin2>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    addressu=clamp;
    addressv=clamp;
};

sampler depthSampler=sampler_state
{
	#ifdef DEPTHMAP
	texture=<entSkin3>;
	#else
	texture=<mtlSkin3>;
	#endif
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    addressu=clamp;
    addressv=clamp;
};

sampler fresnelSampler = sampler_state
{
	Texture = <mtlSkin4>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;   
	AddressU  = wrap;
	AddressV  = wrap;
	
};

float4x4 matWorldViewProj;
float4x4 matWorld;
float4x4 NoiseMatrix = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

float mtlSkill1;
float4 vecTime;

float waveSpeed = 6000;
float3 vecViewPos;
float4 vecSunPos;
float4 vecSunColor;

#define BSIZE 32
#define FULLSIZE 66
#define NOISEFRAC 0.03125

const float4 NTab[66] =
 {
 		-0.854611,-0.453029,0.25378,0,
		-0.84528,-0.456307,-0.278002,1,
		-0.427197,0.847095,-0.316122,2,
		0.670266,-0.496104,0.551928,3,
		-0.675674,-0.713842,0.184102,4,
		-0.0373602,-0.600265,0.798928,5,
		-0.939116,-0.119538,0.322135,6,
		0.818521,0.278224,0.502609,7,
		0.105335,-0.765291,0.635007,8,
		-0.634436,-0.298693,0.712933,9,
		-0.532414,-0.603311,-0.593761,10,
		0.411375,0.0976618,0.906219,11,
		0.798824,-0.416379,-0.434175,12,
		-0.691156,0.585681,-0.423415,13,
		0.612298,0.0777332,0.786797,14,
		0.264612,-0.262848,0.927842,15,
		-0.70809,0.0548396,-0.703989,16,
		0.933195,-0.294222,-0.206349,17,
		0.788936,-0.466718,-0.399692,18,
		-0.540183,-0.824413,0.168954,19,
		0.469322,-0.184125,0.863617,20,
		-0.84773,0.292229,-0.44267,21,
		0.450832,0.650314,-0.611427,22,
		0.906378,-0.247125,-0.342647,23,
		-0.995052,0.0271277,-0.0955848,24,
		-0.0252277,-0.778349,0.627325,25,
		0.991428,0.128623,0.0229457,26,
		-0.842581,-0.290688,0.453384,27,
		-0.662511,-0.500545,-0.557256,28,
		0.650245,-0.692099,-0.313338,29,
		0.636901,0.768918,-0.0558766,30,
		-0.437006,0.872104,-0.220138,31,
		-0.854611,-0.453029,0.25378,0,
		-0.84528,-0.456307,-0.278002,1,
		-0.427197,0.847095,-0.316122,2,
		0.670266,-0.496104,0.551928,3,
		-0.675674,-0.713842,0.184102,4,
		-0.0373602,-0.600265,0.798928,5,
		-0.939116,-0.119538,0.322135,6,
		0.818521,0.278224,0.502609,7,
		0.105335,-0.765291,0.635007,8,
		-0.634436,-0.298693,0.712933,9,
		-0.532414,-0.603311,-0.593761,10,
		0.411375,0.0976618,0.906219,11,
		0.798824,-0.416379,-0.434175,12,
		-0.691156,0.585681,-0.423415,13,
		0.612298,0.0777332,0.786797,14,
		0.264612,-0.262848,0.927842,15,
		-0.70809,0.0548396,-0.703989,16,
		0.933195,-0.294222,-0.206349,17,
		0.788936,-0.466718,-0.399692,18,
		-0.540183,-0.824413,0.168954,19,
		0.469322,-0.184125,0.863617,20,
		-0.84773,0.292229,-0.44267,21,
		0.450832,0.650314,-0.611427,22,
		0.906378,-0.247125,-0.342647,23,
		-0.995052,0.0271277,-0.0955848,24,
		-0.0252277,-0.778349,0.627325,25,
		0.991428,0.128623,0.0229457,26,
		-0.842581,-0.290688,0.453384,27,
		-0.662511,-0.500545,-0.557256,28,
		0.650245,-0.692099,-0.313338,29,
		0.636901,0.768918,-0.0558766,30,
		-0.437006,0.872104,-0.220138,31,
		-0.854611,-0.453029,0.25378,0,
		-0.84528,-0.456307,-0.278002,1
};

float3 s_curve(float3 t) { return t*t*( float3(3,3,3) - float3(2,2,2)*t); }

float noise(float3 v,
			const uniform float4 pg[FULLSIZE])
{
    v = v + float3(10000.0f, 10000.0f, 10000.0f);   // hack to avoid negative numbers

    float3 i = frac(v * NOISEFRAC) * BSIZE;   // index between 0 and BSIZE-1
    float3 f = frac(v);            // fractional position

    // lookup in permutation table
    float2 p;
    p.x = pg[ i[0]     ].w;
    p.y = pg[ i[0] + 1 ].w;
    p = p + i[1];

    float4 b;
    b.x = pg[ p[0] ].w;
    b.y = pg[ p[1] ].w;
    b.z = pg[ p[0] + 1 ].w;
    b.w = pg[ p[1] + 1 ].w;
    b = b + i[2];

    // compute dot products between gradients and vectors
    float4 r;
    r[0] = dot( pg[ b[0] ].xyz, f );
    r[1] = dot( pg[ b[1] ].xyz, f - float3(1.0f, 0.0f, 0.0f) );
    r[2] = dot( pg[ b[2] ].xyz, f - float3(0.0f, 1.0f, 0.0f) );
    r[3] = dot( pg[ b[3] ].xyz, f - float3(1.0f, 1.0f, 0.0f) );

    float4 r1;
    r1[0] = dot( pg[ b[0] + 1 ].xyz, f - float3(0.0f, 0.0f, 1.0f) );
    r1[1] = dot( pg[ b[1] + 1 ].xyz, f - float3(1.0f, 0.0f, 1.0f) );
    r1[2] = dot( pg[ b[2] + 1 ].xyz, f - float3(0.0f, 1.0f, 1.0f) );
    r1[3] = dot( pg[ b[3] + 1 ].xyz, f - float3(1.0f, 1.0f, 1.0f) );

    // interpolate
    f = s_curve(f);
    r = lerp( r, r1, f[2] );
    r = lerp( r.xyyy, r.zwww, f[1] );
    return lerp( r.x, r.y, f[0] );
}

void water_VS( in float4 inPos:POSITION, //input vertex position
					in float2 inTex:TEXCOORD0, //input texcoords
				   out float4 outPos:POSITION, //ouput transformed position
				   out float2 outTex:TEXCOORD0, //output texcoords
				   out float4 outProjectedTex:TEXCOORD1, //output projected texcoords
				   out float3 outVertexPos:TEXCOORD2,
				   out float4 outColor:TEXCOORD3,//untrasnformed vertex position
				   out float OutFog : FOG) 
{
	//animate vertex via noise |generate waves
	float4 noisePos = TurbDensity*mul(inPos+(Speed*vecTime.w),NoiseMatrix);
	float height = noise(noisePos.xyz, NTab);
	
	float4 newPos=inPos;
	newPos.y=inPos.y+height*Displacement;//*extrudeFactor*heightScale;
	outPos=mul(newPos,matWorldViewProj);
	//
	//transform to clip space
	//outPos=mul(inPos,matWorldViewProj);
	//save the vertex information for further projected texture coordinate computation
	outProjectedTex=outPos;
	//ouput texcoords
	outTex=inTex+(vecTime.w/waveSpeed);
	//output untransformed vertex pos
	//outVertexPos=inPos.xyz;
	outVertexPos=newPos.xyz;
	
	
	//color
	float i = (noise(noisePos.xyz, NTab) + 1.0f) * 0.5f;
	//i *= (noise(noisePos.xyz*2, NTab) + 1.0f) * 0.5f;
	//i *= (noise(noisePos.xyz*4, NTab) + 1.0f) * 0.5f;
    //float cr = 1.0-(0.5+ColorRange*(i-0.5));
    float cr = 1.0-(0.5+ColorRange*(i+0.5));
    cr = pow(cr,ColorSharpness);
    outColor = float4((cr).xxx, 1.0f);
    
    OutFog = 1 - (distance(mul(inPos,matWorld), vecViewPos) - vecFog.x) * (vecFog.z);
    
}

float4 water_PS(in float2 inTex:TEXCOORD0, //input texture coords
					  in float4 inProjectedTex:TEXCOORD1, //projectedTexcoords, needed to perform reflections,refractions
					  in float3 inVertexPos:TEXCOORD2,
					  in float3 inColor:TEXCOORD3):COLOR0 //original vertex position
{
	clip(mtlSkill1);
	
	//inProjectedTex.xy/=inProjectedTex.w;
	//inProjectedTex.xy=inProjectedTex.xy*0.5f*float2(1,-1)+0.5f;
	//float2 ProjectedTexCoords;
   inProjectedTex.x = inProjectedTex.x/inProjectedTex.z/2.0f +0.5f;
   inProjectedTex.y = -inProjectedTex.y/inProjectedTex.z/2.0f +0.5f;
   //inProjectedTex.y *= (0.9);
	
	float3 pixelNormal=tex2D(normalmapSampler,inTex*bumpTexScale);
   pixelNormal.yz=pixelNormal.zy;
   pixelNormal=(pixelNormal.xyz-0.5f)*2;
   
   float waterDepth = tex2D(depthSampler,inProjectedTex.xy).g;
	float4 reflection=tex2D(reflectionSampler,inProjectedTex.xy+reflectionStrength*pixelNormal.xz);
	float4 refraction=tex2D(refractionSampler,inProjectedTex.xy-reflectionStrength*pixelNormal.xz);
	
	//sunlight
    float3 camToSurface=inVertexPos-vecViewPos;
    camToSurface=normalize(-camToSurface);
	float3 relfectedVector = normalize(reflect(-camToSurface,pixelNormal));        
	float3 surfaceToSun=normalize(vecSunPos-inVertexPos);
    float3 sunlight = sunlightStrength*pow(saturate(dot(relfectedVector,surfaceToSun)),sunlightArea)*vecSunColor;
	//
	
	//fresnel
	float additionalReflection=camToSurface.x*camToSurface.x+camToSurface.z*camToSurface.z;
   additionalReflection/=fullReflectionDistance;
	float dotProduct=dot(camToSurface,pixelNormal);
   dotProduct=saturate(dotProduct);
	float fresnel=tex1D(fresnelSampler,dotProduct);
	fresnel+=additionalReflection;
	fresnel=saturate(fresnel); 
   fresnel-=globalTransparency;    
   fresnel=saturate(fresnel);
   //
	
	//foam
	float2 foamTex=inTex*foamTiling+pixelNormal.xz*foamDistortion+float2(vecTime.w*foamTranslation,vecTime.w*foamTranslation);
	float4 foam=tex2D(foamSampler,foamTex);
	//
	
	//finalize
	float4 finalColor = lerp(refraction*refractColor,reflection*reflectColor,fresnel);
	finalColor = (refraction*waterDepth)+(finalColor*(1-waterDepth));
	finalColor.xyz += clamp((sunlight.xyz-pow(waterDepth,5)),0,1);;
	finalColor.xyz += clamp((((foam.xyz*foam.a)*waterDepth)-pow(waterDepth,3)),0,1)*shoreFoamScale;
	finalColor.xyz += ((foam.xyz*foam.a)*inColor)*foamScale*(1-waterDepth);
	finalColor.xyz = clamp(finalColor,0,1);
	//
	//finalColor = refraction;
	return finalColor;
};

technique water
{

	pass p0
	{
		CULLMODE = CCW;
		VertexShader  = compile vs_2_0 water_VS();
		PixelShader = compile ps_2_0 water_PS();
	}
}


