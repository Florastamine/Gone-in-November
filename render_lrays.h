/* render_lrays.h */
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
#ifndef    __LRAYS_H_
#define    __LRAYS_H_

#define __GOD_RAYS

#define __In 
#define __Out 
#define __static 

typedef struct {
	float ray_strength;
	float ray_length;
	
	BOOL queued;
} __LRaysState; // The double __ denoted that this should be *only* a struct with strictly one instance.

__LRaysState *__LRays_singleton = NULL;

__LRaysState *render_light_rays_get_singleton();
void render_light_rays_new();
void render_light_rays_free();
void render_light_rays_setup( __In float strength, __In float length );
void render_light_rays_set_queued( __In BOOL state );
BOOL render_light_rays_get_queued();

void render_light_rays_set_debug();
void render_light_rays_reset_debug();
BOOL render_light_rays_get_debug();

void render_light_rays();

MATERIAL *lray = { effect = "lightRayShow.fx"; }
PANEL *__sun_dummy = { bmap = "sun.tga"; scale_x = 0.5; scale_y = 0.5; }

#include "render_lrays.c"
#endif /* render_lrays.h */
