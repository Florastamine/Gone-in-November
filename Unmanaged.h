/* Unmanaged.h */
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
 *
 */
#ifndef    __GAME_H__
#define    __GAME_H__

#include   <acknex.h>

/*
 * Different IDs which are assigned to various parts of the house scene.
 * At first I was going to use entity pointers with a simple macro which bounds
 * actions to different entity pointers, but the engine doesn't seem to co-operate.
 *
 * A workaround was using unique IDs to assign to the level parts and use ptr_first() to
 * traverse between the linked list, finding which entity in the list matched a given ID.
 */
#define    GEOMETRY_00    1 /* */
#define    GEOMETRY_01    2 /* */
#define    STAIRWAY       3 /* */
#define    YARD           4 /* */
#define    LIVINGR_00     5 /* */
#define    DINING_00      6 /* */
#define    DINING_01      7 /* */
#define    DINING_02      8 /* */
#define    BUDDHA_00      9 /* */
#define    BUDDHA_01     10 /* */
#define    _1BEDR_00     11 /* */
#define    _2BEDR_01     12 /* */
#define    _2BEDR_02     13 /* */
#define    _2BEDR_03     14 /* */

#define    SCENE_PART    skill8

#define    MAX_DAYS    5

#define    DAY_1       0
#define    DAY_2       1
#define    DAY_3       2
#define    DAY_4       3
#define    DAY_5       4

int g_calls = 0;
BOOL g_init  = false;

BOOL g_days[MAX_DAYS];

void init();
void __level_load_event(var);

void g_set_day(int);
BOOL g_get_day(int);

#include "Unmanaged.c"
#endif /* Unmanaged.h */
