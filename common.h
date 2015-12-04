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
 */
#ifndef    _COMMON_H_
#define    _COMMON_H_

#define PRAGMA_PRINT "\nGame logic and common tools initialization..."

#define __static 
#define __In
#define __Out

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
	Vector2 load_text_pos;
	Vector2 desc_text_pos;
	Vector2 load_img_pos;
	
	Bitmap *load_img;
	
	String *load_text;
	String *desc_text;
	
	Font   *load_text_font;
	Font   *desc_text_font;
	
	float delay;
	float fade_speed;
	
	int error;
	BOOL fade;
} SceneLoadState;

SceneLoadState *__SceneLoadState_singleton = NULL;

void game_state_new();
void game_state_free();

void game_physx_new();
void game_physx_loop();
void game_physx_free();

void game_log_new();
void game_log_free();
void game_log_write( __In const STRING *content );

void game_event_setup();

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

#include "common.c"
#endif /* common.h */
