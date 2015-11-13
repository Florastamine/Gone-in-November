/* render_shadow.c */
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
__static void mtl_shadowDepthSun_event()
{
	mat_set(mtl_shadow_sun->matrix, matWorld);
	mat_multiply(mtl_shadow_sun->matrix, matView);
	
	mat_set((render_shadow_get_singleton())->sun_light_view, matWorld);
	mat_multiply((render_shadow_get_singleton())->sun_light_view, matView);
	mat_effect3 = (render_shadow_get_singleton())->sun_light_view;
	
	mtl->skill3 = floatv(0.0);
	mtl->skill4 = floatv(0.0);
	
	if(my)
	{
		if(my->skill99 != 0)
		{
			Attribute *temp = (Attribute *) (my->skill99);
			
			mtl->skill3 = floatv(temp->depth);
			mtl->skill4 = floatv(temp->volpart);
		}
	}
}

/*
 * ShadowState *render_shadow_get_singleton()
 * 
 * Returns the shadow mapping state singleton.
 */
ShadowState *render_shadow_get_singleton()
{
	return ShadowState_singleton;
}

/*
 * void render_shadow_free()
 * 
 * Frees the shadow mapping state singleton.
 * All handles and pointers will be automatically invalid afterwards, and will cause
 * chaos such as crashes and all sorts of weird errors (if shadow mapping is in use).
 * Thus, you should only free the shadow singleton upon program exit.
 */
void render_shadow_free()
{
	if(ShadowState_singleton)
	{
		if(ShadowState_singleton->map_shadow_alpha)      bmap_remove(ShadowState_singleton->map_shadow_alpha);
		if(ShadowState_singleton->map_shadow_depth_sun)  bmap_remove(ShadowState_singleton->map_shadow_depth_sun);
		if(ShadowState_singleton->mtl_shadow_sun)        mtl_remove(ShadowState_singleton->mtl_shadow_sun);
		if(ShadowState_singleton->blur_depth_view)       ShadowState_singleton->blur_depth_view = NULL;
		if(ShadowState_singleton->shadow_depth_view)     ShadowState_singleton->shadow_depth_view = NULL;
		
		sys_free(ShadowState_singleton);
	}
}

/*
 * void render_shadow_set_queued( BOOL state )
 * 
 * Queues shadow mapping for rendering later with render_queue_start().
 */
void render_shadow_set_queued( BOOL state )
{
	ShadowState_singleton->queued = state;
}

/*
 * BOOL render_shadow_get_queued()
 * 
 * Returns true if shadow mapping is queued for rendering.
 */
BOOL render_shadow_get_queued()
{
	return ShadowState_singleton->queued;
}

/*
 * void render_shadow_new()
 * 
 * Allocates and initializes a new shadow mapping state singleton.
 * The singleton must be later freed with a call to render_shadow_free().
 */
void render_shadow_new()
{
	
	ShadowState_singleton = (ShadowState *) sys_malloc(sizeof(ShadowState_singleton));
	/* ShadowState_singleton->map_shadow_depth_sun = (BMAP *) sys_malloc(sizeof(BMAP)); */ // <- Cause a crash due to (maybe, very ) bmap_create() allocates its own memory.
	ShadowState_singleton->map_shadow_alpha = bmap_create("shadow_alpha.bmp");
	
	//ShadowState_singleton->blur_depth_view = (VIEW *) sys_malloc(sizeof(VIEW)); // <- The same with View objects.
	ShadowState_singleton->blur_depth_view = view_create( -2 );
	
	//ShadowState_singleton->shadow_depth_view = (VIEW *) sys_malloc(sizeof(VIEW));
	ShadowState_singleton->shadow_depth_view = view_create( -21 );
	
	mtl_shadow_sun->skin2 = ShadowState_singleton->map_shadow_alpha;
	
	mtl_shadowDepthSun->event = mtl_shadowDepthSun_event;
	
	/*
	int large = sizeof(int) * 16;
	memset(ShadowState_singleton->ortho_light_pos, 0, large);
	memset(ShadowState_singleton->ortho_matrix,    0, large);
	memset(ShadowState_singleton->sun_light_view,  0, large);
	*/
	
	ShadowState_singleton->resolution         = 0.0;
	ShadowState_singleton->resolution_range   = 0.0;
	ShadowState_singleton->depth              = 0.0;
	ShadowState_singleton->blurness           = 0.0;
	
	render_shadow_setup();
	
	ShadowState_singleton->queued = false;
}

/*
 * void render_shadow_setup( float resolution, float resolution_range, float depth, float blurness)
 * 
 * Sets various shadow parameters:
 * - resolution: Controls the shadow map resolution. Should be smaller or equal to screen_size.x.
 * - resolution_range: Controls the area of shadow.
 * - depth: The distance after which shadows are clipped. Keep this as low as possible for nice shadows.
 * - blurness: Adjust the "blurness" of shadows.
 * 
 * After the call to render_shadow(), values are "locked" and can no longer be modified.
 */
void render_shadow_setup()
{
	render_shadow_setup( screen_size.x, 15000.0, 1500.0, 0.002 );
}

void render_shadow_setup( float resolution, float resolution_range, float depth, float blurness)
{	
	wait(5.0);
	
	ShadowState_singleton->blur_depth_view->flags |= (PROCESS_TARGET | CHILD);
	ShadowState_singleton->blur_depth_view->material = mtl_shadowBlur;
	
	while( !camera ) wait(1.0);
	
	D3DXMatrixOrthoLH(ShadowState_singleton->ortho_matrix, resolution_range * 1.5, resolution_range, 0.0, camera->clip_far * 5);
	mat_set( mtl_shadowDepthSun->matrix, ShadowState_singleton->ortho_matrix );
	mat_effect1 = ShadowState_singleton->ortho_matrix;
	
	mtl_shadowDepthSun->flags |= (ENABLE_VIEW);
	
	ShadowState_singleton->shadow_depth_view->material = mtl_shadowDepthSun;
	ShadowState_singleton->shadow_depth_view->pan = 302.0;
	ShadowState_singleton->shadow_depth_view->tilt = -38.0;
	ShadowState_singleton->shadow_depth_view->flags |= (SHOW | UNTOUCHABLE | NOSHADOW | NOPARTICLE);
	ShadowState_singleton->shadow_depth_view->size_x = resolution;
	ShadowState_singleton->shadow_depth_view->size_y = resolution;
	
	ShadowState_singleton->map_shadow_depth_sun = bmap_createblack(resolution, resolution, 14);
	ShadowState_singleton->shadow_depth_view->bmap = bmap_createblack(resolution, resolution, 14);
	ShadowState_singleton->shadow_depth_view->stage = ShadowState_singleton->blur_depth_view;
	ShadowState_singleton->blur_depth_view->bmap = ShadowState_singleton->map_shadow_depth_sun;
	mtl_shadow_sun->skin1 = ShadowState_singleton->map_shadow_depth_sun;
	
	(ShadowState_singleton->ortho_light_pos)[3]   = floatv(depth);
	mtl_shadowBlur->skill1                        = floatv(blurness);
	
	// Record the parameters to the singleton.
	(render_shadow_get_singleton())->resolution         = resolution;
	(render_shadow_get_singleton())->resolution_range   = resolution_range;
	(render_shadow_get_singleton())->depth              = depth;
	(render_shadow_get_singleton())->blurness           = blurness;
}

/*
 * void render_shadow()
 * 
 * Performs shadow mapping.
 */
void render_shadow()
{
	VECTOR temp, camera_dist;
	vec_zero(temp);
	vec_zero(camera_dist);
	
	vec_set( &camera_dist, vector((render_shadow_get_singleton())->resolution_range / 2, 0, 0) );
	
	you = __the_sun;
	while(1)
	{
		vec_set( &temp, vector( -sun_pos.x, -sun_pos.y, -sun_pos.z) );
		vec_sub( &temp, &(ShadowState_singleton->shadow_depth_view->x) );
		vec_to_angle( &(ShadowState_singleton->shadow_depth_view->pan), &temp );
		
		vec_set( &you->x, &camera_dist );
		vec_rotate( &you->x, &camera->pan );
		vec_add( &you->x, &camera->x );
		
		vec_set( &temp, &sun_pos );
		vec_scale( &temp, 0.05 );
		vec_set( &(ShadowState_singleton->shadow_depth_view->x), &temp );
		vec_rotate( &(ShadowState_singleton->shadow_depth_view->x), &you->pan );
		vec_add( &(ShadowState_singleton->shadow_depth_view->x), &you->x );
		
		vec_set( &temp, &(ShadowState_singleton->shadow_depth_view->x) );
		(ShadowState_singleton->ortho_light_pos)[0] = floatv(temp.x);
		(ShadowState_singleton->ortho_light_pos)[1] = floatv(temp.y);
		(ShadowState_singleton->ortho_light_pos)[2] = floatv(temp.z);
		mat_effect2 = (ShadowState_singleton->ortho_light_pos);
		
		wait(1.0);
	}
}
