/******************************************************************************************************
Toon-Shader by Wolfgang "BoH_Havoc" Reichardt

Entity Textures:
	Skin1 = Colormap 
	Skin2 = Normalmap (RGB) + Hatchmap (A)

Material Textures:
	Skin1 = Toon Lookupmap (not used yet)
	
Usage:
	Uncomment/Comment the #defines to add/remove an effect.
	vecSkill1.x sets the diffuse shadow alpha (mtl.skill1)
	vecSkill1.y sets the specular power (mtl.skill2)
	
******************************************************************************************************/

/***************************************TWEAKABLES*****************************************************/

// Default Values (will be used if vecSkill1, vecSkill5 or vecSkill9 hasn't been set): 
float specularExp = 30; // vecSkill1.y | sharpness of the specular lighting
float shadowAlpha = 0.5; // range: 0-1 | 0 = pure black shadow | 1 = no shadow at all
//

//misc
float outlineStr = 0.15;
float colorRamp = 2; //sets the colorramp, low values result in a more cartoony look
float bumpStrength = 2; // bumpmapping strength
float hatchTile = 10; //tilingof hatch texture
//

// UNCOMMENT/COMMENT TO ACTIVATE/DEACTIVATE

//Use Diffuse Lighting
#define DIFFLIGHT

//Use Specular Lighting
//#define SPECLIGHT

//Use Normalmapping
//#define NORMALMAPPING

//Use Hatching Effect
//#define HATCHING

//Create black outline
//#define OUTLINE

//Above Effects will react to sunlight (only needed if you have dynamic lights in your level)
//#define DOSUN

//Diffuse Lighting will react to 5 additional dynamic lights including the sun
//#define DYNLIGHTS

//Use PixelShader 2.a | set this if you get an errormessage ("code uses too many many arithmetic instruction slots")
#define PS2A


/***************************************SHADER*CODE****************************************************/

// Matrices
float4x4 matWorldViewProj;
float4x4 matWorldInv;
float4x4 matViewInv;
float4x4 matWorld;

// Passed by the engine
float4 vecTime;
float4 vecFog;
float4 vecViewPos;
float4 vecViewDir;
float4 vecLight;
float4 vecSunPos;
float4 vecSunColor;
float4 vecLightPos[8];
float4 vecLightColor[8];
float4 vecSkill1;
float4 vecSkill5;
float4 vecSkill9;

//Textures
texture entSkin1; // Colormap
texture entSkin2; // Normalmap
texture mtlSkin1; // Lookup-Map

// Color map sampler
sampler ColorMapSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

//NormalMap
sampler NormalMapSampler = sampler_state 
{ 
   Texture = <entSkin2>;
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler1D lookupSampler = sampler_state 
{ 
   Texture = <mtlSkin1>;
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

 
 
// Vertex Shader
void DiffuseVS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0,
   in float3 InTangent : TEXCOORD0,
   in float2 InShadow : TEXCOORD1,
   out float4 OutPos: POSITION, 
   out float4 OutTex: TEXCOORD0, 
   out float4 OutLight: TEXCOORD1,
   out float3 OutViewDir: TEXCOORD2,
   out float3 OutWorldNormal	: TEXCOORD3,
	out float3 OutWorldTangent	: TEXCOORD4,
	out float3 OutWorldBinorm	: TEXCOORD5,
	out float3 OutWorldPos	: TEXCOORD6,
   out float OutFog : FOG) 
{ 
	// Transform the vertex from object space to clip space: 
   OutPos = mul(InPos, matWorldViewProj); 
	// Pass the texture coordinate to the pixel shader: 
   OutTex.xy = InTex;
   // Pass the shadow coordinate to the pixel shader: 
   OutTex.zw = InShadow;
   
   float3 PosWorld = mul(InPos, matWorld);
  	
   //Light
   #ifdef DOSUN
   OutLight.xyz = vecSunPos - PosWorld;;
   OutLight.w = 100000;//distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
   #else
   OutLight.xyz = vecLightPos[0] - PosWorld;
   OutLight.w = 0;
   if(vecLightPos[0].w < 100000) OutLight.w = 1-distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
   #endif
  	//

	//Specular Lighting
	#ifdef DOSUN
	OutViewDir = (vecSunPos) - mul(InPos, matWorld);
	#else
	OutViewDir = (vecLightPos[0]) - mul(InPos, matWorld); 
	#endif
	//
	
	//Environment Mapping
	float3 Binormal = cross(InNormal,InTangent);
	OutWorldNormal.xyz = mul(InNormal, matWorld).xyz;
	OutWorldTangent.xyz = mul(InTangent, matWorldInv).xyz;
	OutWorldBinorm.xyz = mul(Binormal, matWorldInv).xyz;
	OutWorldPos = PosWorld;
	//
	
	//Fog
	OutFog = 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z);
   //
}

// Pixel Shader: 
float4 DiffusePS( 
   in float4 InTex: TEXCOORD0, 
   in float4 InLight : TEXCOORD1,
   in float4 InViewDir: TEXCOORD2,
   in float3 InWorldNormal	: TEXCOORD3,
	in float3 InWorldTangent	: TEXCOORD4,
	in float3 InWorldBinorm	: TEXCOORD5,
	in float3 InWorldPos	: TEXCOORD6
): COLOR 
{ 
	float3 Ln = normalize(InLight.xyz);
   float3 Nn = normalize(InWorldNormal);
   float3 Tn = normalize(InWorldTangent);
   float3 Bn = normalize(InWorldBinorm);
   float3 Nb = 0;
   float3 Vn = 0;
   float3 Hn = 0;
   float4 lighting = 0;

	//check if vecSkill1 has been set. If not, use default values
	#ifdef DIFFLIGHT
		if(vecSkill1.x == 0) vecSkill1.x = shadowAlpha;
	#endif
	#ifdef SPECLIGHT
		if(vecSkill1.y == 0) vecSkill1.y = specularExp;
	#endif
 
 	//get color
 	float4 Color = tex2D(ColorMapSampler,InTex.xy); 
   
   float3 Light1 = Color.xyz;
   
   float3 bumpNormal = 0;
   #ifdef NORMALMAPPING
   	bumpNormal= bumpStrength * (tex2D(NormalMapSampler,InTex.xy).xyz-(0.5).xxx);
   #endif
   
   #ifdef DIFFLIGHT
   	Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   	Nb = normalize(Nb);
   	Vn = normalize(InViewDir);
   	Hn = normalize(Vn + Ln);
   	lighting = lit(dot(Ln,Nb),dot(Hn,Nb),vecSkill1.y);
   #else
   	#ifdef SPECLIGHT
   		Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   		Nb = normalize(Nb);
   		Vn = normalize(InViewDir);
   		Hn = normalize(Vn + Ln);
   		lighting = lit(dot(Ln,Nb),dot(Hn,Nb),vecSkill1.y);
   	#else
   		#ifdef HATCHING
   			Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   			Nb = normalize(Nb);
   			Vn = normalize(InViewDir);
   			Hn = normalize(Vn + Ln);
   			lighting = lit(dot(Ln,Nb),dot(Hn,Nb),vecSkill1.y);
   		#endif
   	#endif
   #endif
   
     //float4 gooch = lerp(float4(0,1,0,1), float4(0,0,1,1) , 1+dot(Nn,Ln)*0.95); //Gooch Shading
      
   #ifdef DIFFLIGHT
   #ifdef DOSUN
   	Light1.xyz *= round(clamp((lighting.y*vecSunColor*2),shadowAlpha,1)*colorRamp)/colorRamp;
   #else
   	Light1.xyz *= round(clamp((lighting.y*vecLightColor[0]*2*InLight.a),shadowAlpha,1)*colorRamp)/colorRamp;
   #endif
   #endif
   
   #ifdef SPECLIGHT
   	#ifdef DOSUN
     		Light1.xyz += round(lighting.z*colorRamp)/colorRamp;
     	#else
     		Light1.xyz += round((lighting.z*InLight.a)*colorRamp)/colorRamp;
     	#endif
   #endif
   
   
   #ifdef HATCHING
   
   	float2 Tile = InTex.xy*hatchTile;
		Tile = frac(Tile)*(float2(0.5,0.5)-2*0.004)+0.004;
		
		float hatchFactor = lighting.y * 4.0;
		float3 weight0 = 0.0;
		float3 weight1 = 0.0;
		if (hatchFactor>3.0) { weight0.x = 1.0; }
		else if (hatchFactor>2.0)
		{
			weight0.x = 1.0 - (3.0 - hatchFactor);
			weight0.y = 1.0 - weight0.x;
		}
		else if (hatchFactor>1.0)
		{
			weight0.y = 1.0 - (2.0 - hatchFactor);
			weight0.z = 1.0 - weight0.y;
		}
		else if (hatchFactor>0.0)
		{
			weight0.z = 1.0 - (1.0 - hatchFactor);
			weight1.x = 1.0 - weight0.z;
		}
		float4 hatchTex0 = tex2D(NormalMapSampler,Tile).a * weight0.x;
		float4 hatchTex1 = tex2D(NormalMapSampler,Tile+float2(0.5, 0.0)).a * weight0.y;
		float4 hatchTex2 = tex2D(NormalMapSampler,Tile+float2(0.0, 0.5)).a * weight0.z;
		float4 hatchTex3 = tex2D(NormalMapSampler,Tile+float2(0.5, 0.5)).a * shadowAlpha;
		float4 hatchColor = hatchTex0 + hatchTex1 + hatchTex2 + hatchTex3;
   	Light1.xyz *= hatchColor.xyz;
		
   #endif
      
   #ifdef OUTLINE
   	Vn = normalize(matViewInv[3].xyz - InWorldPos);
   	half3 velvety1;
   	float vdn = outlineStr-dot(Vn,Nn);
   	float3 vecColor = float4(vdn.xxx,1.0);
   	velvety1 = float4((vecColor).xyz,1);
   	velvety1 = clamp(velvety1,0,1);
   	Light1 *= pow(1-velvety1,400);
   #endif
   
   return float4(Light1,Color.a);
} 



//----------------------------------------------------------------------------------------------------------------
// LIGHTS
//----------------------------------------------------------------------------------------------------------------

struct VS_LIGHTS0
{
    float4 Pos  : POSITION;

    float4 Light1 : TEXCOORD0;
    float4 Light2 : TEXCOORD1;
    float4 Light3 : TEXCOORD2;
    float4 Light4 : TEXCOORD3;
    float4 Light5 : TEXCOORD4;
    
    float2 Tex : TEXCOORD5;
    float Fog : FOG;
};

VS_LIGHTS0 LightsVS(float4 Pos : POSITION, float3 texcoord0 : TEXCOORD0, float3 Normal : NORMAL, float3 Tangent : TEXCOORD0  )
{
    VS_LIGHTS0 Out = (VS_LIGHTS0)0;
    Out.Pos = mul(Pos, matWorldViewProj);	// transform Position
    
    // compute the 3x3 tranform matrix 
    // to transform from world space to tangent space
    float3x3 worldToTangentSpace;
    worldToTangentSpace[0] = mul(Tangent, matWorld);
    worldToTangentSpace[1] = mul(cross(Tangent, Normal), matWorld);
    worldToTangentSpace[2] = mul(Normal, matWorld);
        
    float3 PosWorld = mul(Pos, matWorld);
	
    //Light1	 
    float3 Light1 = PosWorld - vecLightPos[0].xyz  ; 
    Out.Light1.xyz = normalize(mul(worldToTangentSpace, -Light1));
    Out.Light1.xyz = saturate(dot(1, Out.Light1.xyz));
    Out.Light1.w = distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
    Out.Light1.xyz *= 1-saturate(dot(Out.Light1.w, Out.Light1.w));
    
    //Light2	 
    float3 Light2 = PosWorld - vecLightPos[1].xyz  ; 
    Out.Light2.xyz = normalize(mul(worldToTangentSpace, -Light2));
    Out.Light2.xyz = saturate(dot(1, Out.Light2.xyz));
    Out.Light2.w = distance(PosWorld,vecLightPos[1])/vecLightPos[1].w;
    Out.Light2.xyz *= 1-saturate(dot(Out.Light2.w, Out.Light2.w));
    
    //Light3	 
    float3 Light3 = PosWorld - vecLightPos[2].xyz  ; 
    Out.Light3.xyz = normalize(mul(worldToTangentSpace, -Light3));
    Out.Light3.xyz = saturate(dot(1, Out.Light3.xyz));
    Out.Light3.w = distance(PosWorld,vecLightPos[2])/vecLightPos[2].w;
    Out.Light3.xyz *= 1-saturate(dot(Out.Light3.w, Out.Light3.w));
    
    //Light4	 
    float3 Light4 = PosWorld - vecLightPos[3].xyz  ; 
    Out.Light4.xyz = normalize(mul(worldToTangentSpace, -Light4));
    Out.Light4.xyz = saturate(dot(1, Out.Light4.xyz));
    Out.Light4.w = distance(PosWorld,vecLightPos[3])/vecLightPos[3].w;
    Out.Light4.xyz *= 1-saturate(dot(Out.Light4.w, Out.Light4.w));
    
    //Light5	 
    float3 Light5 = PosWorld - vecLightPos[4].xyz  ; 
    Out.Light5.xyz = normalize(mul(worldToTangentSpace, -Light5));
    Out.Light5.xyz = saturate(dot(1, Out.Light5.xyz));
    Out.Light5.w = distance(PosWorld,vecLightPos[4])/vecLightPos[4].w;
    Out.Light5.xyz *= 1-saturate(dot(Out.Light5.w, Out.Light5.w));
    
    float ofog = 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z);
    Out.Fog = ofog;
    
    Out.Tex = texcoord0.xy;
    
    return Out;
}

struct PS_LIGHTS0
{
    float3 Light1 : TEXCOORD0;
    float3 Light2 : TEXCOORD1;
    float3 Light3 : TEXCOORD2;
    float3 Light4 : TEXCOORD3;
    float3 Light5 : TEXCOORD4;
    
    float2 Tex : TEXCOORD5;
};

float4 LightsPS( float4 Light1 : TEXCOORD0,
    float4 Light2 : TEXCOORD1,
    float4 Light3 : TEXCOORD2,
    float4 Light4 : TEXCOORD3,
    float4 Light5 : TEXCOORD4,
    float2 Tex : TEXCOORD5
     ):COLOR0
{
	float3 color = tex2D(ColorMapSampler,Tex);

    float4 shadow1 = Light1;
    float4 shadow2 = Light2;
    float4 shadow3 = Light3;
    float4 shadow4 = Light4;
    float4 shadow5 = Light5;
 
    float3 result =
   	(shadow1*vecLightColor[0])+
   	(shadow2*vecLightColor[1])+
   	(shadow3*vecLightColor[2])+
   	(shadow4*vecLightColor[3])+
   	(shadow5*vecLightColor[4]);
   
   result *= color;     
   
//-------------------------------------------
//Finalize
//-------------------------------------------

    return float4(result,1.0);

}



 
technique t01
{ 

   pass P0 
   {
		VertexShader = compile vs_2_0 DiffuseVS();
   	#ifdef PS2A
      	PixelShader  = compile ps_2_a DiffusePS();
      #else
      	PixelShader  = compile ps_2_0 DiffusePS();
      #endif
   }
 	
 	#ifdef DYNLIGHTS
   pass P1 //lights
    {
    	
    	 alphablendenable=true;
   	 srcblend=one;
    	 destblend=one;
       VertexShader = compile vs_1_0 LightsVS();
       PixelShader  = compile ps_1_4 LightsPS();
    }
    #endif
    
}