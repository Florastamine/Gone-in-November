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

typedef struct {
	BOOL   __game_physx_loaded__;
	
	BOOL   __game_log_loaded__;
	fixed  __game_log_handle__;
} GameState;

GameState *__GameState_singleton = NULL;

void game_state_new();
void game_state_free();

void game_physx_new();
void game_physx_loop();
void game_physx_free();

void game_log_new();
void game_log_free();
void game_log_write(const STRING *content);

void game_event_setup();

#include "common.c"
#endif /* common.h */
