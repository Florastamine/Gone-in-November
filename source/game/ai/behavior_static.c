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
    ent_set_type(my, DYNAMIC_GENERIC);

 	c_setminmax(my);
 	my->push = OBSTACLE_GROUP;
    my->flags |= (POLYGON);

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
    ent_set_type(my, STATIC_SOUND_SOURCE);

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

/*
 * action act_particle()
 *
 * General-purpose particle emitter.
 * skill1 controls the ID of the particle effect you want to render.
 * Refer to ./source/game/fx/unmanaged.h for a list of available particle IDs. Make sure to #define them before you can use them.
 */
action act_particle()
{
    ent_set_type(my, STATIC_PARTICLE);

    my->flags |= (PASSABLE);

    #ifdef    DEBUG
        my->flags |= (TRANSLUCENT);
        my->alpha  = 75.0;
    #else
        my->flags |= (INVISIBLE); // who wants an ugly cube anyway.
    #endif

    particle_process(my, &my->x, my->skill1);
}

/*
 * action act_obj_beam()
 *
 * Creates an object at the position specified by skill1, skill2 and skill3, with orientation defined by skill4..5..6, and plays a sound
 * when the player comes closer than the distance specified in string1 (see below).
 * string1 takes the following form:
 *  filename; audioname; distance
 *      filename: Name of the model/entity you want to be created (extensions cannot be left out, which means you can use wmb, mdl or hmp or whatever ent_create() supports.)
 *      audioname: Name of the sound file you want to be played (extensions cannot be left out, which means you can use whatever format snd_create() supports, which are .ogg and .wav.)
 *      distance: When the distance between the player and the object is lower than this value, the player will be asked to press [F] in order for the event to be triggered.
 * Pass "nil" to one of the arguments in string1 to skip the particular argument.
 */

/*
 * http://en.cppreference.com/w/c/numeric/math/fabs
 * Apparently because using abs() for float is stupid.
 */
float __cdecl fabsf (float x);

action act_obj_beam()
{
    while( !player ) wait(1.0);
    ent_set_type(my, STATIC_INTERACTABLE_GENERIC);

    #ifdef    DEBUG
        my->flags |= (TRANSLUCENT);
        my->alpha  = 75.0;
    #else
        my->flags |= (INVISIBLE);
    #endif

    my->flags |= (PASSABLE);

    if(my->string1)
    {
        // Prepares the necessary ingredients, i. e. parses string1 and puts the result to the container - just do the right things.
        Text *t = txt_create(0, 1);
        float dist = 0.0;

        str_parse_delim(t, _chr(my->string1), ';');

        // Fine-tunes the result.
        if(!fabsf)
            fabsf = DefineApi("msvcrt!fabsf");

        if(dist <= 0.0)
            dist = 256.0;
        else
            dist = fabsf(str_to_float((t->pstring)[2]));

        while(my)
        {
            if(vec_dist(player->x, my->x) <= dist)
            {
                if( !key_f )
                    draw_text("Press [F] to feel the trigger!", 10.0, 10.0, COLOR_WHITE);
                else
                {
                    while(key_f)
                        wait(1.0);

                    if( !(str_cmpi((t->pstring)[0], STR_NIL)) )
                        ent_create(game_asset_get_object((t->pstring)[0]), vector(my->skill1, my->skill2, my->skill3), NULL);
                    if( !(str_cmpi((t->pstring)[1], STR_NIL)) )
                    {
                        SOUND *s = snd_create(game_asset_get_sound((t->pstring)[1]));
                        snd_play(s, 100.0, 0.0);
                    }

                    txt_remove_ex(t);
                    ent_remove(my);
                }
            }

            wait(1.0);
        }
    }
    else
        game_log_write(str_printf(NULL, "[WARNING] Entity %s is defined as a trigger entity but string1 is not defined.", _chr(str_for_entname(NULL, me)) ));
}

/*
 * action act_door()
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

action act_door()
{
    while( !player ) wait(1.0);
    ent_set_type(my, STATIC_INTERACTABLE_GENERIC);

    // This is the "relative" variable used for rotating the door "relatively" to the current angle.
    float angle = 0.0;

    // Correct the distance.
    my->DISTANCE = (var) ifelse(my->DISTANCE <= 0.0, 128.0, my->DISTANCE);

    // Create sound sources if possible - string1 contains the door opening sound, string2 contains the door closing sound.
    SOUND *src_open   = NULL;
    SOUND *src_close  = NULL;

    if(my->string1)
        src_open = snd_create(game_asset_get_sound(my->string1));
    if(my->string2)
        src_close = snd_create(game_asset_get_sound(my->string2));

    VECTOR _text_pos;
    vec_set(&_text_pos, gui_screen_get_center());

    while(my)
    {
        if(vec_dist(player->x, my->x) <= my->DISTANCE)
        {
            if(!key_f)
            {
                if((int) my->skill6)
                    draw_text(lstr_close_door, _text_pos.x, _text_pos.y, COLOR_SCARLET);
                else
                    draw_text(lstr_open_door, _text_pos.x, _text_pos.y, COLOR_SCARLET);
            }
            else
            {
                while(key_f)
                    wait(1.0);

                angle = my->pan;

                if((int) !my->skill6)   // skill6 stores the state of the door (0 = closed, 1 = opened).
                                        // We're testing if the door was closed before.
                {
                    if(src_open)
                        snd_play(src_open, 100.0, 0.0);

                    if(my->skill5 == C_CLOCKWISE) // If we're opening the door in counter-clockwise mode.
                    {
                        while(my->pan <= angle + my->ANGLE)
                        {
                            my->pan += my->SPEED * time_step;
                            wait(1.0);
                        }

                        my->pan = angle + my->ANGLE; // Small fine-tuning.
                    }
                    else
                    {
                        while(my->pan >= angle - my->ANGLE)
                        {
                            my->pan -= my->SPEED * time_step;
                            wait(1.0);
                        }

                        my->pan = angle - my->ANGLE;
                    }

                    my->skill6 = (int) 1;
                }
                else // Close the door, because my->skill6 is already 1.
                {
                    if(src_close)
                        snd_play(src_close, 100.0, 0.0);

                    if(my->skill5 == C_CLOCKWISE) // If we're opening the door in counter-clockwise mode.
                    {
                        while(my->pan >= angle - my->ANGLE)
                        {
                            my->pan -= my->SPEED * time_step;
                            wait(1.0);
                        }

                        my->pan = angle - my->ANGLE;
                    }
                    else
                    {
                        while(my->pan <= angle + my->ANGLE)
                        {
                            my->pan += my->SPEED * time_step;
                            wait(1.0);
                        }

                        my->pan = angle + my->ANGLE;
                    }

                    my->skill6 = (int) 0; // Mark the door as "closed".
                }
            }
        }
        wait(1.0);
    }
}

/*
 * action act_lray()
 *
 * Generic light-ray.
 */
action act_lray()
{
    render_light_rays_on(me);
}
