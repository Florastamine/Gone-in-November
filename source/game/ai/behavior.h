/* behavior.h */
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
 * <behaviour>
 *
 * Contains behaviour code aka "actions", which is (mostly) independent of the
 * game logic, with the goal of making this header as portable as possible.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    __BEHAVIOUR_H__
#define    __BEHAVIOUR_H__

#define PRAGMA_PRINT "\nBehavior initialization..."

void __draw_hands();

#include "behavior_types.h"

#include "behavior_player.h"
#include "behavior_AI.h"
#include "behavior_static.h"

#include "behavior.c"
#endif /* behavior.h */
