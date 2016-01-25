/* render.h */
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
 * <render>
 * A collection of extra rendering effects which enhances the stock 
 * Gamestudio renderer, containing high dynamic rendering, depth of field, 
 * light rays, water reflection and refraction.
 * Preceding modules must include the base header <render> in order to use the shaders.
 * 
 * Authors: Wolfgang "BoH_Havoc" Reichardt (http://dotmos.org/)
 *          Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    A8_FREE

#ifndef    _RENDER_H_
#define    _RENDER_H_

#ifndef    d3d9_h
#include   <d3d9.h>
#endif

#define PRAGMA_PRINT "\nRendering module initialization..."

#define __In 
#define __Out 
#define __static 

#define __namespace(namespace)             {}

#define ATTRIBUTE_DEPTH                    1
#define ATTRIBUTE_VOLPART                  2
#define ATTRIBUTE_TEXMOV                   3

__namespace(render) {
	typedef struct {
		float depth;
		float volpart;
		float texmov;
	} Attribute;
	
	typedef struct {	
		BMAP *map_scene;
		BMAP *map_depth;
		BMAP *bmap_dof;
		BMAP *bmap_gamma;
		
		VIEW *view_last;
		
		BOOL ready;
		BOOL rt;
	} RenderState;
	
	RenderState *RenderState_singleton = NULL;
	
	RenderState *RenderState_get_singleton();
	void render_new();
	void render_free();
	void render_setup_rt();
	void render_reset_rt();
	BOOL render_is_rt();
	
	Attribute *attribute_new();
	void attribute_free( __In __Out Attribute *attribute );
	void render_attribute_setup( __In ENTITY *object, __In const int mode, __In int value );
	
	VIEW *shader_pp_add( __In MATERIAL *material, __In VIEW *view, __In BMAP *bmap );
	void shader_pp_remove( __In MATERIAL *material, __In VIEW *view, __In VIEW *stage_view );
}

#include "render.c" 
#endif /* render.h */

#else
#define PRAGMA_PRINT "\n(Warning!) Running with A8_FREE enabled. Suppress rendering of shaders."
#endif
