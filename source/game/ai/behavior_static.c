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
 	pXent_setfriction(my, 0);
}

/*
 * action act_barrier()
 *
 * "Invisible" geometry handlers.
 */
action act_barrier()
{
    #ifdef    DEBUG
        my->flags |= (TRANSLUCENT);
        my->alpha  = 75.0;
    #else
        my->flags |= (INVISIBLE);
    #endif
}

/*
 * action act_ssndsrc()
 *
 * Static light source - works great for ambiance sounds.
 *
 * string1 specifies the sound file. (must be *.wav)
 * skill1 controls the volume.
 * skill2 controls the range (radius)
 */
action act_ssndsrc()
{
    my->flags |= (INVISIBLE | PASSABLE);

    if(my->skill1 <= 0.0)
        my->skill1 = 100.0;

    if(my->skill2 <= 0.0)
        my->skill2 = 5000.0;

    #ifdef    DEBUG
        ent_create( game_asset_get_2d_sprite("speaker1.png"), &my->x, NULL );
    #endif

    STRING *file = game_asset_get_sound(my->string1);

    if( file_exists(file) )
        ent_playloop2(my, snd_create(file), my->skill1, my->skill2 );
    else
        game_log_write( str_printf(NULL, "Error executing entity %s - a static sound entity was specified but the stream <%s> couldn't be found.", _chr(str_for_entname(NULL, my)) , _chr(file) ) );
}
