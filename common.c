/* common.c */
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
 * <common>
 * Place for common game code related to the game logic resides. 
 * Responsible for general-purpose game functionalities (scene loading, asset loading, ...)
 * and managing (loading/unloading) subsystems (physics, audio, logging,...).
 * 
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#define __LOVELY_21_HYPHENS "---------------------" // It's legit

/*
 * void game_state_new()
 * 
 * Allocates a new game state, and sets up the events for freeing the
 * state on game exit. This function must be called before 
 * any other subsystems can be initialized.
 */
void game_state_new()
{
	// Free the previously allocated game state, if it was allocated before.
	game_state_free();
	
	// Now initialize the state.
	__GameState_singleton = MALLOC(1, GameState);
	
	__GameState_singleton->__game_physx_loaded__ = false;
	__GameState_singleton->__game_log_loaded__   = false;
	__GameState_singleton->__game_log_handle__   = 0;
	
	// And call the event setup (so that the state and some other stuff gets automatically destroyed prior to exiting the game.)
	game_event_setup();
}

/*
 * void game_state_free()
 * 
 * Frees a previously allocated game state.
 */
void game_state_free()
{
	if( __GameState_singleton )
	{
		// Because the game state holds the "switch" variables for every subsystems, 
		// we have to free the subsystems first prior to freeing the game state itself.
		// Thus, calling game_state_free() effectively frees the subsystems.
		game_physx_free();
		game_log_free();
		
		// Now free the game state.
		FREE(__GameState_singleton);
	}
}

/*
 * void game_physx_new()
 * 
 * Initializes PhysX.
 */
void game_physx_new()
{
	game_log_write("Request initializing PhysX...");
	
	if( !__GameState_singleton->__game_physx_loaded__ )
	{
		game_log_write("PhysX hasn't been initialized. Initializing PhysX...");
		
		#ifdef ACKPHYSX_H 
		// Initializes the PhysX subsystem. Straight outta <ackPhysX>
		if( !NxPhysicsSDK )
		{
			dtimer();
			
			NxPhysicsSDK = physX_load();
			if( !NxPhysicsSDK ) {
				game_log_write("Failed initializing PhysX! Abort.");
				
				return;
			}
			
			pX_setsteprate(60.0, 8.0, NX_TIMESTEP_FIXED);
			NxScene = physX_run(0);
			pX_setunit(0.05);
			pX_setccd(1);
			
			#ifdef    _UTILITIES_H_ /* It's merely the same. */
		        pX_setgravity(vector(0, 0, -GRAVITY_ACCELERATION));
		   #else
		        pX_setgravity(vector(0, 0, -9.81));
		   #endif
		   
		   ph_fps_max_lock = 60.0;
		   ph_check_distance = 2;
		   
		   // Register additional events (can't be put into game_event_setup() - maybe we don't need unnecessary functions
		   // and (possible) crashs). ;)
		   game_log_write("Now registering PhysX events...");
		   
		   if( !on_exit )         on_exit = on_exit_px0;
		   if( !on_level_load )   on_level_load = on_level_load_px0;
		   if( !on_ent_remove )   on_ent_remove = on_ent_remove_px0;
		   
		   on_exit_px = on_exit;	// store previous on_exit function
		   on_exit = physX_close;
		   on_level_load_px = on_level_load;
		   on_level_load = physX_level_load;
		   on_ent_remove_px = on_ent_remove;
		   on_ent_remove = physX_ent_remove;
		   
		   __GameState_singleton->__game_physx_loaded__ = true;
		   
		   double d = dtimer();
		   game_log_write( str_printf(NULL, "Successfully initialized PhysX (%f seconds)", d*(10^6) ) );
		}
		#else
		    game_log_write("Header <ackphysX> wasn't included in the current build; initialization cancelled.");
		#endif
	}
}

/*
 * void game_physx_loop()
 *
 * Runs the PhysX simulation. For continuous physics simulation, you 
 * must place this function in a main loop whenever you have initialized PhysX or not.
 * Code taken from <ackPhysX> with a small modification; the algorithm provided in the 
 * manual is wrong and outdated.
 */
void game_physx_loop()
{
	#ifdef ACKPHYSX_H
	if(NxPhysicsSDK)
	{
		#ifdef FIXED_TIME //This for-loop fixes the physX framerate to 60 when the frame rate is below 60 fps
			int loc_n = integer(time_step * 60 / 16) + 1, loc_i = 0;
			
			if (freeze_mode <= 0)
			    for (; loc_i < loc_n; loc_i++)
			        physX_run(0);
			#else
			if (freeze_mode <= 0)		
			    physX_run(time_frame/16);
		#endif
		proc_mode = PROC_EARLY;	// call physX_run() before entity actions.
	}
	#else
	wait(1.0);
	#endif
}

/*
 * void game_physx_free()
 * 
 * Frees PhysX.
 */
void game_physx_free()
{
	game_log_write("Request to free the physics engine.");
	
	#ifdef ACKPHYSX_H
	if( __GameState_singleton->__game_physx_loaded__ )
	{
		game_log_write("Freeing the physics engine...");
		
		physX_destroy();
		// physX_close();
		__GameState_singleton->__game_physx_loaded__ = false;
		
		game_log_write("Physics engine freed.");
	}
	#else
	    game_log_write("Header <ackphysX> wasn't included in the current build; free operation cancelled.");
	#endif
}

/*
 * void game_log_new()
 * 
 * Opens the logging file.
 */
void game_log_new()
{
	if( !__GameState_singleton->__game_log_loaded__ && !__GameState_singleton->__game_log_handle__ )
	{
		__GameState_singleton->__game_log_handle__ = file_open_write(__LOG_FILE);
		__GameState_singleton->__game_log_loaded__ = (BOOL) ifelse(__GameState_singleton->__game_log_handle__, true, false);
		
		if( __GameState_singleton->__game_log_loaded__ )
		{
			game_log_write(__LOVELY_21_HYPHENS); // It's legit
			game_log_write("Logging channel opened.");
		}
	}
}

/*
 * void game_log_free()
 * 
 * Frees the logging file. Note that, because the file is opened in write mode 
 * (not append!), any other calls to game_log_new() after game_log_free() 
 * will cause the old log file to be erased.
 * 
 * TODO: Append instead of a complete erase.
 */
void game_log_free()
{
	if( __GameState_singleton->__game_log_loaded__ )
	{
		game_log_write("Logging channel closed.");
		game_log_write(__LOVELY_21_HYPHENS); // It's legit
		
		file_close(__GameState_singleton->__game_log_handle__);
		__GameState_singleton->__game_log_loaded__ = false;
	}
}

/*
 * void game_log_write(const STRING *content)
 * 
 * Writes a string to the log file.
 * Information about time and date are automatically appended.
 * 
 * For writing with printf(), refer to str_printf().
 */
void game_log_write(const STRING *content)
{
	if( __GameState_singleton->__game_log_loaded__ )
	{
		// Straight outta <utilities>
		char *cstr = MALLOC(256, char);
		sprintf( cstr, "[%i:%i, %i.%i.%i] %s",
		(int) sys_hours,
		(int) sys_minutes,
		(int) sys_day,
		(int) sys_month,
		(int) sys_year,
		ifelse(content, _chr(content), _chr("Undescribed log message.")) );
		
		file_str_write(__GameState_singleton->__game_log_handle__, cstr);
		file_asc_write(__GameState_singleton->__game_log_handle__, 13);
		file_asc_write(__GameState_singleton->__game_log_handle__, 10);
		
		FREE(cstr);
	}
}

__static void __game_event_on_exit()
{
	// game_log_write( str_printf(NULL, "Triggering the pre-terminate event (%s))", function_name_get(__game_event_on_exit)) );
	game_log_write( "Triggering the pre-terminate event (__game_event_on_exit()))" );
	
	game_state_free();
}

__static void __game_event_on_close()
{
	__game_event_on_exit();
	
	sys_exit(NULL);
}

/*
 * void game_event_setup()
 * 
 * Sets up certain events that are automatically invoked later.
 */
void game_event_setup()
{
	on_close         = __game_event_on_close;
	on_exit          = __game_event_on_exit;
}
