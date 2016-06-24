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
__static void __write_newline( fixed fHandle, int times )
{
	if( fHandle )
	{
		int i = 0;
		for(; i < times; i++)
		{
			file_asc_write(fHandle, 13);
			file_asc_write(fHandle, 10);
		}
	}
}

__static void __write_newline( fixed fHandle )
{
	__write_newline(fHandle, 1);
}

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
	__GameState_singleton->menu_switch           = 0;
	__GameState_singleton->exit_switch           = 0;

	// Also initialize the objectives singleton.
	__GameObjectives_singleton = MALLOC(1, GameObjectives);

	__GameObjectives_singleton->objectives         = 0;
	__GameObjectives_singleton->total_objectives   = 0;

	// And call the event setup (so that the state and some other stuff gets automatically destroyed prior to exiting the game.)
	game_event_setup();

	// Parsing launch options
	game_args_parse();
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

	if( __GameObjectives_singleton )
		FREE(__GameObjectives_singleton);
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

		#ifdef    ACKPHYSX_H
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
		   game_log_write( str_printf(NULL, "Successfully initialized PhysX (%f seconds)", d * POW_10_6 ) );
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
		__GameState_singleton->__game_log_handle__ = file_open_append(__LOG_FILE);
		__GameState_singleton->__game_log_loaded__ = (BOOL) ifelse(__GameState_singleton->__game_log_handle__, true, false);

		if( __GameState_singleton->__game_log_loaded__ )
		{
			game_log_write(__LOVELY_21_HYPHENS); // It's legit
			game_log_write( str_printf(NULL, "Logging channel opened on %s", os_get_name()) );
		}
	}
}

/*
 * void game_log_free()
 *
 * Frees the logging file. Note that, because the file is opened in write mode
 * (not append!), any other calls to game_log_new() after game_log_free()
 * will cause the old log file to be erased.
 */
void game_log_free()
{
	if( !__GameState_singleton )
		return;

	if( __GameState_singleton->__game_log_loaded__ )
	{
		game_log_write("Logging channel closed.");
		game_log_write(__LOVELY_21_HYPHENS); // It's legit
		__write_newline(__GameState_singleton->__game_log_handle__, 2);

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
	if( !__GameState_singleton )
		return;

	if( __GameState_singleton->__game_log_loaded__ )
	{
		// Straight outta <utilities>
		char *cstr = MALLOC(str_len(content) + 93, char);
		sprintf( cstr, "[%i:%i, %i.%i.%i] %s",
		(int) sys_hours,
		(int) sys_minutes,
		(int) sys_day,
		(int) sys_month,
		(int) sys_year,
		ifelse(content, _chr(content), _chr("Undescribed log message.")) );

		file_str_write(__GameState_singleton->__game_log_handle__, cstr);
		__write_newline(__GameState_singleton->__game_log_handle__);

		FREE(cstr);
	}
}

__static void __game_event_on_exit()
{
	// game_log_write( str_printf(NULL, "Triggering the pre-terminate event (%s))", function_name_get(__game_event_on_exit)) );
	game_log_write( "Triggering the pre-terminate event (__game_event_on_exit()))" );

	// Frees the game state and its subsystems.
	game_state_free();

	// Frees the buffer holding the (loaded) game resources.
	game_resources_free();

	// Frees the localized strings.
	localized_free();

	// Frees the multilingual module.
	region_free();

	// Frees the music player.
	game_mplayer_free();

	// Frees static-initialized stuff.
	game_static_free();
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

	#ifdef    __FSAA_DLL_H__
		render_sky           = fsaa_eventSky;
		render_entities      = fsaa_eventEntities;
		on_d3d_lost          = fsaa_eventLost;
	#endif
}

__static void __p_set_translucent( PANEL *p )
{
	if(p)
	    if( !(p->flags & TRANSLUCENT) )
	        p->flags |= (TRANSLUCENT);
}

__static void __p_fade_in( PANEL *p, float f, float t, float s )
{
	if(p) {
		if( f > t ) swap( &f, &t );
		__p_set_translucent(p);

		p->alpha = f;
		while(p->alpha < t)
		{
			p->alpha += s * time_step;

			wait(1.0);
		}
	}
}

__static void __p_fade_out( PANEL *p, float f, float t, float s )
{
	if(p) {
		if( t > f ) swap( &f, &t );
		__p_set_translucent(p);

		p->alpha = f;
		while(p->alpha > t)
		{
			p->alpha -= s * time_step;

			wait(1.0);
		}
	}
}

__static void __game_scene_set_defaults()
{
	pair_set( &__SceneLoadState_singleton->load_text_pos, screen_size.x / 2, screen_size.y - 150.0 );
	pair_set( &__SceneLoadState_singleton->desc_text_pos, screen_size.x / 2, screen_size.y - 250.0 );
	pair_set( &__SceneLoadState_singleton->load_img_pos, 0.0, 0.0 );

	__SceneLoadState_singleton->load_img = bmap_createblack(screen_size.x, screen_size.y, 8); // 8-bit black bitmap
	__SceneLoadState_singleton->load_text = str_create("Loading...");
	__SceneLoadState_singleton->desc_text = str_create("The quick brown fox jumps over the lazy dog...");

	__SceneLoadState_singleton->error      = 0;
	__SceneLoadState_singleton->delay      = 0.0;
	__SceneLoadState_singleton->fade       = false;
	__SceneLoadState_singleton->fade_speed = 10.0;

	__SceneLoadState_singleton->load_text_font = font_create("Arial#15b");
	__SceneLoadState_singleton->desc_text_font = font_create("Arial#15b");
}

// Users don't need to care about allocating __SceneLoadState_singleton
__static void __game_scene_state_new()
{
	if( !__SceneLoadState_singleton )
	{
		game_log_write("First time loading a scene, initializing scene state singleton...");

		__SceneLoadState_singleton             = MALLOC(1, SceneLoadState);

		__game_scene_set_defaults();
	}
}

__static void __game_scene_state_free()
{
	game_log_write("Request to free the scene state singleton...");

	if( __SceneLoadState_singleton )
	{
		bmap_remove(__SceneLoadState_singleton->load_img);
		str_remove(__SceneLoadState_singleton->load_text);
		str_remove(__SceneLoadState_singleton->desc_text);
		font_remove(__SceneLoadState_singleton->load_text_font);
		font_remove(__SceneLoadState_singleton->desc_text_font);

		FREE(__SceneLoadState_singleton);

		game_log_write("Scene state singleton freed.");
	}
}

void game_scene_set_defaults()
{
	__game_scene_set_defaults();
}

void game_scene_set_load_text_pos(float x, float y)
{
	__game_scene_state_new();

	pair_set( &__SceneLoadState_singleton->load_text_pos, x, y );
}

void game_scene_set_desc_text_pos(float x, float y)
{
	__game_scene_state_new();

	pair_set( &__SceneLoadState_singleton->desc_text_pos, x, y );
}

void game_scene_set_load_img_pos(float x, float y)
{
	__game_scene_state_new();

	pair_set( &__SceneLoadState_singleton->load_img_pos, x, y );
}

void game_scene_set_load_screen( STRING *load_img )
{
	__game_scene_state_new();

	if(file_exists(load_img))
	    __SceneLoadState_singleton->load_img = bmap_create(load_img);
}

BMAP *game_scene_get_load_screen()
{
	if(__SceneLoadState_singleton)
	{
		if(__SceneLoadState_singleton->load_img)
		    return __SceneLoadState_singleton->load_img;
	}

	return 0;
}

void game_scene_set_load_text( STRING *load_text )
{
	__game_scene_state_new();

	if(load_text)
	    str_cpy( __SceneLoadState_singleton->load_text, load_text );
}

STRING *game_scene_get_load_text()
{
	if(__SceneLoadState_singleton)
	{
		if(__SceneLoadState_singleton->load_text)
		    return __SceneLoadState_singleton->load_text;
	}

	return str_create("NULL"); // ;). A string containing "NULL" is far more surprise than just a NULL-ed string.
}

void game_scene_set_desc_text( STRING *desc_text )
{
	__game_scene_state_new();

	if(desc_text)
	    str_cpy( __SceneLoadState_singleton->desc_text, desc_text );
}

STRING *game_scene_get_desc_text()
{
	if(__SceneLoadState_singleton)
	{
		if(__SceneLoadState_singleton->desc_text)
		    return __SceneLoadState_singleton->desc_text;
	}

	return str_create("NULL");
}

void game_scene_set_load_text_font( FONT *font_struct )
{
	__game_scene_state_new();

	__SceneLoadState_singleton->load_text_font = font_struct;
}

void game_scene_set_load_text_font( STRING *font_str )
{
	game_scene_set_load_text_font(font_create(font_str));
}

FONT *game_scene_get_load_text_font()
{
	if(__SceneLoadState_singleton)
	{
		if(__SceneLoadState_singleton->load_text_font)
		    return __SceneLoadState_singleton->load_text_font;
	}

	return NULL;
}

void game_scene_set_desc_text_font( FONT *font_struct )
{
	__game_scene_state_new();

	__SceneLoadState_singleton->load_text_font = font_struct;
}

void game_scene_set_desc_text_font( STRING *font_str )
{
	game_scene_set_desc_text_font(font_create(font_str));
}

FONT *game_scene_get_desc_text_font()
{
	if(__SceneLoadState_singleton)
	{
		if(__SceneLoadState_singleton->load_text_font)
		    return __SceneLoadState_singleton->load_text_font;
	}

	return NULL;
}

void game_scene_set_delay( float d )
{
	if(__SceneLoadState_singleton)
	    __SceneLoadState_singleton->delay = (float) abs(d);
}

float game_scene_get_delay()
{
	if(__SceneLoadState_singleton)
	    return __SceneLoadState_singleton->delay;
}

void game_scene_set_fade( BOOL b )
{
	if(__SceneLoadState_singleton)
	    __SceneLoadState_singleton->fade = b;
}

void game_scene_set_fade_speed( float s )
{
	if(__SceneLoadState_singleton)
	    __SceneLoadState_singleton->fade_speed = s;
}

float game_scene_get_fade_speed()
{
	if(__SceneLoadState_singleton)
	    return __SceneLoadState_singleton->fade_speed;
}

BOOL  game_scene_is_fade()
{
	if(__SceneLoadState_singleton)
	    return __SceneLoadState_singleton->fade;
}

/*
 * void game_scene_load( STRING *scene )
 *
 * A level_load() wrapper with some GUI elements like loading screen and text.
 * First, set up the various GUI elements by calling the game_scene_set*() functions.
 * This function mimics OpenGL style: Everything is stored in a state object, meaning
 * if the data set is kept unchanged, or if you only want to change a small portion of the state,
 * this normally could be resolved to only one function call, rather than a large ass function
 * with lots and lots of parameters.
 * And you can even query the state using get() functions.
 */
void game_scene_load( STRING *scene )
{
	game_log_write(str_printf(NULL, "Request to loading a scene... <%s>", _chr(scene)));
	if( !file_exists(scene) )
	{
		game_log_write("Scene doesn't exist! Abort.");
		return;
	}

	double load_time = .0; // Measure level_load() time.

	__game_scene_state_new();

	PANEL *p  = pan_create(NULL, 999);
	TEXT  *t0 = txt_create(1, 1000);
	TEXT  *t1 = txt_create(1, 1000);

	// Feed the structs' data with the ones fed from the singleton.
	p->bmap = game_scene_get_load_screen();
	p->pos_x = (__SceneLoadState_singleton->load_img_pos).first;
	p->pos_y = (__SceneLoadState_singleton->load_img_pos).second;
	p->scale_x = screen_size.x / bmap_width(p->bmap);
	p->scale_y = screen_size.y / bmap_height(p->bmap);

	str_cpy((t0->pstring)[0], game_scene_get_load_text());
	str_cpy((t1->pstring)[0], game_scene_get_desc_text());

	t0->pos_x = (__SceneLoadState_singleton->load_text_pos).first;
	t0->pos_y = (__SceneLoadState_singleton->load_text_pos).second;
	t0->font  = game_scene_get_load_text_font();

	t1->pos_x = (__SceneLoadState_singleton->desc_text_pos).first;
	t1->pos_y = (__SceneLoadState_singleton->desc_text_pos).second;
	t1->font  = game_scene_get_desc_text_font();

	p->flags  |= (SHOW);
	t1->flags |= (SHOW);
	t0->flags |= (SHOW);

	if(game_scene_is_fade())
	{
		__p_fade_in(p, 0.0, 100.0, game_scene_get_fade_speed());
		while(proc_status(__p_fade_in) > 1) wait(1.0); // Block the local static function (because of wait(1) could allow
		                                           // level_load() to be executed at the same time due to concurrency).
	}

	dtimer();
	level_load(scene);
	while(proc_status(level_load)) wait(1.0); // Temporary blocking the main thread
	load_time = dtimer();

	wait(1); // So last_error can become valid
	__SceneLoadState_singleton->error = last_error;
	last_error = 0;

	if( game_scene_get_delay() > 0.0 )
	wait( - __SceneLoadState_singleton->delay ); // Note the minus sign - we want to wait in seconds.

	t1->flags &= ~(SHOW);
	t0->flags &= ~(SHOW);

	if(game_scene_is_fade())
	{
		__p_fade_out(p, 100.0, 0.0, game_scene_get_fade_speed());
		while(proc_status(__p_fade_out)) wait(1.0);
	}

	p->flags  &= ~(SHOW);

	// De-allocate resources.
	txt_remove_ex(t0);
	txt_remove_ex(t1);
	pan_remove(p);

	game_log_write( str_printf(NULL, "Scene <%s> loaded. Operation took %f seconds with %i errors.", _chr(scene), load_time * POW_10_6, __SceneLoadState_singleton->error) );
}

/*
 * void game_mplayer_new( const STRING *path, const STRING *extension )
 *
 * Allocates and initializes a new music player.
 * The reason for not including the check for the NULL/!NULL singleton is, sometimes you have to re-scan the music folder
 * for changes, or you may have to switch to another scan path, then you can just call game_mplayer_new() with an argument
 * specifying the (new) scan path.
 */
void game_mplayer_new( const STRING *path, const STRING *extension )
{
	game_mplayer_free();

	game_log_write("Request to allocate a new stream player object...");

	if( !path )
	    path = MPLAYER_DEFAULT_DIRECTORY;
	else // Check for a faulty path
	{
		path = str_create(path); // Workaround for the "invalid arguments" issue.

		char separator = str_getchr(path, str_len(path));
		if( separator != '/' && separator != '\\' )
		    str_cat(path, "/");
	}

	if( !extension )
	    extension = str_create(MPLAYER_DEFAULT_EXTENSION);
	else
	    extension = str_create(extension); // Same as above.

	MPlayer_singleton               = MALLOC(1, MPlayer);
	MPlayer_singleton->handle       = 0;
	MPlayer_singleton->pos          = 1;
	MPlayer_singleton->randomize    = false;
	MPlayer_singleton->volume       = midi_vol * 0.8;
	MPlayer_singleton->__search_path = str_create(path);

	MPlayer_singleton->track_list   = txt_create(MPLAYER_BUFFER_SIZE, LAYER_DEBUG_1);
	MPlayer_singleton->total_tracks = txt_for_dir( MPlayer_singleton->track_list, str_cat(path, extension) );

	#ifdef    DEBUG
	    MPlayer_singleton->track_list->font = font_create("Times#17");
	#endif

	if( MPlayer_singleton->total_tracks ) // Save us some time, because later we don't need to concatenate the directory
	                                      // for each call to game_mplayer_(prev/next/...)(), it's there already.
	{
		game_log_write( str_printf(NULL, "Found %i tracks during scanning.", MPlayer_singleton->total_tracks) );

		int j = 0;
		STRING *s = "";
		for(; j < MPlayer_singleton->total_tracks; j++)
		{
			str_cpy(s, MPlayer_singleton->__search_path);
			str_cat(s, (MPlayer_singleton->track_list->pstring)[j]);
			str_cpy((MPlayer_singleton->track_list->pstring)[j], s);
		}
	}
	else
	{
		game_log_write("The specified directory is either not existed or it is empty. Abort. (Further calls to game_mplayer*() functions could cause a crash!)");

		game_mplayer_free();
	}
}

void game_mplayer_new( const STRING *extension )
{
	game_mplayer_new(MPLAYER_DEFAULT_DIRECTORY, extension);
}

void game_mplayer_new()
{
	game_mplayer_new(MPLAYER_DEFAULT_DIRECTORY, MPLAYER_DEFAULT_EXTENSION);
}

/*
 * void game_mplayer_free()
 *
 * Frees the player singleton.
 * This is not needed if you just want to re-initialize the player.
 */
void game_mplayer_free()
{
	if(MPlayer_singleton)
	{
		game_log_write("Freeing the previously allocated stream player...");

		txt_remove_ex(MPlayer_singleton->track_list);
		str_remove(MPlayer_singleton->__search_path);

		FREE(MPlayer_singleton);
	}
}

/*
 * void game_mplayer_set_pos( int pos )
 *
 * Sets the player's position to a specific position, in the range of [1; game_mplayer_get_total_tracks()].
 */
void game_mplayer_set_pos( int pos )
{
	if(MPlayer_singleton->pos <= MPlayer_singleton->total_tracks && (int) clamp(pos, 1, MPlayer_singleton->total_tracks) == pos )
	    MPlayer_singleton->pos = pos;
}

BOOL game_mplayer_get_randomize()
{
	if(MPlayer_singleton)
	    return MPlayer_singleton->randomize;
}

void game_mplayer_set_randomize( BOOL b )
{
	if(MPlayer_singleton)
	    MPlayer_singleton->randomize = b;
}

int   game_mplayer_get_pos()
{
	if(MPlayer_singleton)
	    return MPlayer_singleton->pos;
}

fixed game_mplayer_get_handle()
{
	if(MPlayer_singleton)
	    return MPlayer_singleton->handle;
}

int   game_mplayer_get_total_tracks()
{
	if(MPlayer_singleton)
	    return MPlayer_singleton->total_tracks;
}

float game_mplayer_get_volume()
{
	if(MPlayer_singleton)
	    return MPlayer_singleton->volume;
}

void  game_mplayer_set_volume( float vol )
{
	if(MPlayer_singleton)
	    MPlayer_singleton->volume = abs(vol);
}

void game_mplayer_next()
{
	if(MPlayer_singleton)
	{
		if( game_mplayer_get_pos() < game_mplayer_get_total_tracks() )
		{
			MPlayer_singleton->pos =  game_mplayer_get_pos() + 1;
			game_mplayer_play();
		}
	}
}

void game_mplayer_prev()
{
	if(MPlayer_singleton)
	{
		if(game_mplayer_get_pos() > 1)
		{
			MPlayer_singleton->pos =  game_mplayer_get_pos() - 1;
			game_mplayer_play();
		}
	}
}

void game_mplayer_pause()
{
	if(MPlayer_singleton)
	{
		if( media_playing(game_mplayer_get_handle()) )
			media_pause(game_mplayer_get_handle());
	}
}

void game_mplayer_play()
{
	game_mplayer_stop();
	MPlayer_singleton->handle = media_play((MPlayer_singleton->track_list->pstring)[MPlayer_singleton->pos - 1], NULL, game_mplayer_get_volume());
}

void game_mplayer_play( int id )
{
	game_mplayer_set_pos(id);
	game_mplayer_play();
}

void game_mplayer_play( STRING *substr )
{
	if( !substr )
	    return;

	substr = str_create(substr);

	int i = 0;
	for(; i < MPlayer_singleton->track_list->strings; i++)
	    if( str_stri((MPlayer_singleton->track_list->pstring)[i], substr) )
	        break;

	game_mplayer_play(i + 1);
}

void game_mplayer_stop()
{
	if( media_playing(game_mplayer_get_handle()) )
		media_stop(game_mplayer_get_handle());
}

/*
 * void game_args_parse()
 *
 * Opens the argument list file and parses it.
 * This is because command_str is broken, otherwise we can use it and it would
 * simplifies parsing a lot.
 */
void game_args_parse()
{
	if( !file_exists(__ARGS_LIST) )
	    return;

	STRING *args = str_create(dump(__ARGS_LIST));

	if( !str_stri(args, __ARGS_NO_LOGGING) && !str_stri(args, __ARGS_NO_LOGGING_SHORT) ) // If __ARGS_NO_LOGGING wasn't specified?
	{
		game_log_new(); // Open the logging stream.

		game_log_write(str_printf(NULL, "ID: <%i>", (int) GetCurrentProcessId()));
		game_log_write(str_printf(NULL, "Engine release %f", (double) version));
		game_log_write(str_printf(NULL, "Game release %s", (char *) __GAME_VERSION));

		#ifdef    __FSAA_DLL_H__
			game_log_write(str_printf(NULL, "Video card anti-aliasing level %i", (int) fsaa_maxQualityNonMaskable()));
		#endif

		if(num_joysticks)
			game_log_write( str_printf(NULL, "Found %i gamepad device(s).", (int) num_joysticks) );
	}

	#ifdef    DEBUG
	    game_log_write( str_printf(NULL, "[DEBUG] Arguments passed: %s", _chr(args)) );
	#endif

	game_log_write(str_printf(NULL, "Trying to parse the argument list located in <%s>...", __ARGS_LIST));

	if( str_stri(args, __ARGS_CONSOLE) || str_stri(args, __ARGS_CONSOLE_SHORT) )
	{
		game_log_write(str_printf(NULL, "\"%s\" found. Opening the console...", __ARGS_CONSOLE));

		game_console_load();
	}

	if( str_stri(args, __ARGS_FORCE_LOW) || str_stri(args, __ARGS_FORCE_LOW_SHORT) )
	{
		game_log_write(str_printf(NULL, "\"%s\" found. Will bypass video card checking and does not render shaders.", __ARGS_FORCE_LOW));
	}
	else
	{
		if( !game_psvs_test() )
		    game_log_write("Your video card does not meet the minimum requirements for running the game. As a result, shaders and particular effects will not be properly rendered.");
	}

	if( str_stri(args, __ARGS_DECRYPTOR) || str_stri(args, __ARGS_DECRYPTOR_SHORT) )
	{
		game_log_write(str_printf(NULL, "\"%s\" found. Open the save game editor instead.", __ARGS_DECRYPTOR));
	}
}

/*
 * BOOL game_psvs_test()
 *
 */
BOOL game_psvs_test()
{
	if( d3d_shaderversion >= __PSVS_VERSION )
	    return true;
	return false;
}

__static void __game_video_set(
	float width,
	float height,
	int   bit_depth,
	bool  fullscreen,
	int   fsaa,
	int   fsaf /* ... */
) {
	if( var_cmp(width, -1) == true )
		width = sys_metrics(0);
	if( var_cmp(height, -1) == true )
		height = sys_metrics(1);

	video_set(width, height, bit_depth, ifelse(fullscreen, FULLSCREEN, WINDOWED));
	d3d_antialias     = fsaa;

	if(fsaf > 0)
	{
		d3d_anisotropy    = fsaf;
		d3d_mipmapping    = 4;
	}
}

/*
 * void game_video_cfg_parse()
 *
 * Reads and applies video settings from the video configuration file
 * defined in <common.h>/__VIDEO_CFG.
 */

__static int __FSAA_GetQuality() {
	#ifdef    __FSAA_DLL_H__
		return fsaa_maxQualityNonMaskable();
	#endif

	return 4; // 4 is enough. 8 or 9 for d3d_antialias is too risky. For cards
	          // with the maximum anti-aliasing level belows 8 - 9, setting this variable
			  // to 8 or 9 could effectively disables AA.
}

void game_video_cfg_parse()
{
	fixed f = file_open_read(__VIDEO_CFG);

	if(!f) // Set default values if the configuration file couldn't be found.
		__game_video_set( sys_metrics(0), sys_metrics(1), 32, true, __FSAA_GetQuality(), 8 );
	else
	{
		String *content              = "";
		ArrayContainer *settings     = array_container_new();
		float width, height, bit_depth, fullscreen, aa, af;

		file_str_readto( f, content, "\\", 128 );
		num_parse_delim( settings, content );

		// Pay a little performance for extra readability, I won't risk anything.
		width      = array_container_get_next(settings);
		height     = array_container_get_next(settings);
		bit_depth  = array_container_get_next(settings);
		fullscreen = array_container_get_next(settings);
		aa         = array_container_get_next(settings);
		af         = array_container_get_next(settings);

		__game_video_set(width, height, bit_depth, fullscreen, aa, af);

		file_close(f);
		array_container_free(settings);
	}
}

/*
 * void game_title_set()
 *
 * Sets the game window's title, in case the .wdf doesn't exist.
 */
void game_title_set()
{
	String *s = str_create("Gone In November ");
	str_cat(s, __GAME_VERSION);

	#ifdef    DEBUG
		str_cat(s, "-DEBUG");
	#endif

	video_window( NULL, NULL, 0, s );
	str_remove(s);
}

__static void __game_version_export()
{
	#ifdef    DEBUG
		fixed f = file_open_write(__VER_FILE);
		file_str_write(f, __GAME_VERSION);
		file_close(f);
	#endif

	return;
}

/*
 * void game_fog_set(int ID, VECTOR *color, VECTOR *range)
 *
 * Sets the fog color, and range.
 */
#define    __FOG_ID(x) d3d_fogcolor##x
void game_fog_set(int ID, VECTOR *color, VECTOR *range)
{
	ID = ifelse(ID > 0 && ID < 4, ID, 1); // We have d3d_fogcolor1..d3d_fogcolor4.

	switch(ID)
	{
		case 1: vec_set(&d3d_fogcolor1, color); break;
		case 2: vec_set(&d3d_fogcolor2, color); break;
		case 3: vec_set(&d3d_fogcolor3, color); break;
		case 4: vec_set(&d3d_fogcolor4, color); break;
	}

	camera->fog_start = ifelse(range->x > 0, range->x, 10.0);
	camera->fog_end   = ifelse(range->y > 0, range->y, 1250.0);
}

void game_fog_render(int ID)
{
	fog_color = ifelse(ID > 0 && ID < 4, ID, 1);
}

/*
 * Fail naming convention, fail coding structures, fail #ifdef-s,
 * fail design, fail everything. Sorry.
 */
#define EXECUTE_FUNCTION_POINTER(ptr) ptr(); WAIT_PROCESS(ptr)

__static void __save_load( char *file, void *pre, void *post, int op )
{
	void func();

	if( pre )
	{
		func = pre;
		EXECUTE_FUNCTION_POINTER(func);
	}

	if(op)
	{
		#ifndef    PERFORM_SERIALIZATION    // Use the built-in saving/loading functions instead.
		                                    // (dangerous and couldn't work properly most of the time).
			game_save( game_asset_get_save(file), 1, SV_ALL);
		#else
			#ifdef    DEBUG
				printf("Built-in serializer used.");
			#endif
		#endif
	}
	else
	{
		#ifndef    PERFORM_SERIALIZATION
			game_load( game_asset_get_save(file), 1);
		#else
			#ifdef    DEBUG
				printf("Built-in serializer used.");
			#endif
		#endif
	}

	if( post )
	{
		func = post;
		EXECUTE_FUNCTION_POINTER(func);
	}
}

/*
 * void save(  char *file,  void *pre,  void *post )
 *
 * Performs game saving.
 * To be honest, I wouldn't rely on game_save() and game_load() calls but
 * use my home brew serializer/deserializer instead. (but it isn't stable either...)...
 * These will soon be replaced by my serializer code, with an additional option to switch
 * to the legacy functions through disabling PERFORM_SERIALIZATION.
 */
void save(  char *file,  void *pre,  void *post )
{
	__save_load(file, pre, post, 1);
}

/*
 * void load(  char *file,  void *pre,  void *post )
 *
 * Performs game loading.
 */
void load(  char *file,  void *pre,  void *post )
{
	__save_load(file, pre, post, 0);
}

/*
 * void game_globals_set()
 *
 * Overrides some of Gamestudio's default variables, like raising the minimum particles
 * and effects count (default values are way too low).
 * This function must be called at the __VERY__ beginning of main(), even before the ready() check.
 */
void game_globals_set()
{
	max_paths = 4096;
	max_particles = 16777216;
	max_entities = 65536; // By default, max_entities equals to nexus * 10.
								// We raise that limit 12.8 times, i.e. nexus * 128.
	clip_particles = 0.7;
	preload_mode = 2 + 3 + 4 + 8;
	mip_levels = 6;
	sky_blend = 1;
	tex_share = 1;
	terrain_chunk = 16;
	terrain_lod = 4;
	detail_size = 80;
	time_smooth = 0.9;
	d3d_shadowdepth = 32;
	d3d_texdepth = 32;
	random_seed(0);

	// Enable standard stencil shadows if the header <render_shadows> isn't included.
	#ifndef    __RENDER_SHADOWS_H__
		shadow_stencil = 2;
	#endif

	#ifdef    DEBUG
		warn_level = 6;
	#else
		warn_level = 0;
	#endif
}

/*
 * void game_console_load()
 *
 * Initializes and fills the custom console with commands, if __ARGS_CONSOLE was specified in the argument file.
 * The user/player can then press [F11] to switch on the console (type "exit" to switch off the console).
 * Command implementations can be found in <./source/game/common/cfuncs>.
 */
void game_console_load()
{
	command_table_new(); // Initialize the internal console interface.

	command_table_add("skyrim", __cfunc__skyrim);
}

/*
 * game_resources_load()
 *
 * Loads the game resources' file. During development, resources are not packed into a single file
 * but scattered across different folders and repositories, but in the completed game, resource files
 * are used instead. .wrs format files are automatically loaded during startup. But again, if you want
 * to use custom extensions, you'll have to manually load it with add_resource().
 */
void game_resources_load()
{
	return;
}

/*
 * void game_resources_free()
 *
 * Frees the previously allocated game resources buffer.
 */
void game_resources_free()
{
	return;
}

/*
 * int game_locker_check()
 *
 * A simple function to test if the game was launched through the launcher.
 * For: 1/ simplicity; 2/ Lite-C doesn't have full support for the facilities and libraries needed for
 * checking if a given process is running; and 3/ I don't like Windows and the whole Windows API in general:
 * I'll just test for a file created by the launcher when it's running (the "locker"). If the file doesn't exist,
 * then we'll assume the game wasn't launched through the launcher, and will just quit the game.
 */
int game_locker_check()
{
	int i = 0;
	#ifdef    LAUNCHER_LINKAGE
		i = (int) ifelse(txt_for_dir(txt_create(1, 0), ".\\save\\*.lock") > 0, 1, 0);
	#else
		i = 1;
	#endif

	return i;
}

/*
 * int game_is_first_time()
 *
 * Tests if the game was run for the first time by reading the value from the registry key defined in
 * the Windows registry. If the key/value pair cannot be found, the function will automatically create one.
 */
int game_is_first_time()
{
	RegistryItem *item = reg_key_new("GiNFT");
	int r = reg_key_exists("Software", item);

	if(!r)
		reg_key_write("Software", item);

	reg_key_free(item);
    return (int) ifelse(!r, 1, 0);
}

/*
 * void game_break_ex(const char *message, const char *archive, const char *file)
 *
 * A very simple attempt at breaking the fourth wall.
 * It triggers the "messenger" ("Messenger.exe"), feeds the provided argument,
 * copies a file out of the archive (.wrs, .pak,... - see file_cpy()),
 * and then terminates the game through a direct call to __game_event_on_close().
 */
void game_break_ex(const char *message, const char *archive, const char *file)
{
	if(!message)
		return;

	game_log_write("Signal received from the other side..");

	#ifdef    DEBUG
		printf("There is an attempt to break the fourth wall but since we're running the game in development mode, let's skip that.");
	#else
		if(archive && file)
			file_cpy(file, archive);

		if(file_exists("Messenger.exe"))
			exec("Messenger.exe", message);

		wait(1.0); // A wait() is necessary for exec() to finish its job, 1.0 is enough.
		__game_event_on_close();
	#endif
}

/*
 * void game_break(const char *message)
 *
 * Breaking the fourth wall without copying anything.
 */
void game_break(const char *message)
{
	game_break_ex(message, NULL, NULL);
}

/*
 * void game_static_init()
 *
 * Performs static initialization, which includes font registering/initialization,
 * event set-ups, etc.
 */
void game_static_init()
{
	// Register the following fonts to the font table, so we can use it without installing into %WINDIR%\Fonts\.
	AddFontResource("./etc/Essai.ttf");
	AddFontResource("./etc/UVN-remind.ttf");
	AddFontResource("./etc/ank.ttf");
	AddFontResource("./etc/AndesRoundedLight.otf");

	// Creates and registers the view points. The coordinates are manually captured.
	vp_computer    = view_point_new(800.0, 395.0, 431.0, 354.0, -16.0, 0.0);
	vp_bedroom     = view_point_new(1237.0, 613.0, 396.0, 260.0, 6.0, 32.0);
	vp_kitchen     = view_point_new(166.0, 192.0, 133.0, 332.0, -3.0, 0.0);
	vp_wake        = view_point_new(1325.0, 487.0, 476.0, 265.0, -2.0, 0.0);

	view_add(vp_bedroom, VP_BEDROOM);
	view_add(vp_computer, VP_COMPUTER);
	view_add(vp_kitchen, VP_KITCHEN);
	view_add(vp_wake, VP_WAKE);

	//
	sndMouseClick   = snd_create(game_asset_get_sound("PC_click.wav"));
	sndPCBootup     = snd_create(game_asset_get_sound("login-01.wav"));
	sndPhoneCall    = snd_create(game_asset_get_sound("phone.wav"));
	sndPCLogout     = snd_create(game_asset_get_sound("logout-01.wav"));
	sndHumanFall    = snd_create(game_asset_get_sound("body-falling.wav"));
	sndMessageSent  = snd_create(game_asset_get_sound("alert-06.wav"));
	sndInvalidClick = snd_create(game_asset_get_sound("alert-05.wav"));
	sndWasher       = snd_create(game_asset_get_sound("washing-machine-1.wav"));

	panBGHandler                 = pan_create(NULL, 995);
	panBGHandler->bmap           = bmap_create(game_asset_get_gui("paper.jpg"));
	panBGHandler->pos_x          = (screen_size.x - bmap_width(panBGHandler->bmap)) * 0.5;
	panBGHandler->pos_y          = 45.0; // Thụt lề, thụt hoài, thụt thụt hoàiiiiii. Kí tên: Lợn khó tính

	txtDataHandler               = txt_create(1, 996);
	txtDataHandler->font         = Note_Text_Font;
	txtDataHandler->pos_x        = panBGHandler->pos_x + 25.0;
	txtDataHandler->pos_y        = panBGHandler->pos_y + 25.0;
	vec_set(&(txtDataHandler->blue), vector(84.0, 84.0, 84.0)); // Replacing vector() with vec_fill(nullvector, 84.0) (to avoid repetitive) and watch the world spin.

	txtSubtitleHandler           = txt_create(1, 1);
	txtSubtitleHandler->font     = Normal_Text_Font;
	txtSubtitleHandler->pos_y    = (screen_size.y / 2.0) + 64.0;
	txtSubtitleHandler->pos_x    = (screen_size.x / 2.0) - 156.0;

	/*
	#ifndef    A8_FREE
		matCrt = ppCrtNewMtl(PP_CRT_MODE_SCANLINE);
	#endif
	*/
}

/*
 * void game_static_free()
 *
 * Frees everything that were previously initialized with game_static_init().
 */
void game_static_free()
{
	view_point_free(vp_bedroom);
	view_point_free(vp_computer);
	view_point_free(vp_kitchen);
	view_point_free(vp_wake);

	gui_credits_free(credits);

	snd_remove(sndMouseClick);
	snd_remove(sndPCBootup);
	snd_remove(sndPhoneCall);
	snd_remove(sndPCLogout);

	#ifndef    A8_FREE
		if(matCrt)
			safe_remove(matCrt);
	#endif
}

/*
 * String *game_region_check()
 *
 * Takes advantage of <utilities>/object_in_region() to check if the player
 * is in a certain region that was drawn beforehand in WED.
 * It'd be more convenient to use a std::map<> and returns the indices, but
 * I'm just being too lazy to write one.
 */
String *game_region_check()
{
	if(object_in_region(player, "KitchenA") || object_in_region(player, "KitchenB"))
		return lstr_rooom_kitchen;

	if(object_in_region(player, "Bathroom"))         return lstr_room_bathroom;
	if(object_in_region(player, "1stBedroom"))       return lstr_room_1stbedroom;
	if(object_in_region(player, "LivingRoom"))       return lstr_room_livingroom;
	if(object_in_region(player, "KitchenHallway"))   return lstr_room_1sthallway;

	if(object_in_region(player, "Balcony"))          return lstr_room_yard;
	if(object_in_region(player, "Workspace"))        return lstr_room_workspace;
	if(object_in_region(player, "WorkspaceHallway")) return lstr_room_2ndhallway;
	if(object_in_region(player, "2ndBedroom"))       return lstr_room_2ndbedroom;
	if(object_in_region(player, "Laundry"))          return lstr_room_balcony;

	return STR_EMPTY;
}

/*
 * int game_day_switch(int d, GamePostData *gpd)
 *
 * Performs days switching. This does more than just simply changing the current_day variable.
 * It does the actual preparation, scene switching and post-initialization.
 */
void scene_load(ChapterData *data, const void *loader); // Pretty much the only way to resolve the cyclic dependencies in this situation.

int game_day_switch(int d, GamePostData *gpd)
{
	if( d >= DAY_1 && d <= DAY_7 ) // If we've passed the correct day.
	{
		game_log_write(str_printf(NULL, "Attempting to switch to day %i.", (int) d));

		scene_load(day[d - 1], game_scene_load);
		WAIT_PROCESS(scene_load);

		if(gpd) {

			#ifdef    DEBUG
				String *_01 = str_printf(NULL,
					"Inspecting the GamePostData struct...\nobjectives = %i, fog_color_id = %i, fog_color->x = %f, fog_color->y = %f, fog_color->z = %f, sun_light = %f",
					(int) gpd->objectives,
					(int) gpd->fog_color_id,
					(double) (gpd->fog_color).x,
					(double) (gpd->fog_color).y,
					(double) (gpd->fog_color).z,
					(double) gpd->sun_light
				);
				game_log_write(_01);
				str_remove(_01);
			#endif

			if( 0 != __GameObjectives_singleton->objectives ) // Make sure to reset the counter.
			{
				__GameObjectives_singleton->objectives       = 0;
			}

			__GameObjectives_singleton->total_objectives = gpd->objectives;
			fog_color                                    = gpd->fog_color_id;
			sun_light                                    = gpd->sun_light;

			vec_set(&d3d_fogcolor1, &(gpd->fog_color));

			if(gpd->use_custom)
			{
				int i = 0, j = 100;
				for(; i <= 16; i++)
				{
					(level_ent->skill)[j - 1] = (gpd->custom)[i];
					j -= 1;
				}
			}
		}

		day_current = d; // Notify that we've switched to a new day.
		return 1;
	}
	else
	{
		game_log_write("In <common>/game_day_switch(): Passed parameter is not in the correct range. Operation aborted.");
		return -1;
	}
}

int game_day_get()
{
	return day_current;
}

int game_scene_switch(int day)
{
	GamePostData     infoBlock;

	StaticTitleText *local_intro_text       = NULL;
	Panel           *local_black_panel      = NULL;

	if( __camera_locked )
		__camera_locked = 0;

	if( __can_press_esc )
		__can_press_esc = 0;

	if( game_intro_done )
		game_intro_done = 0;

	game_mplayer_stop();

	switch(day)
	{
		case    DAY_1:
		{
			infoBlock.objectives = 7;
			infoBlock.use_custom = false;

			game_scene_set_load_text("memory fragment #4");
			game_scene_set_desc_text(lstr_induction_c1);

			break;
		}

		case    DAY_2:
		{
			infoBlock.objectives = 1;
			infoBlock.use_custom = false;

			game_scene_set_load_text("memory fragment #5");
			game_scene_set_desc_text(lstr_induction_c2);

			break;
		}

		case    DAY_3:
		{
			infoBlock.objectives = 0;
			infoBlock.use_custom = false;

			game_scene_set_load_text("memory fragment #1");
			game_scene_set_desc_text(lstr_induction_c3);

			break;
		}

		case    DAY_4:
		{
			infoBlock.objectives = 0;
			infoBlock.use_custom = false;

			game_scene_set_load_text("memory fragment #2");
			game_scene_set_desc_text(lstr_induction_c4);

			break;
		}

		case    DAY_5:
		{
			infoBlock.objectives = 0;
			infoBlock.use_custom = false;

			game_scene_set_load_text("memory fragment #3");
			game_scene_set_desc_text(lstr_induction_c5);

			break;
		}

		case    DAY_6:
		{
			infoBlock.objectives = 0;
			infoBlock.use_custom = false;

			game_scene_set_load_text("memory fragment #6");
			game_scene_set_desc_text(lstr_induction_c6);
		}

		case    DAY_7:
		{
			infoBlock.objectives = 0;
			infoBlock.use_custom = false;

			game_scene_set_load_text("");
			game_scene_set_desc_text("");
		}
	}

	game_day_switch(day, &infoBlock);
	WAIT_PROCESS(game_day_switch);

	sun_light = 100.0;


		// Sets the sky.
		switch(day)
		{
			case    DAY_1:
			case    DAY_2:
			    object_sky_create(game_asset_get_2d_sprite("noon+6.tga"), 42);
				break;

			case    DAY_3:
			case    DAY_4:
			case    DAY_5:
				object_sky_create(game_asset_get_2d_sprite("nothing+6.jpg"), 42);
				break;

			case    DAY_6:
			switch(ending)
			{
				case    GOOD_ENDING:
					object_sky_create(game_asset_get_2d_sprite("noon+6.tga"), 42);
					break;
				case    BAD_ENDING:
					object_sky_create(game_asset_get_2d_sprite("nothing+6.jpg"), 42);
					break;
			}
			break;
		}

	switch(day)
	{
		case    DAY_1:
		{
			local_intro_text  = gui_title_new(
				vector(16.0, 16.0, 0.0),
				COLOR_WHITE,
				lstr_intro_c1,
				10.0,
				LAYER_GUI_5);

			gui_title_set_delay(local_intro_text, 0.03);
			gui_title_set_mode(local_intro_text, FADE_OUT);
			local_intro_text->__container->font = font_create("[ank]*#18b");

			break;
		}

		case    DAY_3:
		{
			local_intro_text  = gui_title_new(
				vector(16.0, 16.0, 0.0),
				COLOR_WHITE,
				lstr_intro_c3,
				10.0,
				LAYER_GUI_5);

			gui_title_set_delay(local_intro_text, 0.03);
			gui_title_set_mode(local_intro_text, FADE_OUT);
			local_intro_text->__container->font = font_create("[ank]*#18b");

			break;
		}
	}

	// Set the fog color and its range from the block to the first (1) fog color slot.
	switch(day)
	{
		case    DAY_1:
		case    DAY_2:
			game_fog_set(1, vector(213, 255, 255), vector(10.0, 10042.0, 0.0)); // Abbreviation for soft yellow (http://www.colourlovers.com/color/FFFFD5/soft-yellow)
			break;

		case    DAY_3:
		case    DAY_4:
		case    DAY_5:
			game_fog_set(1, COLOR_BLACK, vector(10.0, 1400.0, 0.0));
			break;

		case    DAY_6:
			switch(ending)
			{
				case    GOOD_ENDING:
					game_fog_set(1, COLOR_WHITE, vector(10.0, 10042.0, 0.0));
					break;
				case    BAD_ENDING:
					game_fog_set(1, COLOR_BLACK, vector(10.0, 10042.0, 0.0));
					break;
			}
			break;
	}
	game_fog_render(1); // Merely sets fog_color to ID.

	int layern = 0;

	if( local_intro_text )
		layern = local_intro_text->__container->layer - 1;
	else
		layern = LAYER_GUI_1;

	player_lock = 1;
	__camera_locked = 1;

	local_black_panel        = pan_create(NULL, layern);
	local_black_panel->bmap  = bmap_createblack(screen_size.x, screen_size.y, 8);

	SHOW_FLAGS_SAFE(local_black_panel, SHOW);
	SHOW_FLAGS_SAFE(local_black_panel, TRANSLUCENT);
	local_black_panel->alpha = 100.0;

	if( local_intro_text )
	{
		gui_title_show(local_intro_text, true);

		while(!(local_intro_text->done))
			wait(1.0);
	}

	player_lock = 0;
	__camera_locked = 0;
	game_intro_done = 1;

	while(local_black_panel->alpha > 0.0)
	{
		local_black_panel->alpha -= 4.2 * time_step;
		wait(1.0);
	}

	HIDE_FLAGS_SAFE(local_black_panel, SHOW);

	// gui_open_eyes(6.5, 1);

	switch(day)
	{
		case DAY_3: game_mplayer_play("ds-1"); break;
		case DAY_4: game_mplayer_play("ds-2"); break;
		case DAY_5: game_mplayer_play("ds-3"); break;
	}

	__can_press_esc = 1;
}
