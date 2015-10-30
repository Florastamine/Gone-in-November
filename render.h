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
 */
#ifndef    _RENDER_H_
#define    _RENDER_H_

#define __In 
#define __Out 
#define __static 

#define __namespace(namespace)             {}

#define ATTRIBUTE_DEPTH                    1
#define ATTRIBUTE_TEAM                     2
#define ATTRIBUTE_LUMINANCE                3
#define ATTRIBUTE_VOLPART                  4
#define ATTRIBUTE_TEXMOV                   5
#define ATTRIBUTE_REFRACTION_COL           6

__namespace(render) {
	typedef struct {
		float depth;
		float volpart;
		float texmov;
		
		VECTOR *luminance;
		VECTOR *refraction;
		VECTOR *team;
	} Attribute;
	
	typedef struct {	
		BMAP *sc_map_scene;
		BMAP *sc_map_depth;
		BMAP *sc_bmap_dof;
		BMAP *sc_bmap_gamma;
		
		VIEW *sc_view_last;
		
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
