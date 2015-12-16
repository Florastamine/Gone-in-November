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

/*
 * Body types for use within act_rigid_body().
 */
#define    BODY_CONVEX    1
#define    BODY_BOX       2
#define    BODY_SPHERE    3

/*
 * action act_rigid_body()
 *
 * Simple yet generic rigid body.
 * Entity's skill1 is reserved for body types (see above).
 */
action act_rigid_body();

#include "behavior_static.c"
#endif /* behavior_static.h */
