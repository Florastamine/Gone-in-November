/* render_dof.c */
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
__static void __mtl_depth_event()
{
	mtl->skill1 = floatv((render_dof_get_singleton())->sharpness);
	mtl->skill2 = floatv((render_dof_get_singleton())->position);
	mtl->skill3 = floatv(0);
	mtl->skill4 = floatv((render_dof_get_singleton())->scene_max_depth);
	
	if(my)
	{
		if(my->skill99 != 0)
		{
			Attribute *temp = (Attribute *) (my->skill99);
			
			mtl->skill3 = floatv(temp->depth);
			mtl->skill7 = floatv(temp->volpart);
		}
	}
}

__static void __render_dof_depth_setup()
{
	(RenderState_get_singleton())->map_depth = bmap_createblack(screen_size.x, screen_size.y,(render_dof_get_singleton())->bit_depth);
	view_depth->bmap = (RenderState_get_singleton())->map_depth;
	set(view_depth,SHOW);
	
	proc_mode = PROC_LATE;
	
	view_depth->clip_far = (render_dof_get_singleton())->scene_max_depth;
	view_depth->clip_near = 0;
	
	while((RenderState_get_singleton())->ready)
	{
		vec_set( &view_depth->x, &camera->x );
		vec_set( &view_depth->pan, &camera->pan );
		view_depth->arc = camera->arc;
		wait(1);
	}
}

/*
 * void render_dof_depth_set( float focus_speed, float max_depth, float b0n )
 * 
 * Activates auto focus, and set focus speed and maximum depth.
 * The third parameter should be set to 0.1.
 */
__static float __focus_temp[3];
void render_dof_depth_set( float focus_speed, float max_depth, float b0n )
{
	if(__focus_temp[2] == b0n) return;
	
	float hit_dist;
	VECTOR cam_trace;
	
	__focus_temp[0] = focus_speed;
	__focus_temp[1] = max_depth;
	__focus_temp[2] = b0n;
	
	while(__focus_temp[2] > 0)
	{
		vec_set(&cam_trace, vector(screen_size.x / 2, screen_size.y / 2, __focus_temp[1]));
		vec_for_screen(&cam_trace, camera);
		hit_dist = c_trace( &camera->x, &cam_trace, USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE );
		
		if(hit_dist > 0)
		{
			if(hit_dist > (render_dof_get_singleton())->position) (render_dof_get_singleton())->position += __focus_temp[0] * time_step;
			if(hit_dist < (render_dof_get_singleton())->position) (render_dof_get_singleton())->position -= __focus_temp[0] * time_step;
		}
		
		wait(1.0);
	}
}

/*
 * DOFState *render_dof_get_singleton()
 * 
 * Returns the DOF state object singleton.
 * Can then be used to query DOF-related parameters.
 */
DOFState *render_dof_get_singleton()
{
	return DOFState_singleton;
}

/*
 * void render_dof_new()
 * 
 * Allocates memory for the DOF state object singleton.
 * Once DOF is activated, this state can be safely freed (via render_dof_free()/free()), but you won't be able to query DOF parameters or 
 * the state singleton afterwards.
 */
void render_dof_new()
{
	DOFState_singleton = (DOFState *) malloc(sizeof(DOFState));
	
	DOFState_singleton->rt_factor = 2.0;
	DOFState_singleton->bit_depth = 32.0;
	DOFState_singleton->scene_max_depth = 50000.0;
	
	DOFState_singleton->sharpness = 800.0;
	DOFState_singleton->position = 400.0;
	DOFState_singleton->blurness = 0.8;
	
	DOFState_singleton->queued = false; // Ironed out a bug in <render_utils>/render_queue_start() 1-0.
	
	mtl_depth->event = __mtl_depth_event;
}

/*
 * void render_dof_free()
 * 
 * Frees the DOF state object singleton.
 */
void render_dof_free()
{
	if(DOFState_singleton) free(DOFState_singleton);
}

/*
 * BOOL render_dof_get_queued()
 * 
 * Returns true if DOF is being queried for rendering, false otherwise.
 */
BOOL render_dof_get_queued()
{
	if(DOFState_singleton) return DOFState_singleton->queued;
}

/*
 * void render_dof_set_queued( __In BOOL state )
 * 
 * Queries DOF for rendering later (with render_queue_start() in <render_utils>)
 */
void render_dof_set_queued( __In BOOL state )
{
	if(DOFState_singleton) DOFState_singleton->queued = true;
}

/*
 * void render_dof_setup( __In int rt_factor, __In int bit_depth, __In int scene_max_depth, __In int sharpness, __In int position, __In int blurness )
 * 
 * Overrides the default values in the DOF state singleton.
 * Certain arguments can be ignored; simply pass (-1) to which parameters you want to ignore.
 */
void render_dof_setup( __In int rt_factor, __In int bit_depth, __In int scene_max_depth, __In int sharpness, __In int position, __In int blurness )
{
	if(DOFState_singleton)
	{
		DOFState_singleton->rt_factor       = ifelse(rt_factor == -1, DOFState_singleton->rt_factor, rt_factor);
		DOFState_singleton->bit_depth       = ifelse(bit_depth == -1, DOFState_singleton->bit_depth, bit_depth);
		DOFState_singleton->scene_max_depth = ifelse(scene_max_depth == -1, DOFState_singleton->scene_max_depth, scene_max_depth);
		DOFState_singleton->sharpness       = ifelse(sharpness == -1, DOFState_singleton->sharpness, sharpness);
		DOFState_singleton->position        = ifelse(position == -1, DOFState_singleton->position, position);
		DOFState_singleton->blurness        = ifelse(blurness == -1, DOFState_singleton->blurness, blurness);
	}
}

/*
 * void render_dof()
 * 
 * Performs DOF rendering.
 */
void render_dof()
{
	__render_dof_initialize();
	
	camera->stage = view_dofDownsample;
	view_dofDownsample->stage = view_dofHBlur;
	view_dofHBlur->stage = view_dofVBlur;
	view_dofVBlur->stage = view_dof;
}

__static void __render_dof_initialize()
{
	__render_dof_depth_setup();
	
	__static float RT = (render_dof_get_singleton())->rt_factor;
	__static float bits = (render_dof_get_singleton())->bit_depth;
	
	mtl_dofDownsample->effect = "dofDownsample.fx";
	mtl_dof->skill4 = floatv(RT);
	mtl_dofDownsample->skill1 = floatv(RT);
	view_dofDownsample->size_x = screen_size.x/RT;
	view_dofDownsample->size_y = screen_size.y/RT;
	view_dofDownsample->bmap = bmap_createblack(screen_size.x/RT,screen_size.y/RT,bits);
	view_dofHBlur->size_x = screen_size.x/RT;
	view_dofHBlur->size_y = screen_size.y/RT;
	view_dofHBlur->bmap = bmap_createblack(screen_size.x/RT,screen_size.y/RT,bits);
	view_dofVBlur->size_x = screen_size.x/RT;
	view_dofVBlur->size_y = screen_size.y/RT;
	view_dofVBlur->bmap = bmap_createblack(screen_size.x/RT,screen_size.y/RT,bits);
	
	#ifdef __HDR
		if(render_hdr_get_queued())
		{
			(RenderState_get_singleton())->bmap_dof = bmap_createblack(screen_size.x, screen_size.y,bits);
			view_dof->bmap = (RenderState_get_singleton())->bmap_dof;
		}
	#endif
	
	mtl_dofDownsample->skin1 = (RenderState_get_singleton())->map_depth;
	mtl_dof->skin1 = (RenderState_get_singleton())->map_scene;
	mtl_dof->skin2 = (RenderState_get_singleton())->map_depth;
	mtl_dofHBlur->skill1 = floatv((render_dof_get_singleton())->blurness);
	mtl_dofVBlur->skill1 = floatv((render_dof_get_singleton())->blurness);
}
