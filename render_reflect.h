/* render_reflect.h */
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
 * <render_reflect>
 * Reflection.
 * Simply there to add reflection to <render_water>.
 * For normal environment mapping, use the shaders provided in <render_surface>.
 * 
 * Authors: Wolfgang "BoH_Havoc" Reichardt (http://dotmos.org/)
 *          Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    _RENDER_REFLECT_H_
#define    _RENDER_REFLECT_H_

#define    __REFLECT

#define __In 
#define __Out 
#define __static 

typedef struct {
	float rt_factor_x;
	float rt_factor_y;
	BMAP *map_reflect;
	
	BOOL queued;
} ReflectState;

ReflectState *ReflectState_singleton = NULL;

ReflectState *render_reflect_get_singleton();
void render_reflect_new();
void render_reflect_free();
void render_reflect_setup( __In float rtx, __In float rty );

void render_reflect_set_queued( __In BOOL state );
BOOL render_reflect_get_queued();

void render_reflect();

VIEW *view_reflect = { size_x = 0; size_y = 0; layer = -11; }

#include "render_reflect.c"
#endif /* render_reflect.h */
