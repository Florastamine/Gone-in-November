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
 * 
 * __________________________________________________________________
 * 
 * <render_lrays>
 * Light rays.
 * To use the effect, firstly make a call to render_light_rays_new() to 
 * allocate some memory for the adjustable parameters. 
 * Call render_light_rays_setup() to re-define the ray strength and length.
 * Finally, call render_light_rays_setup() to directly render the light rays, 
 * or put it to queue with render_light_rays_set_queued() and later call 
 * render_queue_start() to perform rendering.
 * 
 * Authors: Wolfgang "BoH_Havoc" Reichardt (http://dotmos.org/)
 *          Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    A8_FREE

#ifndef    __LRAYS_H_
#define    __LRAYS_H_

#define PRAGMA_PRINT "[Light rays] "

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
PANEL *__sun_dummy = { scale_x = 0.5; scale_y = 0.5; }

#include "render_lrays.c"
#endif /* render_lrays.h */
#endif
