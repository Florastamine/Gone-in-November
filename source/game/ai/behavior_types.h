/* behavior_types.h */
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
 * <behavior_types>
 *
 * Contains various identifiers which are used for identifying object types.
 *
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    __BEHAVIOUR_TYPES_H__
#define    __BEHAVIOUR_TYPES_H__

#define    __static {}

#define PRAGMA_PRINT "\nBehavior types initialization..."

#define    STATIC_GENERIC                    0
#define    DYNAMIC_GENERIC                   1
#define    DYNAMIC_PLAYER                    2
#define    STATIC_INTERACTABLE_GENERIC       3
#define    DYNAMIC_INTERACTABLE_GENERIC      4
#define    STATIC_AI_GENERIC                 5
#define    STATIC_PARTICLE                   6
#define    STATIC_SOUND_SOURCE               7

#define    STATIC_NOTEPAD                    8
#define    STATIC_TRIGGER                    9

__static    void ent_set_type(ENTITY *entity, const int type);
            int ent_get_type(ENTITY *entity);

/*
 * void ent_iterate(const void *processor)
 *
 * Performs iteration on all entities in the current scene.
 * Function signature for the processor function takes one ENTITY * argument.
 */
void ent_iterate(const void *processor);

void __process_note(ENTITY *entity);

#include "behavior_types.c"
#endif /* behavior_types.h */
