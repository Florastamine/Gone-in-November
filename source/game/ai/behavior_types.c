/* behavior_types.c */
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
 * <behavior_types>+
 *
 * Contains various identifiers which are used for identifying object types.
 *
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
__static void ent_set_type(ENTITY *entity, const int type)
{
    if(entity)
        entity->skill99 = type;
}

int ent_get_type(ENTITY *entity)
{
    return (int) ifelse(entity != NULL, entity->skill99, -1);
}

/*
 * void ent_iterate(const void *processor)
 *
 * Performs iteration on all entities in the current scene.
 * Function signature for the processor function takes one ENTITY * argument.
 */
void ent_iterate(const void *processor)
{
    if(!processor)
        return;

    const void f(ENTITY *entity);
    f = processor;

    for(you = ent_next(NULL); you != NULL; you = ent_next(you))
        f(you); // F*** you!
}
