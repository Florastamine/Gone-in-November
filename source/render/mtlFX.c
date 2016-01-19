//---------------------------------------------------------------------
//STARTHEADER
//
//ver: 1.8
//engineReq: 7.82
//date: 080720
//
/////////////////////////////////////////
//title: Material Effects Library
// Conitec / jcl 2007
/////////////////////////////////////////
//class: ENVIR
//type: USER
//
//help: Standard Material Effects for Terrain, Water, Entities.
/// The effects are automatically activated by assigning the action.
/// Requires A7.84 Commercial or above, VS 2.0, PS 2.0, default.fx
//
//needs: default.fx
//prefix: mtl_
//idcode: 701
//
//ENDHEADER
// ---------------------------------------------------------------------

#ifndef mtlfx_c
#define mtlfx_c

#define PRAGMA_PATH "%EXE_DIR%\\code"; // fx files
#define PRAGMA_PATH "%EXE_DIR%\\templates\\images"; // default sky cube
#define PRAGMA_BIND "d3dcompiler_42.dll"; // DirectX 9.c shader compiler
char* fx_needs = "default.fx"; 	// for the WED resource window

///////////////////////////////////////////////////////////////////////
// helper functions

//function: mtl_copy
//desc: helper function for copying predefined material properties
//desc: to the mtl pointer.
//
function mtl_copy(MATERIAL* mtl_source)
{
	vec_set(mtl.ambient_blue,mtl_source.ambient_blue);
	vec_set(mtl.specular_blue,mtl_source.specular_blue);
	vec_set(mtl.diffuse_blue,mtl_source.diffuse_blue);
	vec_set(mtl.emissive_blue,mtl_source.emissive_blue);
	mtl.power = mtl_source.power;
	mtl.sun_albedo = mtl_source.sun_albedo;
}

// one time material init event
function mtl_model_init()
{
   mtl_copy(mtl_model);
   mtl.event = NULL;
}

function mtl_terrain_init()
{
   mtl_copy(mtl_terrain);
   mtl.event = NULL;
}

function mtl_sprite_init()
{
   mtl_copy(mtl_sprite);
   mtl.event = NULL;
}

function mtl_set(MATERIAL* m,ambient,diffuse,specular,emissive,power)
{
	vec_fill(m.ambient_blue,ambient);
	vec_fill(m.specular_blue,specular);
	vec_fill(m.diffuse_blue,diffuse);
	vec_fill(m.emissive_blue,emissive);
	m.power = power;
}

//function: mtl_setup
//desc: helper function for setting up shader parameters
//
function mtl_setup(v1,v2,v3,v4)
{
// copy the variables to vecSkill41
	if (v1 && !my.skill41) {
	   if (my.skill1) { my.skill41 = floatv(my.skill1); }
	   else { my.skill41 = floatv(v1); }
	}
	if (v2 && !my.skill42) {
		if (my.skill2) { my.skill42 = floatv(my.skill2); }
		else { my.skill42 = floatv(v2); }
	}
	if (v3 && !my.skill43) {
		if (my.skill3) { my.skill43 = floatv(my.skill3); }
		else { my.skill43 = floatv(v3); }
	}
	if (v4 && !my.skill44) {
		if (my.skill4) { my.skill44 = floatv(my.skill4); }
		else { my.skill44 = floatv(v4); }
	}
}

// environment mapping helper functions
BMAP* bmap_skycube;

function init_skycube()
{
	if (!bmap_skycube) {
		if (!sky_cube_level) {
//entry: Default Environment Sky Cube
//help: This sky is used when the level map does not contain a sky cube
//id: 32
			bmap_skycube = bmap_create("skycube+6.dds");
		} else {
			bmap_skycube = bmap_createblack(1536,256,24);
			bmap_blit(bmap_skycube,bmap_for_entity(sky_cube_level,0),vector(0,0,0),vector(1536,256,0));
		}
		bmap_to_cubemap(bmap_skycube);
	}
}

function mtl_envmap_view()
{
// set a user matrix that transforms camera space back to world space
	mat_set(mtl.matrix,matViewInv);
// reset the translation part of the matrix
 	mtl.matrix41 = 0;
 	mtl.matrix42 = 0;
 	mtl.matrix43 = 0;
}


function mtl_envmap_init()
{
	init_skycube();
	mtl.skin1 = bmap_skycube;
	mtl.event = mtl_envmap_view;
	set(mtl,ENABLE_VIEW);
}

////////////////////////////////////////////////////////////////////
//Shader Name: Bumpmapping - Lambert
//Material Name: mtl_bump
//Standard Model: marine.mdl
//Variable1: not used
//Variable2: not used
//Variable3: not used
//Variable4: not used
MATERIAL* mtl_bump =
{
   effect = "bump.fx";
}

//action: fx_bump
//title: Bump mapping using diffuse shading
//
//desc:
//desc: Normal map (second skin)
//
action fx_bump()
{
   my.material = mtl_bump;
}

////////////////////////////////////////////////////////////////////
//Shader Name: Bumpmapping - Blinn
//Material Name: mtl_specBump
//Standard Model: marine.mdl
//Variable1: not used
//Variable2: not used
//Variable3: not used
//Variable4: not used
MATERIAL* mtl_specBump =
{
	effect = "specBump.fx";
	specular_blue = 200;	// bright specular component
	specular_green = 200;
	specular_red = 200;
	power = 10;
	flags = PASS_SOLID;
}

//action: fx_specBump
//title: Specular bump mapping
//
//desc:
//desc: Specular shading from a normal map (second skin)
//
action fx_specBump()
{
   my.material = mtl_specBump;
}

////////////////////////////////////////////////////////////////////
//Shader Name: Toon Shading
//Material Name: mtl_toon
//Standard Model: marine.mdl
//Variable1: Shadow
//Variable2: EdgeWidth
//Variable3: Threshold
//Variable4: Smooth
MATERIAL* mtl_toon =
{
	effect = "toon.fx";
	event = mtl_model_init;
}

//action: fx_toon
//title: Cartoon shading
//
//desc:
//desc: Shades the object in two steps and paints a black line around it
//
action fx_toon()
{
   my.material = mtl_toon;
   mtl_setup(50,50,50,50);
}

////////////////////////////////////////////////////////////////////
//Shader Name: Environment Glass
//Material Name: mtl_envGlass
//Standard Model: marine.mdl
//Variable1: Rainbow
//Variable2: Texture
//Variable3: not used
//Variable4: not used
MATERIAL* mtl_envGlass =
{
	effect = "envglass.fx";
	event = mtl_envmap_init;
	flags = TRANSLUCENT;
}

//action: fx_envglass
//title: Glass shading
action fx_envGlass()
{
   my.material = mtl_envGlass;
   mtl_setup(50,50,0,0);
}

//Shader Name: Glass Bumpmapping
//Material Name: mtl_bumpGlass
//Standard Model: marine.mdl
//Variable1: Rainbow
//Variable2: Texture
//Variable3: not used
//Variable4: not used
MATERIAL* mtl_bumpGlass =
{
	effect = "envglassbump.fx";
	event = mtl_envmap_init;
	flags = TRANSLUCENT;
}

//action: fx_bumpGlass
//title: Glass shading
action fx_bumpGlass()
{
   my.material = mtl_bumpGlass;
   mtl_setup(50,50,50,50);
}

////////////////////////////////////////////////////////////////////
//Shader Name: Environment Mapping
//Material Name: mtl_envMap
//Standard Model: box.mdl
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used
MATERIAL* mtl_envMap =
{
 	event = mtl_envmap_init;
 	effect = "envMap.fx";
}

//action: fx_envmap
//title: Environment mapping
//
//desc:
//desc: Maps the level sky cube on the texture
//
action fx_envmap()
{
   my.material = mtl_envMap;
}

////////////////////////////////////////////////////////////////////
//Shader Name: Environment Bump Map
//Material Name: mtl_envBump
//Standard Model: box.mdl
//Variable1: Scale
//Variable2: Texture
//Variable3: not used
//Variable4: not used
MATERIAL* mtl_envBump =
{
	event = mtl_envmap_init;
	effect = "envBump.fx";
	flags = PASS_SOLID;
}

//action: fx_envBump
//title: Environment Bump Mapping
//
//desc:
//desc: Normal map (second skin)
//
action fx_envBump()
{
   my.material = mtl_envBump;
   mtl_setup(50,50,0,0);
}

///////////////////////////////////////////////////////////////////////
//section: Terrain

//desc: Terrain single pass / 3-texture material.
//desc: Use fx_terraintex3 to assign this material.
//
MATERIAL* mtl_terraintex3 =
{
	effect = "terraintex3.fx";
}

//action: fx_terraintex3
//title: 3-Textures Terrain
//
//skill2: ScaleSkin2 15
//help: Skin2 base texture scale, 1..50, default 15.
//skill3: ScaleSkin3 15
//help: Skin3 red texture scale, 1..50, default 15.
//skill4: ScaleSkin4 15
//help: Skin4 green texture scale, 1..50, default 15.
//desc:
/// Blends 3 textures together, according to
/// the red and green channel in the first skin.
/// The blue channel can be used for a shadow map.
/// Skin1 = Terrain RGB blend & shadow mask.
/// Skin2 = Base tiled terrain texture (grass).
/// Skin3 = Red channel masked tiled texture (sand).
/// Skin4 = Green channel masked tiled texture (rock).
/// Skin5 = Optional base texture for non-shader hardware.
/// Skin6 = Optional detail texture for non-shader hardware.
//
//Based on ideas by Thomas 'ventilator' Oppl
//and Eric 'Steempipe' Hendrickson-Lambert
//
action fx_terraintex3()
{
// use skin 5 for non-shader hardware
	if (d3d_shaderversion < 1111) {
		if (ent_skins(my) >= 5)	{ my.skin = 5; }
		return;
	}

// if entity skins are missing, replace them by standard skins
	mtl = mtl_terraintex3;
	mtl_copy(mat_terrain);
	my.material = mtl;
	if (ent_skins(my) < 4) { mtl.skin4 = bmap_create("rock.tga"); }
	if (ent_skins(my) < 3) { mtl.skin3 = bmap_create("sand.tga"); }
	if (ent_skins(my) < 2) { mtl.skin2 = bmap_create("grass.tga"); }

// copy the texture scales to vecSkill41
   mtl_setup(0,15,15,15);

	my.emask &= ~DYNAMIC;	// when all done, set terrain static
}

//desc: Terrain multipass/multitexture material.
//desc: Use fx_terraintex to assign this material.
//
MATERIAL* mtl_terraintex =
{
	effect = "terraintex.fx";
	flags = PASS_SOLID; // prevent transparency of 32 bit skins
	event = mtl_terrain_init;
}

//action: fx_terraintex
//title: Multi-Texture Terrain
//
//desc:
/// Supports an arbitrary number of 32 bit skins
/// RGB contains the tiled texture
/// Alpha contains the texture mask
//
action fx_terraintex()
{
	my.material = mtl_terraintex;
	my.emask &= ~DYNAMIC;	// when all done, set terrain static
}

////////////////////////////////////////////////////////////////////
BMAP* bmap_waterbump = NULL;

function env_water_init()
{
	init_skycube();
	if (!bmap_waterbump)
	   bmap_waterbump = bmap_to_normals(bmap_create("water.dds"),1);
	mtl.skin1 = bmap_waterbump;
	mtl.skin2 = bmap_skycube;
   mtl.flags |= TRANSLUCENT;
}

//Shader Name: Env Water Reflection
//Material Name: mtl_envWater
//Standard Model: water.hmp
//Variable1: xWind
//Variable2: yWind
//Variable3: Ripples
//Variable4: Scale

MATERIAL* mtl_envWater = // new version
{
	event = env_water_init;
	effect= "envwater.fx";
}

//action: fx_water_env
//title: Environment water reflection
//desc:
//desc: Environment reflection with ripple effect.
/// To be assigned to a water terrain, like water.hmp.
/// Skin1 contains a bump map for the water ripples.
/// If the level map contains a sky cube, it is used
/// for the reflection, otherwise a default sky is used.
/// Transparency is set through Albedo.
action fx_envWater()
{
	my.material = mtl_envWater;
   mtl_setup(50,50,50,50);
}

// old version //////////////////////////////////////////////
function water_env_init()
{
	init_skycube();
	mtl.skin2 = bmap_skycube;
}

MATERIAL* mtl_waterEnv =
{
	flags = TANGENT;
	event = water_env_init;
	effect= "waterEnv.fx";
}

//action: fx_water_env
//title: Environment water reflection
//desc:
//desc: Environment reflection with ripple effect.
/// To be assigned to a water terrain, like water.hmp.
/// Skin1 contains a bump map for the water ripples.
/// If the level map contains a sky cube, it is used
/// for the reflection, otherwise a default sky is used.
/// Transparency is set through Albedo.
/// Required: A6.50+ Pro, VS 1.1, PS 1.1
//
//skill1: WindSpeed_X 2.0
//help: Motion in X direction, default 2
//skill2: WindSpeed_Y 2.0
//help: Motion in Y direction, default 2
//skill3: Ripple 0.5
//help: Ripple strength, default 0.5
//skill4: Scale  0.5
//help: Ripple scale, default 0.5
//skill5: Blue   240
//help: Water blue tint, 0..255
//skill6: Green  200
//help: Water green tint, 0..255
//skill7: Red    200
//help: Water red tint, 0..255
//
// Based on a concept by Eric 'Steempipe' Hendrickson-Lambert
//

action fx_water_env()
{
	my.material = mtl_waterEnv;
	bmap_to_normals(bmap_for_entity(my,0),1.0);
	init_skycube();
	mtl_waterEnv.skin2 = bmap_skycube;

// copy shader parameters to vecSkill41
	if (my.skill1) { my.skill41 = floatd(my.skill1,10000); }
	else { my.skill41 = floatd(2,10000); }
	if (my.skill2) { my.skill42 = floatd(my.skill2,10000); }
	else { my.skill42 = floatd(2,10000); }

	if (my.skill3) { my.skill43 = floatv(my.skill3*0.5); }
	else { my.skill43 = floatv(0.25); }
	if (my.skill4) { my.skill44 = floatv(my.skill4*3); }
	else { my.skill44 = floatv(1.5); }

	if (my.skill5) {
		vec_set(my.blue,vector(my.skill5,my.skill6,my.skill7)); }
	else {
		vec_set(my.blue,vector(240,200,200)); }
}

///////////////////////////////////////////////////////////////////////
// Section: Mirrors (old)

//desc: mtl_water_mirror
//desc: Realtime water reflection with ripple effect.
//desc: Required: A7, VS 1.1, PS 2.0
//desc: Note: can look bad on old 3D cards (GF4 or below)
//desc: Use fx_water_mirror to assign this material
//
MATERIAL* mtl_water_mirror =
{
	effect="waterMirror.fx";
}

//desc: mtl_floor_mirror
//desc: Floor reflection.
//desc: Required: A7, VS 1.1, PS 1.1
//desc: Use fx_floor_mirror to assign this material
//
MATERIAL* mtl_floor_mirror =
{
	effect="floorMirror.fx";
}

//enable: Mirror debugging
//help: Make mirror view visible by pressing F11
//id: 22
//#define DEBUG_MIRROR

#ifdef DEBUG_MIRROR
VIEW* view_mirror = { layer = 1; }
#else
VIEW* view_mirror = { layer = -1; }  // render mirror view before camera view
#endif

BMAP* bmap_mirrortarget = NULL;
var mtlfx_mirrorvisible = 0;

//Simulate a horizontal mirror by generating
//a camera view from below through the floor
function fx_mirror()
{
// do nothing if mirror is already running
	if (bmap_mirrortarget) {
		my.material.skin2 = bmap_mirrortarget;
		return;
	}

	bmap_mirrortarget = bmap_createblack(256,256,888);
	my.material.skin2 = bmap_mirrortarget;
	view_mirror.bmap = bmap_mirrortarget;
	view_mirror.size_x = bmap_width(view_mirror.bmap);
	view_mirror.size_y = bmap_height(view_mirror.bmap);

	vec_set(view_mirror.pnormal_x,vector(0,0,1.0));	// reflect upwards
	set(view_mirror,NOSHADOW|NOCULL|PORTALCLIP);
// suppress shadows in the mirror, look through walls, and clip at portal plane

//enable: Suppress FLAG1 in mirror
//help: Don't display FLAG1 objects in the mirror views
//id: 23
	set(view_mirror,NOFLAG1);	// suppress all entities with flag1 set

//enable: Suppress particles in mirror
//help: Don't display particles in the mirror views
//id: 24
	set(view_mirror,NOPARTICLE);

//enable: Suppress shaders in mirror
//help: Don't render material effects in the mirror views
//id: 25
	set(view_mirror,NOSHADER);

	while (bmap_mirrortarget)
	{
		proc_mode = PROC_LATE;	// camera must be moved and mtlfx_mirrorvisible set before
		if (mtlfx_mirrorvisible)
		{
			set(view_mirror,SHOW);
			view_mirror.genius = camera.genius;
			view_mirror.aspect = (screen_size.x/screen_size.y)*camera.aspect; // screen aspect, independent of render target
			view_mirror.arc    = camera.arc;
			view_mirror.fog_start = camera.fog_start;
			view_mirror.fog_end   = camera.fog_end;
			view_mirror.clip_far  = camera.clip_far * 0.5;
			view_mirror.clip_near = camera.clip_near * 2;
			view_mirror.x 	   = camera.x;
			view_mirror.y 	   = camera.y;
			view_mirror.z 	   = 2*view_mirror.portal_z-camera.z;	// move the camera at its mirror position
			view_mirror.pan    = camera.pan;
			view_mirror.tilt   = -camera.tilt;	// flip the vertical camera angle
			view_mirror.roll   = -camera.roll;
		}
		else {
// switch rendering off when all mirror objects are outside the frustum
			reset(view_mirror,SHOW);
		}
		mtlfx_mirrorvisible = 0;

#ifdef DEBUG_MIRROR
		if (key_f11) {	// debugging - make mirror visible onscreen
			view_mirror.bmap = NULL;
		} else {
			view_mirror.bmap = bmap_mirrortarget;
		}
#endif
		wait(1);
	}
}

//action: fx_water_mirror
//title: Realtime water reflection
//
//skill1: WindSpeed_X 2.0
//help: Motion in X direction, default 2
//skill2: WindSpeed_Y 2.0
//help: Motion in Y direction, default 2
//skill3: Ripple 0.5
//help: Ripple strength, default 0.2
//skill4: Scale  0.5
//help: Ripple scale, default 0.5
//skill5: Blue   240
//help: Water blue tint, 0..255
//skill6: Green  200
//help: Water green tint, 0..255
//skill7: Red    200
//help: Water red tint, 0..255
//desc:
/// Floor reflection.
/// To be assigned to a floor entity in the level.
/// Reflection factor is set through Albedo.
/// Required: A7, VS 1.1, PS 1.1
/// Note: can look bad on 3D cards w/o clip planes (GF4 or below)
//
action fx_water_mirror()
{
	my.material = mtl_water_mirror;
	set(me,TRANSLUCENT);
	BMAP* bump = bmap_for_entity(my,0);
	if (bump) bmap_to_uv(bump);
	fx_mirror();	// activate mirror view

// copy shader parameters to vecSkill41
	if (my.skill1) { my.skill41 = floatd(my.skill1,10000); }
	else { my.skill41 = floatd(2,10000); }
	if (my.skill2) { my.skill42 = floatd(my.skill2,10000); }
	else { my.skill42 = floatd(2,10000); }
	if (my.skill3) { my.skill43 = floatv(my.skill3*0.1); }
	else { my.skill43 = floatv(0.2*0.1); }
	if (my.skill4) { my.skill44 = floatv(my.skill4); }
	else { my.skill44 = floatv(0.5); }

	if (my.skill5) {
		vec_set(my.blue,vector(my.skill5,my.skill6,my.skill7)); }
	else {
		vec_set(my.blue,vector(240,200,200)); }

// update mirror when not clipped away
	while(1)
	{
		if (!is(my,CLIPPED))
		{
			vec_for_max(view_mirror.portal_x,my);
			vec_add(view_mirror.portal_x,my.x);
			mtlfx_mirrorvisible = 1;
		}
		wait(1);
	}
}

//action: fx_floor_mirror
//title: Realtime floor reflection
//
//skill5: Blue   240
//help: Floor blue tint, 0..255
//skill6: Green  200
//help: Floor green tint, 0..255
//skill7: Red    200
//help: Floor red tint, 0..255
//desc:
/// Realtime floor reflection
/// To be assigned to a floor block or terrain.
/// Transparency is set through Albedo.
/// Required: A7.82, VS 1.1, PS 1.1
/// Note: can look bad on 3D cards w/o clip planes (GF4 or below)
//
action fx_floor_mirror()
{
	my.material = mtl_floor_mirror;
	fx_mirror();	// activate mirror view

// update mirror when not clipped away
	while(1)
	{
		if (!is(my,CLIPPED))
		{
			vec_for_max(view_mirror.portal_x,my);
			vec_add(view_mirror.portal_x,my.x);
			mtlfx_mirrorvisible = 1;
		}
		wait(1);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// Section: Mirrors (new)

BMAP* bmap_water_uv = NULL;

function mirror_init()
{
   fx_mirror();   // create a mirror view, set mirror target to mtl.skin2
	while (1)
	{
		if (!is(my,CLIPPED)) {
// place the mirror plane at the topmost entity polygon
			vec_for_max(view_mirror.portal_x,my);
			vec_add(view_mirror.portal_x,my.x);
			mtlfx_mirrorvisible = 1;
		}
		wait(1);
	}
}

function mirror_water_init()
{
	if (!bmap_water_uv)
	   bmap_water_uv = bmap_to_uv(bmap_create("water.dds"));
   mtl.skin1 = bmap_water_uv;
   mtl.flags |= TRANSLUCENT;
	mirror_init();	// activate mirror view
}

//Shader Name: Water Realtime Mirror
//Material Name: mtl_mirrorWater
//Standard Model: water.hmp
//Variable1: xWind
//Variable2: yWind
//Variable3: Ripples
//Variable4: Scale

MATERIAL* mtl_mirrorWater =
{
	effect = "mirrorWater.fx";
	event = mirror_water_init;
}

//action: fx_mirrorWater
//title: Water Realtime Mirror
//
action fx_mirrorWater()
{
	my.material = mtl_mirrorWater;
   mtl_setup(50,50,50,50);
}

//Shader Name: Floor Mirror
//Material Name: mtl_mirrorFloor
//Standard Model: water.hmp
//Variable1: not used
//Variable2: not used
//Variable3: not used
//Variable4: not used

MATERIAL* mtl_mirrorFloor =
{
	effect = "mirrorFloor.fx";
	event = mirror_init;
}

//action: fx_mirrorFloor
//title: Floor Realtime Mirror
//
action fx_mirrorFloor()
{
	my.material = mtl_mirrorFloor;
   mtl_setup(50,50,50,50);
}

////////////////////////////////////////////////////////////////////
//Shader Name: Turbulence
//Material Name: mtl_turbulence
//Standard Model: water.hmp
//Variable1: Amplitude
//Variable2: Speed
//Variable3: Modulation
//Variable4: Not used
MATERIAL* mtl_turbulence =
{
   effect = "turbulence.fx";
	event = mtl_terrain_init;
}

//action: fx_turbulence
//title: Turbulence texture
action fx_turbulence()
{
	my.material = mtl_turbulence;
   mtl_setup(50,50,50,0);
}

////////////////////////////////////////////////////////////////////
//Shader Name: Tree Wind Animation
//Material Name: mtl_animTree
//Standard Model: marine.mdl
//Variable1: x Wind
//Variable2: y Wind
//Variable3: Oscillation speed
//Variable4: not used
MATERIAL* mtl_animTree =
{
	effect= "animTree.fx";
	event = mtl_model_init;
}

//action: fx_animTree
//title: Tree Wind Animation
//
action fx_animTree()
{
   my.material = mtl_animTree;
   mtl_setup(50,50,50,50);
}

////////////////////////////////////////////////////////////////////
//Shader Name: Wireframe
//Material Name: mtl_wireframe
//Standard Model: marine.mdl
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used
MATERIAL* mtl_wireframe =
{
   effect = "technique { pass { Fillmode = Wireframe; } }";
   event = mtl_model_init;
}

////////////////////////////////////////////////////////////////////
//Shader Name: Vertex Light Count
//Material Name: mtl_lightcount
//Standard Model: marine.mdl
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used
MATERIAL* mtl_lightcount =
{
   effect = "lightCount.fx";
   event = mtl_model_init;
}

MATERIAL* mtl_nomipmap =
{
   effect = "technique { pass { MipFilter[0] = None; } }";
   event = mtl_model_init;
}

MATERIAL* mtl_twosided =
{
	effect = "technique { pass { CullMode = None; } }";
	event = mtl_model_init;
}

MATERIAL* mtl_twosided_alpha =
{
	effect = "technique { pass { CullMode = None; AlphaTestEnable = True; ZWriteEnable = true;} }";
	event = mtl_model_init;
}

function mtl_norender() { return 1; }

MATERIAL* mtl_invisible =
{
   event = mtl_norender;
   flags = ENABLE_RENDER;
}

// default material, i.e. to assign to a decal for default rendering
MATERIAL* mtl_default =
{
	event = mtl_terrain_init;
	effect = "technique fallback { pass one { } }";
}

////////////////////////////////////////////////////////////////////
#endif	// mtlfx_c
