/* render_water.c */
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
__static void __render_water_set( const float depth, const STRING *fresnel )
{
	if(WaterState_singleton->depth != depth) WaterState_singleton->depth = depth;
	
	if(fresnel)
	{
		WaterState_singleton->fresnel = bmap_create(WaterState_singleton);
		mtl_water->skin4 = WaterState_singleton->fresnel;
	}
}

__static void __mtl_water_event()
{
	mtl->skill1 = floatv(-1.0);
	if(render_view == camera) mtl->skill1 = floatv(1.0);
}

/*
 * void render_water_set_depth( float depth )
 * 
 * Sets the water depth.
 */
void render_water_set_depth( float depth )
{
	__render_water_set(5.0, NULL);
}

/*
 * void render_water_new()
 * 
 * Creates and allocates the water state singleton.
 */
void render_water_new()
{
	WaterState_singleton = (WaterState *) malloc(sizeof(WaterState));
	WaterState_singleton->fresnel = (BMAP *) malloc(sizeof(BMAP));
	
	WaterState_singleton->depth = 1.0;
	__render_water_set(5.0, str_create("fresnel.bmp") );
	mtl_water->event = __mtl_water_event;
}

/*
 * void render_water_free()
 * 
 * Frees the water state singleton.
 */
void render_water_free()
{
	if(WaterState_singleton)
	{
		if(WaterState_singleton->fresnel) free(WaterState_singleton->fresnel);
		free(WaterState_singleton);
	}
}

/*
 * WaterState *render_water_get_singleton()
 * 
 * Returns the water state singleton.
 */
WaterState *render_water_get_singleton()
{
	return WaterState_singleton;
}

__static void __render_water_loop( ENTITY *entity )
{
	proc_kill(4);
	
	VECTOR temp;
	#ifdef    __REFLECT
	    mtl_water->skin1 = (render_reflect_get_singleton())->map_reflect;
	#endif
	
	#ifdef    __REFRACT
	    mtl_water->skin2 = (render_refract_get_singleton())->map_refract;
	#endif
	
	while(entity)
	{
		proc_mode = PROC_LATE;
		
		#ifdef    __DOF
		    mtl_depth->skill5 = floatv(entity->z);
		    mtl_depth->skill6 = floatv((render_water_get_singleton())->depth);
		#endif
		
		#ifdef    __REFLECT
		    view_reflect->arc = camera->arc;
		#endif
		
		vec_zero(temp);
		vec_rotate( &temp, &camera->pan );
		vec_add( &temp, &camera->x );
		
		#ifdef    __REFLECT
		    // vec_set( &((render_reflect_get_singleton())->view_reflect->x), vector(temp.x, temp.y, ent->z - camera->z) );
		    vec_set( &view_reflect->x, vector(temp.x, temp.y, entity->z - camera->z) );
		    view_reflect->pan = camera->pan;
		    view_reflect->tilt = -camera->tilt;
		#endif
		
		wait(1.0);
	}
}

/*
 * void render_water( ENTITY *entity )
 * 
 * Performs water rendering.
 * If refraction/reflection was enabled before, they *MUST* be activated, otherwise
 * it'll crash (because the water shader access uninitialized data).
 */
void render_water( ENTITY *entity )
{
	#ifdef    __DOF
	    mtl_water->skin3 = mtl_depth;
	#endif
	
	render_attribute_setup(entity, ATTRIBUTE_DEPTH, 2);
	entity->material = mtl_water;
	
	__render_water_loop(entity);
}
