//////////////////////////////////////////////////////////////////////
// default fx functions for Gamestudio Materials
// (c) jcl/Conitec 2009  Version 1.3
// use: #include <transform>,<sun>, etc.
//////////////////////////////////////////////////////////////////////

#ifdef NIX // prevent that this file is included in C scripts
//////////////////////////////////////////////////////////////////////
// often used shader variables & functions

//////////////////////////////////////////////////////////////////////
//section: define - some common defines //////////////////////////////
#ifndef include_define
#define include_define

//enable: Mirror Fresnel Effect
//help: Water transparency depends on view angle
//id: 1
#define MIRROR_FRESNEL

//enable: Enviroment Fresnel Effect
//help: Water transparency depends on view angle
//id: 11
#define ENVIRO_FRESNEL

//enable: Blinn Shading
//help: Enable for Blinn (better), disable for Phong (faster)
//id: 15
#define BLINN

//enable: Bumpmapping Light Falloff
//help: Enable for light angle modulation at bumpmapped surfaces
//id: 16
#define BUMPFALLOFF

//enable: DX Lighting
//help: Enable for DirectX falloff, disable for 1/r lighting 
//id: 5
//#define DXLIGHTING

struct vertexIn
{
    float4 Pos		: POSITION;
    float4 Ambient: COLOR0;
    float3 Normal:  NORMAL; // expected to be normalized
    float2 Tex1:    TEXCOORD0;
    float2 Tex2:    TEXCOORD1;
    float3 Tangent: TEXCOORD2; // pre-normalized
};
#endif
//////////////////////////////////////////////////////////////////////
//section: transform - calculate vertex position /////////////////////
#ifndef include_transform
#define include_transform

float4x4 matWorldViewProj;
float4 DoTransform(float4 Pos)
{
	return mul(Pos,matWorldViewProj);
}
#endif

//////////////////////////////////////////////////////////////////////
//section: bones - calculate world position with bones transformation
#ifndef include_bones
#define include_bones

float4x3 matBones[72];
int iWeights;

float4 DoBones(float4 Pos,int4 BoneIndices,float4 BoneWeights)
{
	if(iWeights == 0) return Pos;
	float3 OutPos = 0;
	for(int i=0; i<iWeights; i++)
		OutPos += mul(Pos.xzyw,matBones[BoneIndices[i]])*BoneWeights[i];
	return float4(OutPos.xzy,1.0);
}

// only rotation and translation => inv(transpose(matWorld)) == matWorld
float3 DoBones(float3 Normal,int4 BoneIndices,float4 BoneWeights)
{
	if(iWeights == 0) return Normal;
	float3 OutNormal = 0;
	for(int i=0; i<iWeights; i++)
		OutNormal += mul(Normal.xzy,(float3x3)matBones[BoneIndices[i]])*BoneWeights[i];
	return normalize(OutNormal.xzy);
}

#endif

//////////////////////////////////////////////////////////////////////
//section: sun - return the sun light on the surface /////////////////
#ifndef include_sun
#define include_sun

float4 vecSunDir;
float4 vecSunColor;
float4 DoSunLight(float3 N)
{
	return vecSunColor * dot(N,-vecSunDir); // modulate sunlight by the surface angle
}
#endif

//////////////////////////////////////////////////////////////////////
//section: lights - return the dynamic light on the surface //////////
#ifndef include_lights
#define include_lights
#include <define>

int iLights;
float4 vecLightPos[8];	 // light positions (xyz) and ranges (w)
float4 vecLightColor[8]; // light colors

// calculate the light attenuation factor
float DoLightFactor(float4 Light,float3 Pos)
{
   float fac = 0.f;
   if (Light.w > 0.f) {    
      float LD = length(Light.xyz-Pos)/Light.w;
#ifdef DXLIGHTING // DX falloff formula
		if (LD < 1.0f)
			fac = saturate(1.f/(0.f + 0.f*LD + 1.5f*LD*LD));	
#else  // Acknex formula, linear lighting
      if (LD < 1.f)
         fac = saturate(1.f - LD);
#endif  
   }
   return fac; // get the distance factor
}

// calculate the light attenuation factor on the front side
float DoLightFactorBump(float4 Light,float3 P,float3 N)
{
#ifdef BUMPFALLOFF
	float3 D = Light.xyz-P; // ray pointing from the light to the surface
	float NdotL = dot(N,normalize(D));   // angle between surface and light ray
	
	if (NdotL >= 0.f) 
	   return saturate(NdotL*8)*DoLightFactor(Light,P);
	else
	   return 0.f;
#else
     return DoLightFactor(Light,P);
#endif
}

float DoLightFactorN(float4 Light,float3 P,float3 N)
{
	float3 D = Light.xyz-P; // ray pointing from the light to the surface
	float NdotL = dot(N,normalize(D));   // angle between surface and light ray
	
	if (NdotL >= 0.f) 
	   return 2 * NdotL * DoLightFactor(Light,P);
	else
	   return 0.f;
}

float4 DoPointLight(float3 P, float3 N, float4 Light, float4 LightColor)
{
	return LightColor * DoLightFactorN(Light,P,N);
}

float4 DoLight(float3 P, float3 N, int i)
{
	return DoPointLight(P,N,vecLightPos[i],
		float4(vecLightColor[i].x,vecLightColor[i].y,vecLightColor[i].z,1)); 
	//return DoPointLight(P,N,vecLightPos[i],vecLightColor[i]);
}

#endif
//////////////////////////////////////////////////////////////////////
//section: fog ///////////////////////////////////////////////////////
#ifndef include_fog
#define include_fog
#include <define>

float4 vecFog;
float4 vecViewPos;
#ifndef MATWORLDVIEW
float4x4 matWorldView;
#define MATWORLDVIEW
#endif
#ifndef MATWORLD
float4x4 matWorld;
#define MATWORLD
#endif

#ifdef DXFOG
float DoFog(float4 Pos)
{
	float3 P = mul(Pos,matWorldView).xyz; // convert vector to view space to get it's depth (.z)
   	return saturate((vecFog.y-P.z) * vecFog.z); // apply the linear fog formula
}
#else // distance based fog
float DoFog(float4 Pos)
{
	float3 P = mul(Pos,matWorld).xyz;
	return 1 - (distance(P,vecViewPos.xyz)-vecFog.x) * vecFog.z;
}
#endif
#endif
//////////////////////////////////////////////////////////////////////
//section: pos - transform vertex position to world space ////////////
#ifndef include_pos
#define include_pos

#ifndef MATWORLD
float4x4 matWorld;
#define MATWORLD
#endif
float4 DoPos(float4 Pos)
{
	return (mul(Pos,matWorld));
}
float3 DoPos(float3 Pos)
{
	return (mul(Pos,(float3x3)matWorld));
}
#endif
//////////////////////////////////////////////////////////////////////
//section: view - transform vertex position to view space ////////////
#ifndef include_view
#define include_view

#ifndef MATWORLDVIEW
float4x4 matWorldView;
#define MATWORLDVIEW
#endif
float4 DoView(float4 Pos)
{
	return (mul(Pos,matWorldView));
}
float3 DoView(float3 Pos)
{
	return (mul(Pos,(float3x3)matWorldView));
}
#endif

//////////////////////////////////////////////////////////////////////
//section: normal - transform vertex normal //////////////////////////
#ifndef include_normal
#define include_normal

#ifndef MATWORLD
float4x4 matWorld;
#define MATWORLD
#endif

// only rotation and translation => inv(transpose(matWorld)) == matWorld
float3 DoNormal(float3 inNormal)
{
	return normalize(mul(inNormal,(float3x3)matWorld));
}

// reconstruct a compressed normal
float3 CreateNormal(float2 inNormalXY)
{
	float3 n;
	n.xy = inNormalXY * 2 - 1;
	n.z  = sqrt(1.0 - dot(n.xy, n.xy));
	return n;
}
#endif
//////////////////////////////////////////////////////////////////////
//section: tangent - create tangent matrix ///////////////////////////
#ifndef include_tangent
#define include_tangent

float3x3 matTangent;	// hint for the engine to create tangents in TEXCOORD2

void CreateTangents(float3 inNormal,float3 inTangent)
{
	matTangent[0] = DoPos(inTangent);
	matTangent[1] = DoPos(cross(inTangent,inNormal));	// binormal
	matTangent[2] = DoPos(inNormal);
}

void CreateTangents(float3 inNormal,float4 inTangent)
{
	matTangent[0] = DoPos(inTangent.xyz);
	matTangent[1] = DoPos(cross(inTangent.xyz,inNormal)*inTangent.w);	// binormal with correct handedness
	matTangent[2] = DoPos(inNormal);
}

float3 DoTangent(float3 inVector)
{
	return normalize(mul(matTangent,inVector));
}
#endif

//////////////////////////////////////////////////////////////////////
//section: tangent_view - create view tangent matrix /////////////////
#ifndef include_tangent_view
#define include_tangent_view

float3x3 matTangent;	// hint for the engine to create tangents in TEXCOORD2

void CreateViewTangents(float3 inNormal,float3 inTangent)
{
// create matWorldView Rotation-only matrix
   float3x3 matViewRot;
   matViewRot[0] = matWorldView[0].xyz;
   matViewRot[1] = matWorldView[1].xyz;
   matViewRot[2] = matWorldView[2].xyz;

	matTangent[0] = mul(inTangent,matViewRot);
	matTangent[1] = mul(cross(inTangent,inNormal),matViewRot);	// binormal
	matTangent[2] = mul(inNormal,matViewRot);
}

void CreateViewTangents(float3 inNormal,float4 inTangent)
{
// create matWorldView Rotation-only matrix
   float3x3 matViewRot;
   matViewRot[0] = matWorldView[0].xyz;
   matViewRot[1] = matWorldView[1].xyz;
   matViewRot[2] = matWorldView[2].xyz;

	matTangent[0] = mul(inTangent.xyz,matViewRot);
	matTangent[1] = mul(cross(inTangent.xyz,inNormal)*inTangent.w,matViewRot);	// binormal
	matTangent[2] = mul(inNormal,matViewRot);
}

float3 DoTangent(float3 inVector)
{
	return normalize(mul(matTangent,inVector));
}
#endif

//////////////////////////////////////////////////////////////////////
//section: vecskill - set vecSkill41 to default values ///////////////
#ifndef include_vecskill
#define include_vecskill

float4 vecSkill41;

float DoDefault(float vecSkill,float defVal)
{
   if (0 == vecSkill) 
      return defVal;
   else
      return vecSkill;
}
#endif

//////////////////////////////////////////////////////////////////////
//section: srctex - set up the source for postprocessing shaders
#ifndef include_srctex
#define include_srctex

float4 vecViewPort;
float4 vecSkill1;
Texture TargetMap;
sampler2D src = sampler_state { texture = <TargetMap>; };

#endif

//////////////////////////////////////////////////////////////////////
//section: texture - use the texture matrix for scaling + translation
#ifndef include_texture
#define include_texture
float4x4 matTexture;

float2 DoTexture(float2 Tex)
{
   return mul(float3(Tex.x,Tex.y,1),matTexture).xy;
}
#endif

//////////////////////////////////////////////////////////////////////
//section: color - calculate final color from Diffuse, Ambient, Lightmap, etc.
#ifndef include_color
#define include_color
float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float fPower;

float4 DoAmbient()
{
	return (vecAmbient * vecLight) + float4(vecEmissive.xyz*vecColor.xyz,vecLight.w);	
}

float DoSpecular()
{
	return (vecSpecular.x+vecSpecular.y+vecSpecular.z)*0.333;	
}

float4 DoLightmap(float3 Diffuse,float3 Lightmap,float4 Ambient)
{
   return float4(Diffuse+Lightmap*(Diffuse+Ambient.xyz),Ambient.w);
}

float4 DoColor(float3 Diffuse,float4 Ambient)
{
   return float4(Diffuse,Ambient.w) + Ambient;
}
#endif

//////////////////////////////////////////////////////////////////////
//section: phong - blinn and phong shading ///////////////////////////
#ifndef include_phong
#define include_phong

float DoShine(float3 LightDir,float3 Normal)
{
   return dot(normalize(LightDir),Normal);
}

float3 DoReflect(float3 inLightDir,float3 inNormal)
{
	//return normalize(2 * dot(inNormal, inLightDir) * inNormal - inLightDir);
	return -reflect(inLightDir,inNormal);
}

float3 DoPhong(float3 Diffuse, float fLight, float fHalf)
{
	return Diffuse * (saturate(fLight) * vecDiffuse.xyz + pow(saturate(fHalf),fPower) * vecSpecular.xyz);
}

float3 DoPhong(float3 Diffuse, float fLight, float fHalf, float fSpecular)
{
	return Diffuse * (saturate(fLight) * vecDiffuse.xyz + pow(saturate(fHalf),fPower) * fSpecular * vecSpecular.xyz);
}
#endif

//////////////////////////////////////////////////////////////////////
//section: bump_vs - bumpmapping vertex shader //////////////////////////
#include <define>
#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <tangent>
#include <lights>
#include <color>

struct bumpOut
{
	float4 Pos: POSITION;
	float  Fog:	FOG;
	float4 Ambient:  COLOR0;
	float4 Tex12:    TEXCOORD0;	
	float3 ViewDir:  TEXCOORD1;
	float3 LightDir1:	 TEXCOORD2;	
	float3 LightDir2:	 TEXCOORD3;
	float3 LightDir3:	 TEXCOORD4;	
	float3 Diffuse1: TEXCOORD5;		
	float3 Diffuse2: TEXCOORD6;		
	float3 Diffuse3: TEXCOORD7;		
};

bumpOut bump_VS(vertexIn In)
{
	bumpOut Out;

	Out.Pos	= DoTransform(In.Pos);
	Out.Tex12 = float4(In.Tex1,In.Tex2);
	Out.Fog	= DoFog(In.Pos);
	Out.Ambient = DoAmbient();	

	CreateTangents(In.Normal,In.Tangent);
  float3 PosWorld = DoPos(In.Pos);
	Out.ViewDir = DoTangent(vecViewPos-PosWorld);
	Out.LightDir1 = DoTangent(vecLightPos[0].xyz-PosWorld);
	Out.LightDir2 = DoTangent(vecLightPos[1].xyz-PosWorld);
	Out.LightDir3 = DoTangent(vecLightPos[2].xyz-PosWorld);
	
  float3 Normal = DoNormal(In.Normal);
	Out.Diffuse1 = DoLightFactorBump(vecLightPos[0],PosWorld,Normal) * vecLightColor[0];
	Out.Diffuse2 = DoLightFactorBump(vecLightPos[1],PosWorld,Normal) * vecLightColor[1];
	Out.Diffuse3 = DoLightFactorBump(vecLightPos[2],PosWorld,Normal) * vecLightColor[2];
	
	return Out;		
}

//////////////////////////////////////////////////////////////////////
//section: tangent_vs - tangent creating vertex shader ///////////////
#include <define>
#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <tangent_view>
#include <view>
#include <lights>
#include <color>

float4x4 matView;

struct tangentOut
{
	float4 Pos: POSITION;
	float  Fog:	FOG;
	float4 Ambient:  COLOR0;
	float3 Diffuse1: COLOR1;
	float4 Tex12: 	  TEXCOORD0;
	float3 PosView:  TEXCOORD1;
	float3 Light1:	  TEXCOORD2;
	float3 Light2:	  TEXCOORD3;
	float3 Diffuse2: TEXCOORD4;	
	float3 Tangent0: TEXCOORD5;
	float3 Tangent1: TEXCOORD6;
	float3 Tangent2: TEXCOORD7;
};

tangentOut tangent_VS(vertexIn In)
{
	tangentOut Out;

	Out.Pos = DoTransform(In.Pos);
	Out.Tex12 = float4(In.Tex1,In.Tex2);
	Out.Fog = DoFog(In.Pos);
	Out.Ambient = DoAmbient();	
	
// vertex position in world view space
  Out.PosView = DoView(In.Pos);
   
// tangent space vectors in world view space
	CreateViewTangents(In.Normal,In.Tangent);
	Out.Tangent0 = matTangent[0];
	Out.Tangent1 = matTangent[1];
	Out.Tangent2 = matTangent[2];

  Out.Light1 = mul(float4(vecLightPos[0].xyz,0),matView); // Light in view space
  Out.Light2 = mul(float4(vecLightPos[1].xyz,0),matView); 
   
  float3 PosWorld = DoPos(In.Pos);
  float3 Normal = DoNormal(In.Normal);
	Out.Diffuse1 = DoLightFactorBump(vecLightPos[0],PosWorld,Normal) * vecLightColor[0].xyz;
	Out.Diffuse2 = DoLightFactorBump(vecLightPos[1],PosWorld,Normal) * vecLightColor[1].xyz;
   
   return Out;
}

//////////////////////////////////////////////////////////////////////
//section: poisson - poisson filter //////////////////////////////////
#ifndef include_poisson
#define include_poisson

float4 vecViewPort; // contains viewport pixel size in zw components

static const int num_poisson_taps = 12;
static const float2 fTaps_Poisson[num_poisson_taps] = 
{
	{-.326,-.406},
	{-.840,-.074},
	{-.696, .457},
	{-.203, .621},
	{ .962,-.195},
	{ .473,-.480},
	{ .519, .767},
	{ .185,-.893},
	{ .507, .064},
	{ .896, .412},
	{-.322,-.933},
	{-.792,-.598}
};

float4 DoPoisson(sampler smp,float2 tex,float fDist)
{
   float4 Color = 0.;
   for (int i=0; i < num_poisson_taps; i++)
     Color += tex2D(smp,tex + vecViewPort.zw*fDist*fTaps_Poisson[i]);
   return Color/num_poisson_taps;
}

#endif

//section: box - box filter //////////////////////////////////
#ifndef include_box
#define include_box

float4 vecViewPort; // contains viewport pixel size in zw components

#define NUM_BOX_TAPS 4
static const float2 fTaps_Box[NUM_BOX_TAPS] = {
	{-1.f,-1.f},
	{ 1.f,-1.f},
	{-1.f, 1.f},
	{ 1.f, 1.f},
};

float4 DoBox2x2(sampler smp,float2 tex,float fDist)
{
   float4 Color = 0.;
   for (int i=0; i < NUM_BOX_TAPS; i++)
     Color += tex2D(smp,tex + vecViewPort.zw*fDist*fTaps_Box[i]);
   return Color * (1.f/NUM_BOX_TAPS);
}

#endif

//section: gauss - gauss filter //////////////////////////////////
#ifndef include_gauss
#define include_gauss

float4 vecViewPort; // contains viewport pixel size in zw components

#define NUM_GAUSS_TAPS 13
static const float fWeights_Gauss[NUM_GAUSS_TAPS] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};

float4 DoGauss(sampler smp,float2 tex,float2 fDist)
{
   float4 Color = 0.;
   for (int i=0; i < NUM_GAUSS_TAPS; i++)
     Color += fWeights_Gauss[i]*tex2D(smp,tex + vecViewPort.zw*fDist*(i-(NUM_GAUSS_TAPS/2)));
   return Color;
}

#endif

//////////////////////////////////////////////////////////////////////
//section: end
#endif
