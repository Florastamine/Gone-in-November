action act_player()
{
	#ifdef ACKPHYSX_H
	game_log_write("Request to infiltrate the soul...");
	
	if(proc_status(act_player) > 1) {  // This is not a two-person game...
		game_log_write("More than one requests detected. Abort.");
		return;
	}
	
	BOOL    IS_GROUNDED = 0, SLEEP_ONCE = 0;
	VECTOR  force, absForce, dist, absDist, velocity, friction, tracePos, traceInput;
	
	act_player_new(); // Allocates and fills the singleton so we can fetch and set values from/to it.
	
	__act_player_state_singleton->__object = my;
	player = my;
	
	my->flags |= (TRANSLUCENT);
	my->OBJECT_TYPE = OBJECT_HERO;
	my->group = PLAYER_GROUP;
	my->push = PLAYER_GROUP;
	my->eflags |= FAT | NARROW;
	vec_set( &my->min_x, vector(-6, -6, -16));
	vec_set( &my->max_x, vector(6, 6, 24));
	
	__act_player_register_physics();
	__act_player_create_bbox();
	while(proc_status(__act_player_create_bbox)) wait(1.0);
	
	while( __act_player_state_singleton->health > 0 )
	{
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
				vec_set( &force, vector(__act_player_state_singleton->move_speed * (key_w - key_s), __act_player_state_singleton->move_speed * (key_a - key_d), 0) );
				
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
				
				force.x *= (float) ifelse(key_shift, __act_player_state_singleton->run_multiplier, __act_player_state_singleton->walk_multiplier);
				force.y *= (float) ifelse(key_shift, __act_player_state_singleton->run_multiplier, __act_player_state_singleton->walk_multiplier);
				
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
				vec_set( &traceInput, vector((key_w - key_s), (key_a - key_d), 0));
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
				force.z = __act_player_state_singleton->climb_speed * (key_w - key_s);
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
		if(__act_player_state_singleton->__object_stand != NULL){
			// move and rotate it with player:
			vec_set( &(__act_player_state_singleton->__object_stand->x), &my->x);
			vec_set( &(__act_player_state_singleton->__object_stand->pan), &my->pan);
		}
		// attach camera:
		__act_player_update_camera();
		wait(1);
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
	    __act_player_state_singleton->cam_height = (float) ifelse(f, f, abs(f));
}
