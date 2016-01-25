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
 * Useful for screen-space shaders, like high dynamic rendering and depth of field.
 * 
 * Not required when using surface shaders.
 */
void render_new()
{
	RenderState_singleton = (RenderState *) sys_malloc(sizeof(RenderState) + 4 * sizeof(BMAP) + sizeof(VIEW));
	
	RenderState_singleton->map_scene = NULL;
	RenderState_singleton->map_depth = NULL;
	RenderState_singleton->bmap_dof = NULL;
	RenderState_singleton->bmap_gamma = bmap_create("#1x1x32");
	
	RenderState_singleton->view_last = NULL;
	
	RenderState_singleton->ready = true;
}

/*
 * void render_free()
 * 
 * Frees the render state singleton.
 * You should not arbitrary free the render state (as it contains data required for screen-space effects as noted above on render_new()).
 * However, render state is not required (and thus can bypassed) when using object-based shaders.
 */
void render_free()
{
	if(RenderState_singleton) sys_free(RenderState_singleton);
}

/* 
 * Attribute *attribute_new()
 * 
 * Allocates a new object attribute. 
 * Attributes allocated this way must be freed later with attribute_free()/free().
 */
Attribute *attribute_new()
{
	Attribute *attribute = (Attribute *) sys_malloc(sizeof(Attribute));
	
	attribute->depth = 0;
	attribute->volpart = 0;
	attribute->texmov = 0;
	
	return attribute;
}

/*
 * void attribute_free( Attribute *attribute )
 * 
 * Frees an object attribute that was allocated before with attribute_new()/MALLOC().
 */
void attribute_free( Attribute *attribute )
{
	if(attribute) sys_free(attribute);
}

/*
 * void render_attribute_setup( ENTITY *object, const int mode, int value )
 * 
 * Sets up a shader attribute for an object. 
 */
void render_attribute_setup( ENTITY *object, const int mode, int value )
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
		
		case ATTRIBUTE_VOLPART:
		    attribute->volpart = value;
		    attribute->depth = 2.0; // Clip from depth map.
		    break;
		
		case ATTRIBUTE_TEXMOV:
		    attribute->texmov = value;
		    break;
		
		default: return;
	}
	
	if(object) object->skill99 = attribute;
	attribute_free(attribute);
}

/*
 * void render_setup_rt()
 * 
 * Sets up scene RT. 
 * This is required for using screen-space effects such as HDR.
 * Must be called prior to invoking HDR/DOF functions, and they 
 * must be enabled together. 
 * 
 * Not required by surface shaders.
 */
void render_setup_rt()
{
	(RenderState_get_singleton())->map_scene = bmap_createblack(screen_size.x, screen_size.y,32);
	camera->bmap = (RenderState_get_singleton())->map_scene;
	(RenderState_get_singleton())->rt = true;
}

/*
 * void render_reset_rt()
 * 
 * Resets the scene RT.
 */
void render_reset_rt()
{
	camera->bmap = NULL;
	(RenderState_get_singleton())->rt = false;
}

/*
 * BOOL render_is_rt()
 * 
 * Returns true if scene RT is active, false otherwise.
 */
BOOL render_is_rt()
{
	return (RenderState_get_singleton())->rt;
}

/*
 * void shader_pp_remove( MATERIAL *material, VIEW *view, VIEW *stage_view )
 * 
 * Removes an effect from the effect chain.
 * If stage_view is not NULL/0, removes the view with the ID of stage_view from the view chain.
 */
void shader_pp_remove( MATERIAL *material, VIEW *view, VIEW *stage_view )
{
	// Find the view with the material selected or "stage_view" and the previous view
	RenderState_singleton->view_last = view;
	
	while(RenderState_singleton->view_last->material != material && ((stage_view == NULL)+(RenderState_singleton->view_last->stage != NULL)) != NULL)
	{
		view = RenderState_singleton->view_last;
		RenderState_singleton->view_last = RenderState_singleton->view_last->stage;
		
		// Stage doesn't exist?
		if(RenderState_singleton->view_last == NULL) return;
	}
	
	//pass the views stage to the previous view
	view->stage = RenderState_singleton->view_last->stage;
	
	//reset the views bmap to null
	RenderState_singleton->view_last->bmap = NULL;
	
	// Remove it.
	ptr_remove(RenderState_singleton->view_last);
}

/*
 * VIEW *shader_pp_add( MATERIAL *material, VIEW *view, BMAP *bmap )
 * 
 * Adds a post-process effect to the effect chain.
 * If "bmap" is given, the view is rendered into this bitmap.
 */
VIEW *shader_pp_add( MATERIAL *material, VIEW *view, BMAP *bmap )
{
	// Find the last view of the effect chain and store its pointer
	RenderState_singleton->view_last = view;
	while(RenderState_singleton->view_last->stage != NULL)
	{
		RenderState_singleton->view_last = RenderState_singleton->view_last->stage;
	}
	
	// Creates a new view
	RenderState_singleton->view_last->stage = view_create(0);
	set(RenderState_singleton->view_last->stage, PROCESS_TARGET);
	
	// Puts the effect in the newly created view
	RenderState_singleton->view_last = RenderState_singleton->view_last->stage;
	RenderState_singleton->view_last->material = material;
	
	if(bmap)
	{
		RenderState_singleton->view_last->bmap = bmap;
	}
	
	return RenderState_singleton->view_last;
}

