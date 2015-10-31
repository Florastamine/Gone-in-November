/* render_water.h */
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
#ifndef    _RENDER_WATER_H_
#define    _RENDER_WATER_H_

#define    __WATER

#define __In 
#define __Out 
#define __static 

typedef struct {
	float depth;
	BMAP *fresnel;	
} WaterState;

WaterState *WaterState_singleton = NULL;

WaterState *render_water_get_singleton();

void render_water_new();
void render_water_free();
void render_water_set_depth( __In float depth );

void render_water( __In ENTITY *entity );

MATERIAL *mtl_water = { flags = ENABLE_RENDER | TANGENT; effect = "water.fx"; }

#include "render_water.c"
#endif /* render_water.h */
