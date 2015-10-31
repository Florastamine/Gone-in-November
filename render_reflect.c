/* render_reflect.c */
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
__static void __render_reflect_setRT(float rtX, float rtY)
{
	if( rtX != view_reflect->size_x ) view_reflect->size_x = rtX;
	if( rtY != view_reflect->size_y ) view_reflect->size_y = rtY;
}

/*
 * ReflectState *render_reflect_get_singleton()
 * 
 * Returns the singleton object of the ReflectState struct which 
 * contains reflection information.
 */
ReflectState *render_reflect_get_singleton()
{
	return ReflectState_singleton;
}

/*
 * void render_reflect_new()
 * 
 * Allocates and initializes a the reflection state singleton, which must be freed later with render_reflect_free()/free().
 * This is necessary if you want to do water reflections.
 */
void render_reflect_new()
{
	ReflectState_singleton = (ReflectState *) malloc(sizeof(ReflectState));
	ReflectState_singleton->map_reflect = (BMAP *) malloc(sizeof(BMAP));
	
	ReflectState_singleton->rt_factor_x = 1.0;
	ReflectState_singleton->rt_factor_y = 1.0;
	ReflectState_singleton->queued = false;
	
	__render_reflect_setRT(512.0, 384.0); // Set default values, so that the singleton gets initialized first time.
}

/*
 * void render_reflect_free()
 * 
 * Frees the reflection state singleton.
 * Freeing the state immediately after initialization is *NOT* recommended as it contains 
 * data reserved for later use (for example the reflection map which is used later if you use water effects).
 */
void render_reflect_free()
{
	if(ReflectState_singleton)
	{
		if(ReflectState_singleton->map_reflect) free(ReflectState_singleton->map_reflect);
		free(ReflectState_singleton);
	}
}

/*
 * void render_reflect_setup( float rtx, float rty )
 * 
 * Overrides the default render target size values in the reflection state singleton.
 */
void render_reflect_setup( float rtx, float rty )
{
	if(ReflectState_singleton)
	{
		ReflectState_singleton->rt_factor_x = ifelse(rtx, rtx, 512.0);
		ReflectState_singleton->rt_factor_y = ifelse(rty, rty, 384.0);
		
		__render_reflect_setRT(rtx, rty);		
	}
}

/*
 * void render_reflect_set_queued( BOOL state )
 * 
 * Queue reflection for rendering.
 */
void render_reflect_set_queued( BOOL state )
{
	if(ReflectState_singleton)
	{
		ReflectState_singleton->queued = true;
	}
}

/*
 * BOOL render_reflect_get_queued()
 * 
 * Returns true if reflection is queried for rendering.
 */
BOOL render_reflect_get_queued()
{
	return ReflectState_singleton->queued;
}

/*
 * void render_reflect()
 * 
 * "Performs" reflection.
 * Well, not actually, since it only sets up the reflection map & view
 * for use later within the water shader.
 */
void render_reflect()
{
	ReflectState_singleton->map_reflect = bmap_createblack(ReflectState_singleton->rt_factor_x, ReflectState_singleton->rt_factor_y, 32);
	view_reflect->bmap = ReflectState_singleton->map_reflect;
	view_reflect->clip_near = 1.0;
	
	view_reflect->flags |= (SHOW | NOCULL | PORTALCLIP | UNTOUCHABLE | AUDIBLE);
	view_reflect->aspect = -1;
	vec_set( &view_reflect->portal_x, vector(0, 0, -1) );
}
