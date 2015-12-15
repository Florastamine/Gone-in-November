/* render_rain.h */
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
 * Authors: MasterQ32 (http://www.masterq32.de/)
            Modified and optimized by Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    A8_FREE

#ifndef    _RENDER_RAIN_H_
#define    _RENDER_RAIN_H_

#define PRAGMA_PRINT "[Rain] "

typedef struct {
	int       depth;
	int       state;
	MATERIAL *rain;
	ENTITY  **rain_levels;
	VECTOR   *fall_speed;
} RainState;

RainState *RainState_singleton = NULL;

void render_rain_new();
void render_rain_free();

void render_rain_start();
void render_rain_stop();

int render_rain_get_depth();
int render_rain_get_state();

void render_rain_set_speed(VECTOR *speed);
void render_rain_set_speed(float x, float y, float z);

#include "render_rain.c"
#endif /* render_rain.h */
#endif
