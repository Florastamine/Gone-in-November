/* behavior_static.h */
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
 * <behavior_static>
 *
 * Contains code for general, static entities.
 *
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    __BEHAVIOUR_STATIC_H__
#define    __BEHAVIOUR_STATIC_H__

#define    DISTANCE     skill11

/*
 * void act_lray()
 *
 * Generic light-ray.
 */
void act_lray();

/*
 * Body types for use within act_rigid_body().
 */
#define    BODY_CONVEX    1
#define    BODY_BOX       2
#define    BODY_SPHERE    3

/*
 * void act_rigid_body()
 *
 * Simple yet generic rigid body.
 * Entity's skill1 is reserved for body types (see above).
 */
void act_rigid_body();

/*
 * void act_barrier()
 *
 * "Invisible" geometry handlers.
 */
void act_barrier();

/*
 * void act_ssndsrc()
 *
 * Static light source - works great for ambiance sounds.
 *
 * string1 specifies the sound file. (must be *.wav)
 * skill1 controls the volume.
 * skill2 controls the range (radius)
 */
void act_ssndsrc();

/*
 * void act_particle()
 *
 * General-purpose particle emitter.
 * skill1 controls the ID of the particle effect you want to render.
 * Refer to ./source/game/fx/unmanaged.h for a list of particle IDs available. Make sure to #define them before you can use them.
 */
void act_particle();

/*
 * void act_trigger()
 *
 * Creates an object at the position specified by skill1, skill2 and skill3, with orientation defined by skill4..5..6, and plays a sound
 * when the player comes closer than the distance specified in skill11 (see below).
 * string1 contains the file name of the object you want to create afterwards. (extensions cannot be left out, which means you can use wmb, mdl or hmp or whatever ent_create() supports.)
 * string2 contains the file name of the sound you want to be played afterwards. (extensions cannot be left out, which means you can use whatever format snd_create() supports, which are .ogg and .wav.)
 * Pass "nil" to one of the arguments in string1 to skip the particular argument.
 */
void act_trigger();

/*
 * void act_door()
 *
 * A *very* simple implementation of in-game doors. For the purpose of simplicity, I've removed the
 * ability to choose between clockwise or counter-clockwise style of opening/closing doors, as well as
 * manipulating my->pan in place of c_rotate() calls.
 *
 * string1: Name of the sound file which is supposed to be played when the door is opened.
 * string2: Name of the sound file which is supposed to be played when the door is closed.
 * skill1 : Distance between the player and the door in order for the event to be triggered.
 * skill2 : Door opening speed.
 * skill3 : How "wide" you want the door to be.
 * skill4 : Unique ID of the door.
 * skill5: Door opening/closing mode, 0 for clockwise, 1 for counter-clockwise.
 */
#define    CLOCKWISE    0
#define    C_CLOCKWISE  1

#define    SPEED        skill2
#define    ANGLE        skill3
#define    UNIQUE_ID    skill4

void act_door();

/*
 * void act_notepad()
 *
 * A very simple, mini notepad which can be picked up and viewed.
 * string1 must exists (not empty) and contains the name of the according translation
 * file (with the .lstr extension). For example:
 * "read_note.lstr" (without the double quotes).
 * During level load, the contents of the translation file given in string1 is copied to the entity's internal
 * container and will be shown when the player comes close to the notepad and hit the left mouse button.
 */
void act_notepad();

/*
 * void act_level_changer()
 *
 * A generic, tweakable level changing entity. By default, this entity will closes down the current level and loads the next one if:
 * - You've completed all of the objectives in the current level.
 *
 * string1: Contains the full file name of the translation file which will be popped up when the player comes close to the trigger entity.
 * string2: Contains the full file name of the sound effect you want to be played when the player hits the left mouse button.
 * skill1: Distance between the player and the entity in which the trigger is activated.
 * skill2: ID of one of the initialized ViewPoint-s.
 */
void act_level_changer();

#include "behavior_static.c"
#endif /* behavior_static.h */
