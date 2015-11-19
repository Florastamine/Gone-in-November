/* render_lrays.c */
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
 * __LRaysState *render_light_rays_get_singleton()
 * 
 * Returns the singleton of the light rays state.
 */
__LRaysState *render_light_rays_get_singleton()
{
	return __LRays_singleton;
}

/*
 * void render_light_rays_new()
 * 
 * Allocates a and initializes the light rays singleton.
 * One thing to note though, the godray's parameters is so simple that I can just ditch the
 * singleton model and create wrapper functions for the parameters, but I want everything to 
 * have a single, unified convention, so, as a result, it has _new(), _setup(), _free(), ... methods
 * like any <render>-based shader types out there.
 *
 * You must free the singleton during program exit by calling render_light_rays_free()/free().
 */
void render_light_rays_new()
{
	__LRays_singleton = (__LRaysState *) sys_malloc(sizeof(__LRaysState));
	
	__LRays_singleton->ray_strength = 0.8;
	__LRays_singleton->ray_length   = 11.5;
	
	__LRays_singleton->queued = false;
}

/*
 * void render_light_rays_setup( float strength, float length )
 * 
 * Overrides the default values in the light ray state singleton.
 * These values can be dynamically changed during the lifetime of the light ray.
 */
void render_light_rays_setup( float strength, float length )
{
	if(__LRays_singleton->ray_strength != strength) __LRays_singleton->ray_strength = strength;
	if(__LRays_singleton->ray_length != length)     __LRays_singleton->ray_length   = length;
}

/*
 * void render_light_rays_free()
 * 
 * Frees the light ray state singleton.
 */
void render_light_rays_free()
{
	if(__LRays_singleton) sys_free(__LRays_singleton);
}

/*
 * void render_light_rays_set_queued( BOOL state )
 * 
 * Queues the light ray effect for rendering.
 */
void render_light_rays_set_queued( BOOL state )
{
	if(__LRays_singleton)
	{
		__LRays_singleton->queued = state;
	}
}

/*
 * BOOL render_light_rays_get_queued()
 * 
 * Returns true if the light ray effect was queued for rendering, false otherwise.
 */
BOOL render_light_rays_get_queued()
{
	return __LRays_singleton->queued;
}

/*
 * void render_light_rays()
 * 
 * Performs light ray rendering.
 */
void render_light_rays()
{
	__sun_dummy->bmap = bmap_create(game_get_asset_2d_fx("sun.tga"));
	
	MATERIAL *mtl_lightRayCut = mtl_create();
	mtl_lightRayCut->effect = "lightRayCut.fx";
	
	MATERIAL *mtl_lightRay = mtl_create();
	mtl_lightRay->effect = "lightRay.fx";
	
	MATERIAL *mtl_lightRayBlur = mtl_create();
	mtl_lightRayBlur->effect = "lightRayBlur.fx";
	
	BMAP *map_lightRayCut = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	
	VIEW *view_lightRayCut = view_create(2);
	view_lightRayCut->size_x = screen_size.x/2;
	view_lightRayCut->size_y = screen_size.y/2;
	view_lightRayCut->flags |= (PROCESS_TARGET | SHOW);
	mtl_lightRayCut->skin1 = RenderState_singleton->map_depth;
	mtl_lightRayCut->skin2 = RenderState_singleton->map_scene;
	view_lightRayCut->material = mtl_lightRayCut;
	view_lightRayCut->arc = camera->arc;
	view_lightRayCut->bmap = map_lightRayCut;
	
	BMAP *map_lightRay = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	
	VIEW *view_lightRay = view_create(2);
	view_lightRay->size_x = screen_size.x/2;
	view_lightRay->size_y = screen_size.y/2;
	view_lightRay->flags |= (PROCESS_TARGET);
	mtl_lightRay->skill1 = floatv(0.5);
	mtl_lightRay->skill2 = floatv(0.5);
	mtl_lightRay->skill3 = floatv( (render_light_rays_get_singleton())->ray_length );
	view_lightRay->arc = camera->arc;
	view_lightRay->material = mtl_lightRay;
	view_lightRayCut->stage = view_lightRay;
	view_lightRay->bmap = map_lightRay;
	
	BMAP *map_lightRayBlur = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	
	VIEW *view_lightRayBlur = view_create(2);
	view_lightRayBlur->size_x = screen_size.x/2;
	view_lightRayBlur->size_y = screen_size.y/2;
	view_lightRayBlur->flags |= (PROCESS_TARGET);
	mtl_lightRayBlur->skill1 = floatv(0.005);
	view_lightRayBlur->arc = camera->arc;
	view_lightRayBlur->material = mtl_lightRayBlur;
	view_lightRay->stage = view_lightRayBlur;
	view_lightRayBlur->bmap = map_lightRayBlur;
	
	shader_pp_add(lray, camera, 0);
	lray->skin1 = map_lightRayBlur;

	VECTOR vektor;
	float intermediate = 0.0;
	
	int lightAdded = 1;
	
	while(1)
	{
		mtl_lightRay->skill3 = floatv( (render_light_rays_get_singleton())->ray_length );
		
		vec_set( &vektor, &sun_pos);
		if( vec_to_screen( &vektor, camera) )
		{
			if(lightAdded == 0)
			{
				shader_pp_add(lray,camera,0);
				lightAdded = 1;
			}
			
			__sun_dummy->pos_x = vektor.x;
			__sun_dummy->pos_y = vektor.y;
			
			vektor.x /= screen_size.x;
			vektor.y /= screen_size.y;
			mtl_lightRay->skill1 = floatv(vektor.x);
			mtl_lightRay->skill2 = floatv(vektor.y);
			
			if(vektor.x > 0.5) vektor.x = 1.0 - vektor.x;
			if(vektor.y > 0.5) vektor.y = 1.0 - vektor.y;
			
			intermediate = (vektor.x * (vektor.y * 2)) + (vektor.y * (vektor.x * 2));
			lray->skill1 = floatv(  intermediate * (render_light_rays_get_singleton())->ray_strength );			
		}
		else
		{
			if(lightAdded == 1)
			{
				lray->skill1 = floatv(0.0);
				shader_pp_remove(lray, camera, 0);
				lightAdded = 0;
			}
		} 
		
		wait(1.0);
	}
	
	printf("No Flex Zone!");
}

/*
 * void render_light_rays_set_debug()
 * 
 * Shows a small panel which lets you know where the sun (where the ray was casted) is.
 */
void render_light_rays_set_debug()
{
	__sun_dummy->flags |= (SHOW);
}

/*
 * void render_light_rays_reset_debug()
 * 
 * Turns off the small panel representing ray location.
 */
void render_light_rays_reset_debug()
{
	__sun_dummy->flags &= ~(SHOW);
}

/*
 * BOOL render_light_rays_get_debug()
 * 
 * Returns true if the debug panel is being displayed, false otherwise.
 */
BOOL render_light_rays_get_debug()
{
	if( __sun_dummy->flags & SHOW ) return true;
	return false;
}
