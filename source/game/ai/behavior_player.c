/* behavior_player.c */
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
 * <behavior_player>
 *
 * Player code, first person, with credits goes to 3Run.
 * I modified, optimized and simplified his original code for PRAGMA_POINTER
 * compatibility and better coding structures. As a result (and as the nature of
 * GiN), the code won't include ducking and jumping.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
/* __static */ int __invert_y          = 1; // vodkins's request

 __static void __text_init_pos(Text *text) {
     if(text) {
         text->pos_x = (screen_size.x - str_width((text->pstring)[0], text->font)) * 0.5;
         text->pos_y = screen_size.y - 150.0;
     }
 }

__act_player_state *act_player_get_singleton()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton;
}

void act_player_free()
{
	game_log_write("Request to free the soul from all the burdens.");

	if( __act_player_state_singleton )
	{
		dtimer();
		game_log_write("Now freeing the soul.");

		safe_remove(__act_player_state_singleton->__object);
		safe_remove(__act_player_state_singleton->__object_stand);

		FREE(__act_player_state_singleton);

		double d = dtimer();
		game_log_write( str_printf(NULL, "The soul was freed within a total of %f seconds.", d * POW_10_6) );
	}
}

void act_player_new()
{
	game_log_write("Request to allocate a home for the soul...");

	if( !__act_player_state_singleton )
	{
		game_log_write("Home not found. Allocating a new one...");
		dtimer();

		__act_player_state_singleton = MALLOC(1, __act_player_state);

		// Don't need to do MALLOC() for entity pointers because ent_create() will take
		// care of that instead.
		vec_zero(__act_player_state_singleton->__bbox_lowest_pos);
		vec_zero(__act_player_state_singleton->__platform_move_speed);
		vec_zero(__act_player_state_singleton->__cam_pos);
		vec_zero(__act_player_state_singleton->__cam_ang);
		vec_zero(__act_player_state_singleton->__last_pos);
		vec_zero(__act_player_state_singleton->__first_pos);

		__act_player_state_singleton->__move_type                 = MOVE_ON_FOOT;
		__act_player_state_singleton->state                       = STATE_IDLE;
		__act_player_state_singleton->health                      = 100;
		__act_player_state_singleton->cam_height                  = 60;
		__act_player_state_singleton->cam_lerp                    = 0;
		__act_player_state_singleton->cam_smooth                  = 0.9;
		__act_player_state_singleton->cam_smooth_offset           = 2;
		__act_player_state_singleton->friction                    = 2.0;
		__act_player_state_singleton->movement_friction           = 0.0;
		__act_player_state_singleton->movement_friction_air       = 0.4;
		__act_player_state_singleton->movement_friction_ground    = 0.8;
		__act_player_state_singleton->move_speed                  = 42 + 42 + 21; /* ;) */
		__act_player_state_singleton->climb_speed                 = 2.5;
		__act_player_state_singleton->run_multiplier              = 1.5;
		__act_player_state_singleton->walk_multiplier             = 1.0;

		__act_player_state_singleton->__distance_to_ground        = 0.0;
		__act_player_state_singleton->__quants_covered            = 0.0;

		__act_player_state_singleton->can_stand                   = true;
		__act_player_state_singleton->can_climb                   = false;
		__act_player_state_singleton->is_moving                   = false;
		__act_player_state_singleton->can_move                    = true;

		// Create the corresponding footstep sound effects. TODO: re-factor them into customizable functions with arbitrary stuff.
		(__act_player_state_singleton->__footstep_sound)[0]       = snd_create(game_asset_get_sound("foot0.wav"));
		(__act_player_state_singleton->__footstep_sound)[1]       = snd_create(game_asset_get_sound("foot1.wav"));
		(__act_player_state_singleton->__footstep_sound)[2]       = snd_create(game_asset_get_sound("foot2.wav"));
		(__act_player_state_singleton->__footstep_sound)[3]       = snd_create(game_asset_get_sound("foot3.wav"));

		double d = dtimer();
		game_log_write( str_printf(NULL, "Finished building a home for the soul. (%f seconds)", d * POW_10_6) );

        /*
        __invert_y_state[0] = 1;
        __invert_y_state[1] = -1;
        */
	}
}

__static void __act_player_update_camera()  // hôm qua mẹ nói
                                            // bà cô nào đó có đứa con trai đang định cư và học ở nước ngoài
                                            // bà cô hỏi mẹ ta xem ta có muốn quen không để còn mối.... ôi trời ạ
                                            // :'(
{
    proc_mode = PROC_LATE; // Only updates the camera after everything has been called.

    if(!__camera_locked) {
        if(__act_player_state_singleton->can_move && __act_player_state_singleton->__move_type != MOVE_ON_LADDER)
        {
            __act_player_state_singleton->__cam_ang.pan = cycle(__act_player_state_singleton->__cam_ang.pan - (joy_raw.z / 192) - mickey.x / 6.5 * 1.0, 0, 360);
            __act_player_state_singleton->__cam_ang.tilt = clamp(__act_player_state_singleton->__cam_ang.tilt - (joy_rot.x / 192) - mickey.y / 6.5 * __invert_y, -90, 90);
            __act_player_state_singleton->__cam_ang.roll = 0;
        }

        vec_set( &camera->pan, &(__act_player_state_singleton->__cam_ang) );
        vec_set( &(__act_player_state_singleton->__cam_pos), vector(0, 0, __act_player_state_singleton->cam_height));
        vec_rotate( &(__act_player_state_singleton->__cam_pos) , vector(camera->pan, 0, 0));
        vec_add( &(__act_player_state_singleton->__cam_pos), &my->x );
        vec_set( &camera->x, vector(__act_player_state_singleton->__cam_pos.x, __act_player_state_singleton->__cam_pos.y, camera->z) );
        __act_player_state_singleton->cam_lerp = __act_player_state_singleton->cam_smooth * time_step;

        camera->z -= (camera->z - __act_player_state_singleton->__cam_pos.z) * __act_player_state_singleton->cam_lerp;
        camera->z = clamp(camera->z, __act_player_state_singleton->__cam_pos.z - __act_player_state_singleton->cam_smooth_offset, __act_player_state_singleton->__cam_pos.z + __act_player_state_singleton->cam_smooth_offset);
        // camera->arc += ifelse(mouse_right != 0, -1, 1) * 3.5 * time_step; // 3.5 gives the zooming speed. Magic numbers is bad, I know, but I'm just too lazy to re-factor these.
        camera->z += 35.0;
    }

	if(camera->tilt > 75.0)
	    camera->tilt = 75.0;
	if(camera->tilt < -70.0)
	    camera->tilt = -70.0;

    /*
	if(camera->arc <= 45.0)
		camera->arc = 45.0;
	if(camera->arc >= 75.0)
		camera->arc = 75.0;
    */
}

__static void __act_player_scan_foot()
{
	float old = my->min_z;
	long mode = IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT | IGNORE_FLAG2 | USE_BOX;

	c_ignore(PUSH_GROUP, 0);
	my->min_z = -0.125;
	c_trace( &my->x, vector(my->x, my->y, my->z - 256.0), mode );
	my->min_z = old;

	// get lowest position of the bbox:
	vec_for_min( &(__act_player_state_singleton->__bbox_lowest_pos), my);
	// reset distance:
	__act_player_state_singleton->__distance_to_ground = 999;

	if(trace_hit)
	    __act_player_state_singleton->__distance_to_ground  = my->z + __act_player_state_singleton->__bbox_lowest_pos.z - target.z; // find height to the ground:

	vec_set( &(__act_player_state_singleton->__platform_move_speed), nullvector);

	if(you != NULL) // if we are standing on something
	{
		if( your->OBJECT_TYPE == OBJECT_PLATFORM ) // platform?
		    vec_set( &(__act_player_state_singleton->__platform_move_speed), vector(your->SPEED_X, your->SPEED_Y, 0) ); // save your movement speed:

		// more cases here...
	}
}

/*
 * __act_player_register_physics()
 *
 * Register the player to the physics engine.
 */
__static void __act_player_register_physics()
{
	game_log_write("Now registering the soul to the physics engine.");

	#ifdef ACKPHYSX_H
	if(__GameState_singleton->__game_physx_loaded__)
	{
		pXent_settype(my, PH_RIGID, PH_CAPSULE);
		pXent_setmass(my, 5);
		pXent_setskinwidth(my, 0.01);
		pXent_setbodyflag(my, NX_BF_FROZEN_ROT, 1);
		pXent_setdamping(my, 50, 100);
		pXent_setmaterial(my, vector(1, 0, 0), vector(0, 0, NX_MF_DISABLE_FRICTION | NX_MF_DISABLE_STRONG_FRICTION), NULL);

		game_log_write("Done registering.");
	}
	else
	{
		game_log_write("Failed to register the soul. (PhysX failed to initialize)");
	}
	#else
	    game_log_write("Header <ackphysX> wasn't included in the current build; cannot register.");
	#endif
}

__static void __act_player_create_bbox()
{
	__act_player_state_singleton->__object_stand = ent_create( game_asset_get_object("bbox_stand.mdl") , &my->x, NULL); // create stand shape
	__act_player_state_singleton->__object_stand->flags |= (POLYGON);
	object_scale_set(__act_player_state_singleton->__object_stand, player->scale_x);

	#ifndef    DEBUG
		__act_player_state_singleton->__object_stand->flags |= (INVISIBLE);
	#endif

	while(proc_status(ent_create)) wait(1.0);
	c_setminmax(__act_player_state_singleton->__object_stand);
}

__static int __joy_force_x_to_int()
{
	if(joy_force.x)
		return 1;
	return 0;
}

__static int __joy_force_y_to_int()
{
	if(joy_force.y)
		return 1;
	return 0;
}

/*
 * void act_player()
 *
 * Player action. Should be assigned through WED.
 */
action act_player()
{
	ent_set_type(me, DYNAMIC_PLAYER);

	#ifdef ACKPHYSX_H
	game_log_write("Request to infiltrate the soul...");

	if(proc_status(act_player) > 1) {  // This is not a two-person game...
		game_log_write("More than one requests detected. Abort.");
		return;
	}

	BOOL    IS_GROUNDED = 0, SLEEP_ONCE = 0;
	VECTOR  force, absForce, dist, absDist, velocity, friction, tracePos, traceInput;
	fixed   footstep_handle = 0;

	act_player_new(); // Allocates and fills the singleton so we can fetch and set values from/to it.

	__act_player_state_singleton->__object = my;
	player = my;

	// This is not necessary and can obscure views sometimes.
	/*
	#ifndef    DEBUG
		my->flags |= (INVISIBLE);
	#endif
	*/
	my->flags |= (INVISIBLE | POLYGON);

	#ifdef    DEBUG
	   my->flags |= (SHADOW);
	#endif

	camera->arc = 75.0;
	my->OBJECT_TYPE = OBJECT_HERO;
	my->group = PLAYER_GROUP;
	my->push = PLAYER_GROUP;
	my->eflags |= FAT | NARROW;
	vec_set( &my->min_x, vector(-12, -12, -32));
	vec_set( &my->max_x, vector(24.0, 24.0, 192.0)); // This damn line of code took me a few months to figure out 'cause
                                                     // apparently the bounding box is so small that it "penetrates" through
                                                     // the door and/or slim entities.
                                                     // The values are a bit tricky to adjust correctly here -
                                                     // they not only have to handle level obstacles but also the level geometry.

	__act_player_register_physics();
	__act_player_create_bbox();
	while(proc_status(__act_player_create_bbox)) wait(1.0);

	while( __act_player_state_singleton->health > 0 )
	{
        while( player_lock )
            wait(1.0);

		if(__act_player_state_singleton->can_move)
		{
			if(__act_player_state_singleton->__move_type == MOVE_ON_FOOT)
			{
				if(SLEEP_ONCE)
				{
					__act_player_register_physics();
					SLEEP_ONCE = 0;
				}

				// get our velocity:
				pXent_getvelocity(my, &velocity, vector(0, 0, 0));
				// check if we are moving or not:
				var velResult = vec_length( &velocity );
				// get movement speed:
				__act_player_state_singleton->__quants_covered = velResult / 255;
				// reset moving variable:
				__act_player_state_singleton->is_moving = false;

				if(velResult > 10) // if we have movement speed:
					__act_player_state_singleton->is_moving = true; // then we are moving:

				// apply direction to the movement:
				vec_set( &force, vector(__act_player_state_singleton->move_speed * ((key_w || key_cuu || joy_force.y > 0) - (key_s || key_cud || joy_force.y < 0)), __act_player_state_singleton->move_speed * ((key_a || key_cul || joy_force.x < 0) - (key_d || key_cur || joy_force.x > 0)), 0) );

				// Applies the current force which was calculated by the key presses (W, A, S, D,...) to the "last" vector, which then can be used to measure the "delta" of the movement.
				vec_set(&(__act_player_state_singleton->__last_pos), &force);

				// Check if we're moving __on the ground__ (hence the absence of the z axis):
				// (TODO: Come up with some nicer checks, f.e. (first_pos - last_pos) > epsilon or something like that.)
				if((__act_player_state_singleton->__last_pos).x != (__act_player_state_singleton->__first_pos).x ||
				   (__act_player_state_singleton->__last_pos).y != (__act_player_state_singleton->__first_pos).y)
				{
					int r = rand() % FOOTSTEP_SOUND_VARIANTS;

					if( !snd_playing(footstep_handle) )
						footstep_handle = snd_play((__act_player_state_singleton->__footstep_sound)[r], game_volume * 0.2 , 0.0);
				}

				// if movement speed is more than allowed:
				if(vec_length(vector(force.x, force.y, 0)) > __act_player_state_singleton->move_speed)
				{
					// calculate some math crap:
					var len = sqrt(force.x * force.x + force.y * force.y);
					// lower the actual speed:
					force.x *= ((__act_player_state_singleton->move_speed) / len);
					force.y *= ((__act_player_state_singleton->move_speed) / len);
				}

				// if we are in the air:
				force.x *= (float) ifelse( !IS_GROUNDED, 0.4, 1.0 );
				force.y *= (float) ifelse( !IS_GROUNDED, 0.4, 1.0 );

                #ifdef DEBUG
				force.x *= (float) ifelse(key_shift || joy_6, __act_player_state_singleton->run_multiplier, __act_player_state_singleton->walk_multiplier);
				force.y *= (float) ifelse(key_shift || joy_6 , __act_player_state_singleton->run_multiplier, __act_player_state_singleton->walk_multiplier);
                #endif

				__act_player_scan_foot();
				// we are in the air:
				IS_GROUNDED = 0;
				if(__act_player_state_singleton->__distance_to_ground < 5.0 && __act_player_state_singleton->__distance_to_ground > -1.0) // if we are on the ground:
				    IS_GROUNDED = 1;

				// reset global X and Y movement:
				vec_set( &absForce, vector(0, 0, absForce.z));

				// if we are on the ground:
				if(IS_GROUNDED)
				{
					// if we didn't make it till the landing, and we stuck in jump or in_air state:
					if(__act_player_state_singleton->state == STATE_IN_AIR)
					    __act_player_state_singleton->state = STATE_IDLE;

					// set ground friction:
					__act_player_state_singleton->movement_friction = __act_player_state_singleton->movement_friction_ground;
					// reset absolute forces:
					absForce.z = 0;
				}
				else
				{
					// if we are high enough:
					if(__act_player_state_singleton->__distance_to_ground > 45 || force.z > 0)
					{
						// if we are simply falling:
						if(force.z <= 0)
						    __act_player_state_singleton->state = STATE_IN_AIR;
					}
					// airborne - reduce all relative forces
					// to prevent him from jumping or further moving in the air
					__act_player_state_singleton->movement_friction = __act_player_state_singleton->movement_friction_air;
					// reset jumping force:
					force.z = 0;
					// add the world gravity force:
					absForce.z = -GRAVITY_ACCELERATION;
				}
				// rotate movement with camera:
				vec_rotate( &force, vector(camera->pan, 0, 0));
				// accelerate the entity relative speed by the force vector:
				var accelerationVar = maxv((1 - time_step * __act_player_state_singleton->movement_friction), 0);
				// calculate velocity:
				my->SPEED_X = (time_step * force.x) + (accelerationVar * my->SPEED_X);
				my->SPEED_Y = (time_step * force.y) + (accelerationVar * my->SPEED_Y);
				my->SPEED_Z = (time_step * absForce.z) + (accelerationVar * my->SPEED_Z);
				// calculate relative distances to move:
				vec_set( &dist, vector(my->SPEED_X * time_step, my->SPEED_Y * time_step, 0) );
				// reset absolute distance to move:
				vec_set( &absDist, vector(0, 0, my->SPEED_Z * time_step) );
				// add the speed given by the ground elasticity and the jumping force:
				if(__act_player_state_singleton->__distance_to_ground  < 5)
				{
					// if the actor is standing on a moving platform, add it's horizontal displacement:
					absDist.x += __act_player_state_singleton->__platform_move_speed.x;
					absDist.y += __act_player_state_singleton->__platform_move_speed.y;
				}
				// get input for trace:
				vec_set( &traceInput, vector(((key_w || key_cuu || joy_force.y > 0) - (key_s || key_cud || joy_force.y < 0)), ((key_a || key_cul || joy_force.x < 0) - (key_d || key_cur || joy_force.x > 0)), 0));
				vec_rotate( &traceInput, vector(camera->pan, 0, 0));
				// save our position:
				vec_set(&tracePos, &my->x);
				// check for collisions and make corrections for X axis:
				tracePos.x += traceInput.x + (my->max_x * sign(traceInput.x));
				// ignore groups:
				c_ignore(PLAYER_GROUP, 0);
				// trace in the movement direction (X coordinates):
				result = c_trace(&my->x, &tracePos, (IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT) + ACTIVATE_SHOOT + IGNORE_FLAG2 + USE_BOX);
				if(trace_hit)
				    dist.x = my->SPEED_X = force.x = 0; // Reset input.

				// save our position:
				vec_set(&tracePos, &my->x);
				// check for collisions and make corrections for Y axis:
				tracePos.y += traceInput.y + (my->max_y * sign(traceInput.y));
				// ignore groups:
				c_ignore(PLAYER_GROUP, 0);
				// trace in the movement direction (Y coordinates):
				result = c_trace( &my->x, &tracePos, (IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT) + ACTIVATE_SHOOT + IGNORE_FLAG2 + USE_BOX);
				// if trace was hit:
				if(trace_hit)
				    dist.y = my->SPEED_Y = force.y = 0; // reset input.
				// if we can climb the ladder:
				if(__act_player_state_singleton->can_climb)
				{
					// save our position:
					vec_set(&tracePos, vector(my->x, my->y, my->z - 24));
					// ignore groups:
					c_ignore(PLAYER_GROUP, 0);
					// trace in the movement direction (Y coordinates):
					result = c_trace( &my->x, &tracePos, (IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT) + ACTIVATE_SHOOT + IGNORE_FLAG2 + USE_BOX);
				}
				// save our velocity into the friction vector:
				vec_set(&friction, vector(velocity.x, velocity.y, GRAVITY_ACCELERATION * 4));
				// inverse it:
				vec_inverse(&friction);
				// scale it up:
				vec_scale(&friction, __act_player_state_singleton->friction * time_step * (16.0 / 60));
				// add friction velocity:
				pXent_addvelcentral(my,  &friction);
				// add input movement velocity:
				pXent_addvelcentral(my, vector(dist.x, dist.y, absDist.z));
				// add absolute distance:
				pXent_move(my, nullvector, vector(absDist.x, absDist.y, 0));
				// we can stand up:
				__act_player_state_singleton->can_stand = true;
			}
			// if we are moving on the ladder:
			if(__act_player_state_singleton->__move_type == MOVE_ON_LADDER)
			{
				// switch to ladder state:
				__act_player_state_singleton->state = STATE_LADDER;
				// allow player to move up and down:
				force.z = __act_player_state_singleton->climb_speed * ((key_w || key_cuu || joy_force.y > 0) - (key_s || key_cud || joy_force.y < 0));
				// accelerate forces:
				accelerate(&absDist, force.z * time_step, __act_player_state_singleton->movement_friction_ground);
				// ignore groups:
				c_ignore(PLAYER_GROUP, 0);
				// no friction:
				move_friction = 0;
				// apply movement:
				c_move( my, nullvector, vector(0, 0, absDist.z), (IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT) + IGNORE_PUSH + GLIDE);
				// reset input XY:
				force.x = force.y = 0;
				absDist.x = absDist.y = 0;
				// reset movement vectors:
				vec_zero(absForce);
				vec_zero(dist);
				vec_zero(velocity);
				vec_zero(friction);
				vec_zero(tracePos);
				vec_zero(traceInput);
				vec_fill(&my->SPEED_X, 0);
			}
		}
		else
		{
			// if we can go to sleep:
			if(SLEEP_ONCE == 0)
			{
				// unregister us again:
				pXent_settype(my, NULL, NULL);
				// we are sleeping:
				SLEEP_ONCE = 1;
			}
			// reset everything:
			vec_zero(absForce);
			vec_zero(dist);
			vec_zero(absDist);
			vec_zero(velocity);
			vec_zero(friction);
			vec_zero(tracePos);
			vec_zero(traceInput);
			vec_zero(my->SPEED_X);
		}

		// stand shape exist:
		if(__act_player_state_singleton->__object_stand != NULL)
		{
			// move and rotate it with player:
			vec_set( &(__act_player_state_singleton->__object_stand->x), &my->x);
			vec_set( &(__act_player_state_singleton->__object_stand->pan), &my->pan);
		}

		// Update camera:
		__act_player_update_camera();

		wait(1.0);
	}

	act_player_free();

	#else
	    game_log_write("Header <ackphysX> wasn't included in the current build; request to control the soul ignored.");
	#endif
}

BOOL  act_player_can_stand()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->can_stand;
}

BOOL  act_player_can_climb()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->can_climb;
}

BOOL  act_player_can_move()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->can_move;
}

BOOL  act_player_is_moving()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->is_moving;
}

float act_player_get_move_speed()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->move_speed;
}

void  act_player_set_move_speed( float f )
{
	if(__act_player_state_singleton)
	    __act_player_state_singleton->move_speed = (float) ifelse(f, f, abs(f)); // Apparently 0 is not handled, though.
}

float act_player_get_climb_speed()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->climb_speed;
}

void  act_player_set_climb_speed( float f )
{
	if(__act_player_state_singleton)
	    __act_player_state_singleton->climb_speed  = (float) ifelse(f, f, abs(f));
}

float act_player_get_run_multiplier()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->run_multiplier;
}

void  act_player_set_run_multiplier( float f )
{
	if(__act_player_state_singleton)
	    __act_player_state_singleton->run_multiplier = (float) ifelse(f, f, abs(f));
}

float act_player_get_walk_multiplier()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->walk_multiplier;
}

void  act_player_set_walk_multiplier( float  f )
{
	if(__act_player_state_singleton)
	    __act_player_state_singleton->walk_multiplier = (float) ifelse(f, f, abs(f));
}

float act_player_get_friction()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->friction;
}

void  act_player_set_friction( float f )
{
	if(__act_player_state_singleton)
	    __act_player_state_singleton->friction = (float) ifelse(f, f, abs(f));
}

float act_player_get_camera_height()
{
	if(__act_player_state_singleton)
	    return __act_player_state_singleton->cam_height;
}

void  act_player_set_camera_height( float f )
{
	if(__act_player_state_singleton)
	    __act_player_state_singleton->cam_height = (float) ifelse(f, f, abs(f)); // TODO: plain wrong.
}

void act_player_camera_lock_toggle()
{
    __camera_locked = 1 - __camera_locked;
}

void act_player_camera_lock_to(int pos)
{
    if(!__camera_locked)
        act_player_camera_lock_toggle();

    view_set_custom(false);
    view_switch(pos);
}

void act_player_camera_unlock_to()
{
    if(__camera_locked)
        act_player_camera_lock_toggle();

    view_set_custom(true); // Notify that we're using our custom view (player camera).
}
