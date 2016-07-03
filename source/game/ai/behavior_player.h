/* behavior_player.h */
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
 * <behavior_player>
 *
 * Player code, first person, with credits goes to 3Run.
 * I modified, optimized and simplified his original code for PRAGMA_POINTER
 * compatibility and better coding structures. As a result (and as the nature of
 * GiN), the code won't include ducking and jumping.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    __HOLLA_HOLLA_GET_DOLLAS__
#define    __HOLLA_HOLLA_GET_DOLLAS__

#define __action /* This serves nothing - merely just a mark for distinguish between normal functions and "action" functions. */
#define __static
#define __In
#define __Out
#define __namespace(X) {}

// Collision group
#define PUSH_GROUP                     1
#define PLAYER_GROUP                   2
#define DOOR_GROUP                     3
#define PLATFORM_GROUP                 4
#define OBSTACLE_GROUP                 5
#define LADDER_GROUP                   6
#define LEVEL_GROUP                    7

// Object types (could come in handy later.)
#define OBJECT_TYPE								skill71
#define OBJECT_NONE								0
#define OBJECT_HERO								1
#define OBJECT_DOOR								2
#define OBJECT_PLATFORM							3

#define STATE_IDLE                        0
#define STATE_RUN                         1
#define STATE_IN_AIR                      2
#define STATE_LADDER                      3

#define MOVE_ON_FOOT                      1
#define MOVE_ON_LADDER                    2

#define FOOTSTEP_SOUND_VARIANTS           4

#define SPEED_X                           skill75
#define SPEED_Y                           skill76
#define SPEED_Z                           skill77

/*
* __act_player_state (struct)
*
* This struct contains information related to the first-person player,
* and is intended to not be accessed internally (thus the double underscore at the beginning of the identifier).
* For reading/writing/serializing/deserializing from/into the struct's singleton, use accessors instead.
*/
typedef struct {
	// Stuff used for internal calculations, shouldn't be yielded directly.
	ENTITY *__object;
	ENTITY *__object_stand;

	VECTOR  __bbox_lowest_pos;
	VECTOR  __platform_move_speed;

	float   __quants_covered;              /* "Private" movement speed - which is not the actual movement speed but the distance covered returned by c_move(). */
	float   __distance_to_ground;
	int     __move_type;

	SOUND  *__footstep_sound[FOOTSTEP_SOUND_VARIANTS];

	VECTOR  __last_pos;
	VECTOR  __first_pos;

	// Camera (play with these values!)
	VECTOR  __cam_pos;                     /* For internal use only - if you want to get camera pos & ang, yield &camera->x and &camera->pan instead. */
	ANGLE   __cam_ang;

	float   cam_height;						   /* Camera height, which is added to player->z. */
	float   cam_lerp;							   /* Lerping factor. */
	float   cam_smooth;						   /* Smoothing factor. */
	float   cam_smooth_offset;				   /* Smoothing offset. */

	// Properties
	float   friction;							   /* Movement friction. Higher values causes the player to "slip" on the surface as if it is made from ice. */
	float   movement_friction;
	float   movement_friction_air;
	float   movement_friction_ground;

	float   move_speed;                      /* *Actual* movement speed, climb speed and    */
	float   climb_speed;                     /* walk/run multilpliers. Alter-able in-game.  */
	float   run_multiplier;
	float   walk_multiplier;

	float   health;
	int     state;                           /* State machine */

	// Flags
	BOOL    can_stand;                        /* 1 - if we can stand up, 0 - if there is a celling above the head. */
	BOOL    can_climb;
	BOOL    is_moving;
	BOOL    can_move;
} __act_player_state;

__act_player_state *__act_player_state_singleton = NULL;
__act_player_state *act_player_get_singleton();

__static void __act_player_update_camera();
__static void __act_player_scan_foot();
__static void __act_player_register_physics();
__static void __act_player_create_bbox();

void  act_player_new();
void  act_player_free();

BOOL  act_player_can_stand();
BOOL  act_player_can_climb();
BOOL  act_player_can_move();
BOOL  act_player_is_moving();

float act_player_get_move_speed();
void  act_player_set_move_speed( __In float f );

float act_player_get_climb_speed();
void  act_player_set_climb_speed( __In float f );

float act_player_get_run_multiplier();
void  act_player_set_run_multiplier( __In float f );

float act_player_get_walk_multiplier();
void  act_player_set_walk_multiplier( __In float  f );

float act_player_get_friction();
void  act_player_set_friction( __In float f );

float act_player_get_camera_height();
void  act_player_set_camera_height( __In float f );

int act_player_get_footstep_num();
void act_player_set_footstep( __In int ID, __In const char *file_name );
void act_player_realloc_footstep( __In int ID );

void act_player_camera_lock_toggle();

void act_player_camera_lock_to(int pos);
void act_player_camera_unlock_to();

#include "behavior_player.c"
#endif /* behavior_player.h */
