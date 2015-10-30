/* render_refract.h */
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
#ifndef    _RENDER_REFRACT_H
#define    _RENDER_REFRACT_H

#define    __REFRACT

typedef struct {
	float rt_factor;
	BMAP* map_refract;
	
	BOOL queued;
} RefractState;

RefractState *RefractState_singleton = NULL;

VIEW *view_refract = { size_x = 0; size_y = 0; layer = -10; clip_near = 0; clip_far = camera->clip_far / 10; }

RefractState *render_refract_get_singleton();
void render_refract_new();
void render_refract_free();
void render_refract_setup( __In float RT );

void render_refract_set_queued( __In BOOL state );
BOOL render_refract_get_queued();

void render_refract();

#include "render_refract.c"
#endif /* render_refract.h */
