/* render.c */
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

/*
 * RenderState *RenderState_get_singleton()
 * 
 * Returns the singleton of the render state object.
 */
RenderState *RenderState_get_singleton()
{
	return RenderState_singleton;
}

/*
 * void render_new()
 * 
 * Allocates a new render state.
 * 
 */
void render_new()
{
	RenderState_singleton = (RenderState *) malloc(sizeof(RenderState) + 4 * sizeof(BMAP) + sizeof(VIEW));
	
	RenderState_singleton->sc_map_scene = NULL;
	RenderState_singleton->sc_map_depth = NULL;
	RenderState_singleton->sc_bmap_dof = NULL;
	RenderState_singleton->sc_bmap_gamma = bmap_create("#1x1x32");
	
	RenderState_singleton->sc_view_last = NULL;
	
	RenderState_singleton->ready = true;
}

void render_free()
{
	if(RenderState_singleton) free(RenderState_singleton);
}

Attribute *attribute_new()
{
	Attribute *attribute = (Attribute *) malloc(sizeof(Attribute));
	
	attribute->luminance = vector(0.0, 0.0, 0.0);
	attribute->refraction = vector(1.0, 1.0, 1.0);
	attribute->team = vector(1.0, 1.0, 1.0);
	attribute->depth = 0;
	attribute->volpart = 0;
	attribute->texmov = 0;
	
	return attribute;
}

void attribute_free( __In __Out Attribute *attribute )
{
	if(attribute) free(attribute);
}

void render_attribute_setup( __In ENTITY *object, __In const int mode, __In int value )
{
	Attribute *attribute = attribute_new();
	if(object)
	{
		if(object->skill99) attribute = (Attribute *) object->skill99; // Restore old data from the object, if there is any.
	}
	
	switch(mode)
	{
		case ATTRIBUTE_DEPTH:
		    attribute->depth = value;
		    break;
		
		case ATTRIBUTE_TEAM:
		    vec_set(attribute->team, vec_fill(nullvector, value / 255));
		    break;
		
		case ATTRIBUTE_LUMINANCE:
		    vec_set(attribute->luminance, vec_fill(nullvector, value));
		    attribute->luminance->x /= 255;
		    attribute->luminance->y /= 255;
		    attribute->luminance->z /= 255;
		    break;
		
		case ATTRIBUTE_VOLPART:
		    attribute->volpart = value;
		    attribute->depth = 2.0; // Clip from depth map.
		    break;
		
		case ATTRIBUTE_TEXMOV:
		    attribute->texmov = value;
		    break;
		
		case ATTRIBUTE_REFRACTION_COL:
		    vec_set(attribute->luminance, vec_fill(nullvector, value / 255));
		    break;
		
		default: return;
	}
	
	if(object) object->skill99 = attribute;
	attribute_free(attribute);
}

void render_setup_rt()
{
	(RenderState_get_singleton())->sc_map_scene = bmap_createblack(screen_size.x, screen_size.y,32);
	camera->bmap = (RenderState_get_singleton())->sc_map_scene;
	(RenderState_get_singleton())->rt = true;
}

void render_reset_rt()
{
	camera->bmap = NULL;
	(RenderState_get_singleton())->rt = false;
}

BOOL render_is_rt()
{
	return (RenderState_get_singleton())->rt;
}
