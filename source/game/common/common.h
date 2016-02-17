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
    /* #define    PRAGMA_POINTER */
#endif

#ifndef    PRAGMA_ZERO
    #define    PRAGMA_ZERO
#endif

#define __PSVS_VERSION   3030
#define __GAME_VERSION   "v0.1.0-alpha-DEBUG"
#define __GAME_DATA      "base.gpk"

/*
 * Launch parameters.
 */
#define __ARGS_CONSOLE          "--console"    /* Opens the console for command line processing.  No, this is   */
                                               /* not the console that was presented in <default>, it's our own */
                                               /* console with our own commands, which means it will always be  */
                                               /* in our control.                                               */
#define __ARGS_CONSOLE_SHORT    "-c"

#define __ARGS_NO_LOGGING       "--no-log"     /* Suppresses redirecting log messages to __LOG_FILE.            */
#define __ARGS_NO_LOGGING_SHORT "-nl"

#define __ARGS_FORCE_LOW        "--force-low"  /* Suppresses rendering of fragment shaders and vertex shaders,  */
                                               /* regardless of the 3D card capabilities.                       */
#define __ARGS_FORCE_LOW_SHORT  "-fl"

#define __ARGS_DECRYPTOR        "--decryptor"  /* Opens the save game editor instead.                           */
#define __ARGS_DECRYPTOR_SHORT  "-d"

/*
 * Configuration files.
 */
#define __ARGS_LIST "./cfg/args.cfg"           /* Use an external file for parsing arguments instead, because apparently  */
                                               /* command_str is broken.                                                  */

#define __VIDEO_CFG "./cfg/video.cfg"          /* For reading out video parameters.                                       */

#define __VER_FILE  "buildver.txt"             /* For writing the game version, as well as reading to perform updates.    */

#define __LOG_FILE  "stdout.log"               /* For logging events (as long as __ARGS_NO_LOGGING isn't specified).      */

/*
 * Ordered layers.
 */
#define LAYER_ENTITY    1
#define LAYER_HUD_1     2
#define LAYER_HUD_2     3
#define LAYER_HUD_3     4
#define LAYER_HUD_4     5
#define LAYER_GUI_1     6
#define LAYER_GUI_2     7
#define LAYER_GUI_3     8
#define LAYER_GUI_4     9
#define LAYER_GUI_5     10
#define LAYER_GUI_6     11
#define LAYER_GUI_7     12
#define LAYER_GUI_8     13
#define LAYER_GUI_9     14
#define LAYER_MESSAGE   15
#define LAYER_DEBUG_1   16
#define LAYER_DEBUG_2   17

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

/*
 * Prototypes
 */
__namespace(IO) {
    /*
     * void save(  char *file,  void *pre,  void *post )
     *
     * Performs game saving.
     */
    void save( __In char *file, __In void *pre, __In void *post );

    /*
     * void load(  char *file,  void *pre,  void *post )
     *
     * Performs game loading.
     */
    void load( __In char *file, __In void *pre, __In void *post );
}

__namespace(ArgsParse) {
    /*
     * void game_args_parse()
     *
     * Opens the argument list file and parses it.
     */
	void game_args_parse();

    /*
     * void game_video_cfg_parse()
     *
     * Reads and applies video settings from the video configuration file
     * defined in __VIDEO_CFG.
     */
    void game_video_cfg_parse();
}

__namespace(November) {
    /*
     * void game_state_new()
     *
     * Allocates a new game state, and sets up the events for freeing the
     * state on game exit. This function must be called before
     * any other subsystems can be initialized.
     */
	void game_state_new();

    /*
     * void game_state_free()
     *
     * Frees a previously allocated game state.
     */
	void game_state_free();

    /*
     * void game_physx_new()
     *
     * Initializes PhysX.
     */
	void game_physx_new();

    /*
     * void game_physx_loop()
     *
     * Runs the PhysX simulation. For continuous physics simulation, you
     * must place this function in a main loop whenever you have initialized PhysX or not.
     * Code taken from <ackPhysX> with a small modification; the algorithm provided in the
     * manual is wrong and outdated.
     */
	void game_physx_loop();

    /*
     * void game_physx_free()
     *
     * Frees PhysX.
     */
	void game_physx_free();

    /*
     * void game_log_new()
     *
     * Opens the logging file.
     */
	void game_log_new();

    /*
     * void game_log_free()
     *
     * Frees the logging file. Note that, because the file is opened in write mode
     * (not append!), any other calls to game_log_new() after game_log_free()
     * will cause the old log file to be erased.
     */
	void game_log_free();

    /*
     * void game_log_write(const STRING *content)
     *
     * Writes a string to the log file.
     * Information about time and date are automatically appended.
     *
     * For writing with printf(), refer to str_printf().
     */
	void game_log_write( __In const STRING *content );

    /*
     * void game_event_setup()
     *
     * Sets up certain events that are automatically invoked later.
     */
	void game_event_setup();

    /*
     * BOOL game_psvs_test()
     *
     */
    BOOL game_psvs_test();

    /*
     * void game_title_set()
     *
     * Sets the game window's title, in case the .wdf doesn't exist.
     */
    void game_title_set();

    /*
     * void game_fog_set(int ID, VECTOR *color, VECTOR *range)
     *
     * Sets the fog color, and range.
     */
    void game_fog_set(int ID, VECTOR *color, VECTOR *range);
    void game_fog_render(int ID);

    /*
     * void game_globals_set()
     *
     * Overrides some of Gamestudio's default variables, like raising the minimum particles
     * and effects count (default values are way too low).
     * This function must be called at the __VERY__ beginning of main(), even before the ready() check.
     */
    void game_globals_set();

    /*
     * void game_console_load()
     *
     * Initializes and fills the custom console with commands, if __ARGS_CONSOLE was specified in the argument file.
     * The user/player can then press [F11] to switch on the console (type "exit" to switch off the console).
     */
    void game_console_load();

    /*
     * game_resources_load()
     *
     * Loads the game resources' file. During development, resources are not packed into a single file
     * but scattered across different folders and repositories, but in the completed game, resource files
     * are used instead. .wrs format files are automatically loaded during startup. But again, if you want
     * to use custom extensions, you'll have to manually load it with add_resource().
     */
    char *__game_data_buffer = NULL;
    void game_resources_load();

    void game_resources_free();

    __static void __game_version_export();

    /*
     * int game_locker_check()
     *
     * A simple function to test if the game was launched through the launcher.
     * For: 1/ simplicity; 2/ Lite-C doesn't have full support for the facilities and libraries needed for
     * checking if a given process is running; and 3/ I don't like Windows and the whole Windows API in general:
     * I'll just test for a file created by the launcher when it's running (the "locker"). If it doesn't exist,
     * then we'll assume the game wasn't launched through the launcher, and will just quit the game.
     */
    int game_locker_check();
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

__namespace(MPlayer) {
	void game_mplayer_new( __In const STRING *path, __In const STRING *extension );
	void game_mplayer_new( __In const STRING *extension );
	void game_mplayer_new();
	void game_mplayer_free();

	void game_mplayer_next();
	void game_mplayer_prev();
	void game_mplayer_pause();
	void game_mplayer_play();
	void game_mplayer_play( __In int id );
	void game_mplayer_play( __In STRING *substr );
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
}

#include "common.c"
#endif /* common.h */
