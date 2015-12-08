/* common.h */
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
 * kay
 */
#ifndef    _COMMON_H_
#define    _COMMON_H_

#define PRAGMA_PRINT "\nGame logic and common tools initialization..."

#define __static 
#define __In
#define __Out
#define __namespace(nspc) {}

#ifndef    PRAGMA_POINTER
    #define    PRAGMA_POINTER
#endif

#ifndef    PRAGMA_ZERO
    #define    PRAGMA_ZERO
#endif

#define __LOG_FILE "stdout.log"

/*
 * GameState (struct)
 * This struct contains everything related to the current game state.
 */
typedef struct {
	BOOL   __game_physx_loaded__;
	
	BOOL   __game_log_loaded__;
	fixed  __game_log_handle__;
} GameState;

GameState *__GameState_singleton = NULL;

/*
 * SceneLoadState (struct)
 * This struct contains information about the loading state.
 * When loading a scene with game_scene_load(), the information
 * in this structure is used, so there are functions to alter the singleton
 * (so you can set custom loading bitmap or text pos., more to come).
 */
typedef struct {
	Vector2 load_text_pos;           /* 2D position of the loading text. */
	Vector2 desc_text_pos;           /* 2D position of the additional text (normally to describe the to be played scene) */
	Vector2 load_img_pos;            /* 2D position of the loading image (usually this is (0; 0) and the image is scaled up to fit the screen) */
	
	Bitmap *load_img;                /* Pointer to the loading image. */
	
	String *load_text;               /* Loading text; mutable by game_scene_set_load_text*() or str_(cpy/cat/cut/...)(__SceneLoadState_singleton->load_text, content); */
	String *desc_text;               /* Description text; mutable by game_scene_set_desc_text*() or str_(cpy/cat/cut/...)(__SceneLoadState_singleton->desc_text, content); */
	
	Font   *load_text_font;          /* Font type of the loading text. Neither the font nor the string itself can be rendered on the screen. During */
	                                 /* the execution of game_scene_load(), a temporary Text object is created which combines both the text and the */
	                                 /* font so the string can be rendered onto the screen along with other attributes (fonts, size, width, height) */
	Font   *desc_text_font;          /* The same, but with the description text. */
	
	float delay;                     /* Delay after the scene loading operation completed, in seconds. */
	float fade_speed;                /* Fade speed (if fading was specified). */
	
	int error;                       /* The value of last_error that was passed during the latest call to level_load(). */
	BOOL fade;                       /* Enable/disable screen fading. */
} SceneLoadState;

SceneLoadState *__SceneLoadState_singleton = NULL;

/*
 * MPlayer (struct)
 * This struct contains everything required for the simplified, built-in music player.
 */
#define MPLAYER_DEFAULT_DIRECTORY "./sound/stream/"
#define MPLAYER_DEFAULT_EXTENSION "*.ogg" // By default scans for ogg
#define MPLAYER_BUFFER_SIZE       128
#define MPLAYER_CROSSFADE_SPEED   0.75

typedef struct {
	String  *__search_path;           /* A string which stores the initial search path. */
	Text    *track_list;              /* A text object which stores txt_for_dir()'s result, which, contains the list of tracks scanned. */
	
	int      pos;                     /* Current position of the music player, in the range [1; txt_for_dir()]. */
	int      total_tracks;            /* Contains the total tracks scanned. */
	fixed    handle;                  /* Handle ID to the current track being played. */
	BOOL     randomize;               /* Toggles tracks shuffling. */
	BOOL     fade;                    /* Toggles crossfading. */
	float    volume;	
} MPlayer;

MPlayer *MPlayer_singleton = NULL;

__namespace(November) {
	void game_state_new();
	void game_state_free();

	void game_physx_new();
	void game_physx_loop();
	void game_physx_free();

	void game_log_new();
	void game_log_free();
	void game_log_write( __In const STRING *content );

	void game_event_setup();	
}

__namespace(SceneLoadState) {
	// It'd be better to split this big ass function into a few smaller ones but doing that would 
	// lengthen the functions' name so... bear with it. ;(
	// Edited: Nailed it.
	void game_scene_set_defaults();

	void game_scene_set_load_screen( __In STRING *load_img );
	BMAP *game_scene_get_load_screen();

	void game_scene_set_load_text( __In STRING *load_text );
	STRING *game_scene_get_load_text();

	void game_scene_set_desc_text( __In STRING *desc_text );
	STRING *game_scene_get_desc_text();

	void game_scene_set_load_text_font( __In STRING *font_str );
	void game_scene_set_load_text_font( __In FONT *font_struct );
	FONT *game_scene_get_load_text_font();

	void game_scene_set_desc_text_font( __In STRING *font_str );
	void game_scene_set_desc_text_font( __In FONT *font_struct );
	FONT *game_scene_get_desc_text_font();

	void game_scene_set_load_text_pos(__In float x, __In float y);
	void game_scene_set_desc_text_pos(__In float x, __In float y);
	void game_scene_set_load_img_pos(__In float x, __In float y);

	void game_scene_set_delay( __In float d );
	float game_scene_get_delay();

	void game_scene_set_fade( __In BOOL b );
	void game_scene_set_fade_speed( __In float s );

	float game_scene_get_fade_speed();
	BOOL  game_scene_is_fade();

	void game_scene_load( __In STRING *scene );
}

__namespace(Player) {
	void game_mplayer_new( __In const STRING *path, __In const STRING *extension );
	void game_mplayer_new( __In const STRING *extension );
	void game_mplayer_new();
	void game_mplayer_free();
	
	void game_mplayer_next();
	void game_mplayer_prev();
	void game_mplayer_pause();
	void game_mplayer_play();
	void game_mplayer_stop();
	void game_mplayer_tune();
	
	// Accessors
	fixed game_mplayer_get_handle();
	int   game_mplayer_get_total_tracks();
	
	void  game_mplayer_set_pos( int pos );
	int   game_mplayer_get_pos();
	
	BOOL  game_mplayer_get_randomize();
	void  game_mplayer_set_randomize( __In BOOL b );
	
	void  game_mplayer_set_volume( __In float vol );
	float game_mplayer_get_volume();
	
	BOOL  game_mplayer_get_cfade();
	void  game_mplayer_set_cfade( __In BOOL b );
}

#include "common.c"
#endif /* common.h */
