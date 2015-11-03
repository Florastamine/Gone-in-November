/* render_shadow.h */
/*
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                     Version 2, December 2004
 * 
 *  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 * 
 *  Everyone is permitted to copy and distribute verbatim or modified
 *  copies of this license document, and changing it is allowed as long
 *  as the name is changed.
 *
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 * 
 *   0. You just DO WHAT THE FUCK YOU WANT TO.
 */
#ifndef    _SHADOW_H_
#define    _SHADOW_H_

#define __SHADOW

#define __In 
#define __Out 
#define __static 

typedef struct {
	BMAP *map_shadow_alpha;
	BMAP *map_shadow_depth_sun;
	
	MATERIAL *mtl_shadow_sun;
	
	float ortho_matrix[16];
	float ortho_light_pos[16];
	float sun_light_view[16];
	
	VIEW *blur_depth_view;
	VIEW *shadow_depth_view;
	
	float resolution;
	float resolution_range;
	float depth;
	float blurness;
	
	BOOL queued;
} ShadowState;

ShadowState *ShadowState_singleton = NULL;

__static void mtl_shadowDepthSun_event();

ShadowState *render_shadow_get_singleton();
void render_shadow_new();
void render_shadow_setup();
void render_shadow_setup( __In float resolution, __In float resolution_range, __In float depth, __In float blurness);
void render_shadow();
void render_shadow_free();
void render_shadow_set_queued( __In BOOL state );
BOOL render_shadow_get_queued();

ENTITY *__the_sun = { type = "__sun.mdl"; }

MATERIAL *mtl_shadowDepth = { effect = "vsmDepth.fx"; }
/* MATERIAL *__mtl_shadow = { effect = "vsm.fx"; } */
MATERIAL *mtl_shadowDepthSun = { effect = "vsmDepthSun3.fx"; }
MATERIAL *mtl_shadowSunAdd = { effect = "vsmSunAdd.fx"; }
MATERIAL *mtl_shadowBlur = { effect = "vsmSunBlur.fx"; }
MATERIAL *mtl_shadow_sun = {  effect = "vsmSun.fx"; }

#include "render_shadow.c"
#endif /* render_shadow.h */
