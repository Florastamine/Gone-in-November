/* render_refract.c */
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
__static void __render_refract_setRT( float RT )
{
	if( (int) RT == (int) view_refract->size_x ) return;
	
	view_refract->size_x = screen_size.x / RT;
	view_refract->size_y = screen_size.y / RT;
}

/*
 * void render_refract_new()
 * 
 * Allocates and initializes the refraction state singleton.
 * This is necessary (plus an extra call to render_refract()) if you want to do water refractions.
 */
void render_refract_new()
{
	RefractState_singleton = (RefractState *) malloc(sizeof(RefractState));
	RefractState_singleton->map_refract = (BMAP *) malloc(sizeof(BMAP));
	
	RefractState_singleton->rt_factor = 1.0;
	RefractState_singleton->queued = false;
	
	// 2.0 instead of 1.0 so RefractState_singleton->view_refract will get initialized for the first time. 
	// (same values will get it unitialized, but hey, we don't need unnecessary calculations right?)
	__render_refract_setRT(2.0);
}

/*
 * void render_refract_free()
 * 
 * Frees the refraction state singleton.
 */
void render_refract_free()
{
	if(RefractState_singleton)
	{
		if(RefractState_singleton->map_refract) free(RefractState_singleton->map_refract);
		free(RefractState_singleton);
	}
}

/*
 * void render_refract()
 * 
 * Sets up the refraction data for later use in the water shader.
 */
void render_refract()
{	
	float rt = RefractState_singleton->rt_factor;
	
	RefractState_singleton->map_refract = bmap_createblack( screen_size.x / rt, screen_size.y / rt, 32 );
	
	view_refract->bmap = RefractState_singleton->map_refract;
	view_refract->clip_near = 1.0;
	view_refract->flags |= (SHOW | UNTOUCHABLE);
	view_refract->flags &= ~(AUDIBLE);
}

/*
 * void render_refract_set_queued( BOOL state )
 * 
 * Queue refraction for rendering.
 */
void render_refract_set_queued( BOOL state )
{
	if(RefractState_singleton)
	{
		RefractState_singleton->queued = true;
	}
}

/*
 * BOOL render_refract_get_queued()
 * 
 * Returns true if refraction is queued for rendering, false otherwise.
 */
BOOL render_refract_get_queued()
{
	return RefractState_singleton->queued;
}

/*
 * void render_refract_setup( float RT )
 * 
 * Overrides the render target's default value in the refraction state singleton.
 */
void render_refract_setup( float RT )
{
	if(RefractState_singleton)
	{
		RefractState_singleton->rt_factor = ifelse(RT, RT, 2.0);
		__render_refract_setRT(RT);
	}
}

/*
 * RefractState *render_refract_get_singleton()
 * 
 * Returns the refraction state singleton.
 */
RefractState *render_refract_get_singleton()
{
	return RefractState_singleton;
}
