/* behavior_static.c */
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

/*
 * action act_rigid_body()
 *
 * Simple yet generic rigid body.
 */
action act_rigid_body()
{
    my->flags |= (POLYGON);

 	c_setminmax(my);
 	my->push = OBSTACLE_GROUP;

    wait(1);
    switch((int) my->skill1)
    {
        case    BODY_CONVEX:
        {
            pXent_settype(my, PH_RIGID, PH_CONVEX);
            break;
        }

        case    BODY_BOX:
        {
            pXent_settype(my, PH_RIGID, PH_BOX);
            break;
        }

        case    BODY_SPHERE:
        {
            pXent_settype(my, PH_RIGID, PH_SPHERE);
            break;
        }

        default:
        {
            game_log_write(str_printf(my->string1, "(Warning!) Entity %s contains invalid body types for registering as physics entity.", _chr(str_for_entname(NULL, my)) ));
        }
    }

 	pXent_setgroup(my, OBSTACLE_GROUP);
}
