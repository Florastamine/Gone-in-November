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
 * 
 * TODO: Append instead of a complete erase.
 */
void game_log_free()
{
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
		__write_newline(__GameState_singleton->__game_log_handle__);
		
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
		while(p->alpha < t) {
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
		while(p->alpha > t) {
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
	
	PANEL *p  = pan_create(NULL, 1);
	TEXT  *t0 = txt_create(1, 2);
	TEXT  *t1 = txt_create(1, 2);
	
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
		while(proc_status(__p_fade_in)) wait(1.0); // Block the local static function (because of wait(1) could allow
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
	
	game_log_write( str_printf(NULL, "Scene <%s> loaded. Operation took %f seconds.", _chr(scene), load_time * POW_10_6) );
}

__static void __cfade_out()
{
	float old = MPlayer_singleton->volume;
	while( MPlayer_singleton->volume > 0.0 )
	{
		media_tune( game_mplayer_get_handle(), MPlayer_singleton->volume, 0, 0 );
		
		MPlayer_singleton->volume -= MPLAYER_CROSSFADE_SPEED * time_step;
		wait(1.0);
	}
	
	MPlayer_singleton->volume = old;
}

__static void __cfade_in()
{
	float old = MPlayer_singleton->volume;
	MPlayer_singleton->volume = 0.0;
	while( MPlayer_singleton->volume < old )
	{
		media_tune( game_mplayer_get_handle(), MPlayer_singleton->volume, 0, 0 );
		
		MPlayer_singleton->volume += MPLAYER_CROSSFADE_SPEED * time_step;
		wait(1.0);
	}
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
	
	MPlayer_singleton->track_list   = txt_create(MPLAYER_BUFFER_SIZE, 1);
	MPlayer_singleton->total_tracks = txt_for_dir( MPlayer_singleton->track_list, str_cat(path, extension) );
	
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

BOOL  game_mplayer_get_cfade()
{
	if(MPlayer_singleton)
	    return MPlayer_singleton->fade;
}

void  game_mplayer_set_cfade( BOOL b )
{
	if(MPlayer_singleton)
	    MPlayer_singleton->fade = b;
}

void game_mplayer_next()
{
	if(MPlayer_singleton)
	{
		if( game_mplayer_get_pos() < game_mplayer_get_total_tracks() )
		    MPlayer_singleton->pos =  game_mplayer_get_pos() + 1;
		
		game_mplayer_play();
	}
}

void game_mplayer_prev()
{
	if(MPlayer_singleton)
	{
		if(game_mplayer_get_pos() > 1)
		    MPlayer_singleton->pos =  game_mplayer_get_pos() - 1;
		
		game_mplayer_play();
	}
}

void game_mplayer_pause()
{
	if( media_playing(game_mplayer_get_handle()) )
	{
		if( game_mplayer_get_cfade() )
		{
			__cfade_out();
			while(proc_status(__cfade_out)) wait(1.0);
		}
		media_pause(game_mplayer_get_handle());
	}
}

void game_mplayer_play()
{
	game_mplayer_stop();
	while(proc_status(game_mplayer_stop)) wait(1.0); // In case of crossfading
	
	MPlayer_singleton->handle = media_play((MPlayer_singleton->track_list->pstring)[MPlayer_singleton->pos - 1], NULL, game_mplayer_get_volume());
	if( game_mplayer_get_cfade() ) // Crossfading enabled?
	    __cfade_in();
}

void game_mplayer_stop()
{
	if( media_playing(game_mplayer_get_handle()) )
	{
		if( game_mplayer_get_cfade() )
		{
			__cfade_out();
			while(proc_status(__cfade_out)) wait(1.0);
		}
		media_stop(game_mplayer_get_handle());
	}
}
