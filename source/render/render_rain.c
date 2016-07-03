/* render_rain.c */
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
 * __________________________________________________________________
 * 
 * <render_rain>
 * Rain. Can be used independently of the above <render*> headers.
 * Call render_rain_new() and render_rain_start() to perform
 * rain rendering. Make sure both the scene (level_ent) and the camera entity is valid.
 * 
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */

/*
 * void render_rain_new()
 * 
 * Allocates and initializes the rain state singleton.
 * After this call, rain can be permanently activated (and switched on/off)
 * by using render_rain_start()/render_rain_stop().
 */
void render_rain_new()
{
	RainState_singleton = (RainState *) sys_malloc(sizeof(RainState));
	
	// Set default values.
	RainState_singleton->fall_speed = vector(0, 0, 0);
	RainState_singleton->depth      = 3;                       // Immutable (planned later). (TODO: Adjustable depth, very easy).
	
	RainState_singleton->rain = mtl_create();
	effect_load(RainState_singleton->rain, "rain.fx");
	render_rain_set_speed(0.15, 0.2, 0.07);
	
	RainState_singleton->rain_levels = (ENTITY *) sys_malloc(sizeof(ENTITY) * RainState_singleton->depth);
	int i = 0;
	for(; i < RainState_singleton->depth; i++)
	{
		(RainState_singleton->rain_levels)[i] = ent_create( game_asset_get_object("__rain_cylinder.mdl"), nullvector, NULL);
		(RainState_singleton->rain_levels)[i]->scale_x = 2 + 8 * i;
		(RainState_singleton->rain_levels)[i]->scale_y = 2 + 8 * i;
		(RainState_singleton->rain_levels)[i]->scale_z = 100;
		(RainState_singleton->rain_levels)[i]->material = RainState_singleton->rain;
		(RainState_singleton->rain_levels)[i]->flags |= PASSABLE | SHOW;
	}
	
	RainState_singleton->state = 0;
}

/*
 * void render_rain_set_speed(float x, float y, float z)
 * 
 * Sets the speed of the rainfall.
 */
void render_rain_set_speed(float x, float y, float z)
{
	render_rain_set_speed(vector(x, y, z));
}

void render_rain_set_speed(VECTOR *speed)
{
	if( RainState_singleton->fall_speed->x == speed->x &&
	    RainState_singleton->fall_speed->y == speed->y &&
	    RainState_singleton->fall_speed->z == speed->z)
	return;
	
	if(RainState_singleton)
	{
		vec_set(RainState_singleton->fall_speed, speed);
		RainState_singleton->rain->skill1 = floatv(RainState_singleton->fall_speed->x);
		RainState_singleton->rain->skill2 = floatv(RainState_singleton->fall_speed->y);
		RainState_singleton->rain->skill3 = floatv(RainState_singleton->fall_speed->z);
	}
}

/*
 * void render_rain_start()
 * 
 * Lets the sky rain.
 */
void render_rain_start()
{
	proc_mode = PROC_LATE;	
	if( RainState_singleton )
	{
		RainState_singleton->state = 1 - RainState_singleton->state;
		
		int i = 0;
		
		if(RainState_singleton->state)
		{
			for(; i < RainState_singleton->depth; i++)
			    (RainState_singleton->rain_levels)[i]->flags &= ~INVISIBLE;
		}
		
		while(RainState_singleton->state != 0)
		{
			for(i = 0; i < RainState_singleton->depth; i++)
			{
				vec_set(&(RainState_singleton->rain_levels)[i]->x, &camera->x);
				(RainState_singleton->rain_levels)[i]->skill41 = floatv(2.1);		// Control visibility (0 to ~)
			}
			
			wait(1.0);
		}
	}
}

/*
 * void render_rain_stop()
 * 
 * Stops raining.
 */
void render_rain_stop()
{
	RainState_singleton->state = 0;
	
	int i = 0;
	for(; i < RainState_singleton->depth; i++)
	    (RainState_singleton->rain_levels)[i]->flags |= (INVISIBLE); /* Somehow SHOW and INVISIBLE are two different flags? */
}

/*
 * void render_rain_free()
 * 
 * Frees the rain state singleton.
 */
void render_rain_free()
{
	int i = 0;
	
	render_rain_stop();
	mtl_remove(RainState_singleton->rain);
	for(; i < RainState_singleton->depth; i++)
	    ent_remove((RainState_singleton->rain_levels)[i]);
	    
	sys_free(RainState_singleton);
}

int render_rain_get_depth()
{
	if( RainState_singleton )
	{
		return RainState_singleton->depth;
	}
}

int render_rain_get_state()
{
	if( RainState_singleton )
	{
		return RainState_singleton->state;
	}
}
