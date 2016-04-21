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
__static void __text_init_pos(Text *text) {
    if(text) {
        text->pos_x = (screen_size.x - str_width((text->pstring)[0], text->font)) * 0.5;
        text->pos_y = screen_size.y - 150.0;
    }
}

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

    ent_set_type(my, STATIC_DOOR);

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

    Text *subtitle = txt_create(1, LAYER_GUI_1);
    (subtitle->pstring)[0] = lstr_open_door;
    subtitle->font = Normal_Text_Font;
    __text_init_pos(subtitle);

    VECTOR shot_target;
    long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

    while(my)
    {
        vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
        vec_rotate( &shot_target, &camera->pan);
        vec_add( &shot_target, &camera->x);
        c_trace(&camera->x, &shot_target, flags);

        if(trace_hit && hit->entity) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
        {
            if(ent_get_type(hit->entity) == STATIC_DOOR) // Check if we're shooting at the right entity
            {
                if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                {
                    if(!mouse_left)
                    {
                        if((int) my->skill6)
                        {
                            // draw_text(lstr_close_door, _text_pos.x, _text_pos.y, COLOR_SCARLET);
                            (subtitle->pstring)[0] = lstr_close_door;
                        }
                        else
                        {
                            // draw_text(lstr_open_door, _text_pos.x, _text_pos.y, COLOR_SCARLET);
                            (subtitle->pstring)[0] = lstr_open_door;
                        }
                        SHOW_FLAGS_SAFE(subtitle, SHOW);
                    }
                    else
                    {
                        while(mouse_left)
                            wait(1.0);

                        HIDE_FLAGS_SAFE(subtitle, SHOW);

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
                else
                    HIDE_FLAGS_SAFE(subtitle, SHOW);

            }
            else // End of if(ent_get_type(hit->entity) == STATIC_DOOR)
                HIDE_FLAGS_SAFE(subtitle, SHOW);

        }
        else // End of if(trace_hit && hit->entity)
            HIDE_FLAGS_SAFE(subtitle, SHOW);

        wait(1.0);
    }
    txt_remove(subtitle);
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

action act_glasses()
{
    SHOW_FLAGS_SAFE(my, TRANSLUCENT);

    if(var_cmp(my->pan, 0.0))
        my->pan = .001;
}

/*
 * action act_notepad()
 *
 * A very simple, mini notepad which can be picked up and viewed.
 * string1 must exists (not empty) and contains the name of the according translation
 * file (with the .lstr extension). For example:
 * "read_note.lstr" (without the double quotes).
 * During level load, the contents of the translation file given in string1 is copied to the entity's internal
 * container and will be shown when the player comes close to the notepad and hit the left mouse button.
 */
#define    __HIDE_FLAGS_BULK(noparam) HIDE_FLAGS_SAFE(subtitle, SHOW); HIDE_FLAGS_SAFE(data, SHOW); HIDE_FLAGS_SAFE(bg, SHOW)   // Because it's a multiple statement-macro, it must be
                                                                                                                                // wrapped in brackets.
action act_notepad()
{
    while( !player ) wait(1.0);

    if(my->string1)
    {
        ent_set_type(my, STATIC_NOTEPAD);
        SHOW_FLAGS_SAFE(my, POLYGON);

        VECTOR shot_target;
        long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

        Text *subtitle = txt_create(1, LAYER_GUI_1);
        (subtitle->pstring)[0] = lstr_interact;
        subtitle->font = Normal_Text_Font;
        __text_init_pos(subtitle);

        Panel *bg          = pan_create(NULL, LAYER_GUI_1);
        bg->bmap           = bmap_create(game_asset_get_gui("paper.jpg"));
        bg->pos_x          = (screen_size.x - bmap_width(bg->bmap)) * 0.5;
        bg->pos_y          = 45.0; // Thụt lề, thụt hoài, thụt thụt hoàiiiiii. Kí tên: Lợn khó tính

        Text  *data        = txt_create(1, LAYER_GUI_2);
        data->font         = Note_Text_Font;
        (data->pstring)[0] = region_get_string(my->string1);

        data->pos_x        = bg->pos_x + 25.0;
        data->pos_y        = bg->pos_y + 25.0;
        vec_set(&(data->blue), vector(84.0, 84.0, 84.0)); // Replacing vector() with vec_fill(nullvector, 84.0) (to avoid repetitive) and watch the world spin.

        Sound *paper_sound = snd_create(game_asset_get_sound("page-flip-4.wav"));

        while(my)
        {
            vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
            vec_rotate( &shot_target, &camera->pan);
            vec_add( &shot_target, &camera->x);
            c_trace(&camera->x, &shot_target, flags);

            if(trace_hit && hit->entity) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
            {
                if(ent_get_type(hit->entity) == STATIC_NOTEPAD) // Check if we're shooting at the right entity
                {
                    if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                    {
                        SHOW_FLAGS_SAFE(subtitle, SHOW);

                        if(mouse_left)
                        {
                            while(mouse_left)
                                wait(1.0);

                            snd_play(paper_sound, 100.0, 0.0);
                            SHOW_FLAGS_SAFE(data, SHOW);
                            SHOW_FLAGS_SAFE(bg, SHOW);

                            if((my->flags & FLAG1) && __GameObjectives_singleton)
                                if(__GameObjectives_singleton->objectives < __GameObjectives_singleton->total_objectives)
                                    __GameObjectives_singleton->objectives += 1;
                        }
                    }
                    else
                    {
                        __HIDE_FLAGS_BULK( _(：3 」∠ )_ ); // Dựa trên tác phẩm "Kiều ở lầu Ngưng Bích"
                                                           // Như các bạn thấy đấy, lầu này có 3 tầng.
                                                           // Kiều đang tọa lạc ở tầng 3 của lầu.
                                                           // Kiều nhìn thấy ánh sáng "mặt trời" đang chiếu thẳng
                                                           // vào tym nàng đến nỗi nàng không dám nhìn thẳng vì sợ bị mù.
                                                           // Nên nàng đã quyết định từ nay sẽ giác bộ
                                                           // Lộn
                                                           // Giác ngộ
                                                           // và đi theo con đường kách mạng
                                                           // Nàng nhảy lầu
                                                           // Đừng như nàng.
                    }

                }
                else // End of if(ent_get_type(hit->entity) == STATIC_NOTEPAD)
                { __HIDE_FLAGS_BULK( _(：3 」∠ )_ ); }

            }
            else // End of if(trace_hit && hit->entity)
                { __HIDE_FLAGS_BULK( _(：3 」∠ )_ ); }

            wait(1.0);
        } // End of while(my).

        // Frees associated resources.
        safe_remove(bg);
        safe_remove(paper_sound);
        txt_remove_ex(data);
        txt_remove(subtitle);

    } // End of if(my->string1).
    else
        game_log_write(str_printf(NULL, "[WARNING] Entity %s is defined as a notepad entity but string1 is not defined.", _chr(str_for_entname(NULL, me)) ));
}

/*
 * action act_trigger()
 *
 * Creates an object at the position specified by skill1, skill2 and skill3, with orientation defined by skill4..5..6, and plays a sound
 * when the player comes closer than the distance specified in string1 (see below).
 * string1 takes the following form:
 *  filename; audioname
 *      filename: Name of the model/entity you want to be created (extensions cannot be left out, which means you can use wmb, mdl or hmp or whatever ent_create() supports.)
 *      audioname: Name of the sound file you want to be played (extensions cannot be left out, which means you can use whatever format snd_create() supports, which are .ogg and .wav.)
 * Pass "nil" to one of the arguments in string1 to skip the particular argument.
 */
action act_trigger()
{
    while( !player ) wait(1.0);

    if(my->string1)
    {
        // Parse string1. [0] contains the object filename, while [1] contains the sound.
        Text *string1_parsed = txt_create(0, 0);
        str_parse_delim(string1_parsed, _chr(my->string1), ';');

        // We (sort of) "precache" the result of the str_cmpi() call, because doing it in runtime is generally not a good idea.
        bool precache_object_creatable = (bool) ifelse( str_cmpi((string1_parsed->pstring)[0], "nil"), false, true );
        bool precache_sound_playable   = (bool) ifelse( str_cmpi((string1_parsed->pstring)[1], "nil"), false, true );

        // Set the object's default scaling if the users forgot to set the values.
        if((int) my->skill7 == 0) my->skill7 = 1.0;
        if((int) my->skill8 == 0) my->skill8 = 1.0;
        if((int) my->skill9 == 0) my->skill9 = 1.0;

        // Default position & rotation.
        if((int) my->skill1 == 0) my->skill1 = player->x + random(42.0);
        if((int) my->skill2 == 0) my->skill2 = player->y + random(42.0);
        if((int) my->skill3 == 0) my->skill3 = player->z;

        if((int) my->skill4 == 0) my->skill4 = random(360.0);
        if((int) my->skill5 == 0) my->skill5 = random(360.0);
        if((int) my->skill6 == 0) my->skill6 = random(360.0);

        ent_set_type(my, STATIC_TRIGGER);
        SHOW_FLAGS_SAFE(my, POLYGON);

        VECTOR shot_target;
        long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

        Text *subtitle = txt_create(1, LAYER_GUI_1);
        (subtitle->pstring)[0] = lstr_interact;
        subtitle->font = Normal_Text_Font;
        __text_init_pos(subtitle);

        while(my)
        {
            vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
            vec_rotate( &shot_target, &camera->pan);
            vec_add( &shot_target, &camera->x);
            c_trace(&camera->x, &shot_target, flags);

            if(trace_hit && hit->entity) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
            {
                if(ent_get_type(hit->entity) == STATIC_TRIGGER) // Check if we're shooting at the right entity
                {
                    if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                    {
                        SHOW_FLAGS_SAFE(subtitle, SHOW);

                        if(mouse_left)
                        {
                            while(mouse_left)
                                wait(1.0);

                            VECTOR  pos;
                            VECTOR  rot;
                            VECTOR  scl;

                            pos.x    = my->skill1; pos.y    = my->skill2; pos.z    = my->skill3;
                            rot.x    = my->skill4; rot.y    = my->skill5; rot.z    = my->skill6;
                            scl.x    = my->skill7; scl.y    = my->skill8; scl.z    = my->skill9;

                            #ifdef    DEBUG
                                printf("pos.x = %f, pos.y = %f, pos.z = %f\nrot.x = %f, rot.y = %f, rot.z = %f\nscl.x = %f, scl.y = %f, scl.z = %f",
                                (double) pos.x,
                                (double) pos.y,
                                (double) pos.z,
                                (double) rot.x,
                                (double) rot.y,
                                (double) rot.z,
                                (double) scl.x,
                                (double) scl.y,
                                (double) scl.z);
                            #endif

                            if(precache_object_creatable)
                                Object *object = ent_create(game_asset_get_object((string1_parsed->pstring)[0]), nullvector, NULL);

                            if(precache_sound_playable)
                                snd_play(snd_create((string1_parsed->pstring)[1]), 100.0, 0.0);

                            vec_set(&object->pan    , &rot);
                            vec_set(&object->x      , &pos);
                            vec_set(&object->scale_x, &scl);

                            ptr_remove(hit->entity);

                            if((my->flags & FLAG1) && __GameObjectives_singleton)
                                if(__GameObjectives_singleton->objectives < __GameObjectives_singleton->total_objectives)
                                    __GameObjectives_singleton->objectives += 1;
                        }
                    }
                    else
                        HIDE_FLAGS_SAFE(subtitle, SHOW);

                }
                else // End of if(ent_get_type(hit->entity) == STATIC_TRIGGER)
                    HIDE_FLAGS_SAFE(subtitle, SHOW);

            }
            else // End of if(trace_hit && hit->entity)
                HIDE_FLAGS_SAFE(subtitle, SHOW);

            wait(1.0);
        } // End of while(my).

        // Frees associated resources.
        txt_remove(subtitle);

    } // End of if(my->string1).
    else
        game_log_write(str_printf(NULL, "[WARNING] Entity %s is defined as a trigger entity but string1 is not defined.", _chr(str_for_entname(NULL, me)) ));
}

/*
 * action act_level_changer()
 *
 * A generic, tweakable level changing entity. By default, this entity will closes down the current level and loads the next one if:
 * - You've completed all of the objectives in the current level.
 *
 * string1: Contains the full file name of the translation file which will be popped up when the player comes close to the trigger entity.
 * string2: Contains the full file name of the sound effect you want to be played when the player hits the left mouse button.
 * skill1: Distance between the player and the entity in which the trigger is activated.
 * skill2: ID of one of the initialized ViewPoint-s.
 */
action act_level_changer()
{
    while( !player ) wait(1.0);

    #ifndef    DEBUG
        SHOW_FLAGS_SAFE(my, INVISIBLE);
    #endif

    ent_set_type(my, STATIC_TRIGGER_LEVEL);
    SHOW_FLAGS_SAFE(my, POLYGON);

    VECTOR shot_target;
    long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;
    bool   custom_interact_text = false;

    Text *subtitle = txt_create(1, LAYER_GUI_1);

    Panel *fader = pan_create(NULL, LAYER_GUI_2);
    fader->bmap  = bmap_createblack(screen_size.x, screen_size.y, 8);
    SHOW_FLAGS_SAFE(fader, TRANSLUCENT);
    fader->alpha = 0.0;

    if(my->string1)
    {
        (subtitle->pstring)[0] = region_get_string(my->string1);
        custom_interact_text = true;
    }
    else
        (subtitle->pstring)[0] = lstr_interact;

    subtitle->font = Normal_Text_Font;
    __text_init_pos(subtitle);

    if(my->string2)
        Sound *event_sound = snd_create(game_asset_get_sound(my->string2));

    while(my)
    {
        vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
        vec_rotate( &shot_target, &camera->pan);
        vec_add( &shot_target, &camera->x);
        c_trace(&camera->x, &shot_target, flags);

        if(trace_hit && hit->entity) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
        {
            if(ent_get_type(hit->entity) == STATIC_TRIGGER_LEVEL) // Check if we're shooting at the right entity
            {
                if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                {
                    SHOW_FLAGS_SAFE(subtitle, SHOW);

                    if(mouse_left)
                    {
                        while(mouse_left)
                            wait(1.0);

                        snd_play(event_sound, 100.0, 0.0);
                        act_player_camera_lock_to( (int) my->skill2 );

                        SHOW_FLAGS_SAFE(fader, SHOW);
                        while(fader->alpha < 100.0)
                        {
                            fader->alpha += 3.5 * time_step;
                            wait(1.0);
                        }
                    }
                }
                else
                    HIDE_FLAGS_SAFE(subtitle, SHOW);

            }
            else // End of if(ent_get_type(hit->entity) == STATIC_TRIGGER_LEVEL)
                HIDE_FLAGS_SAFE(subtitle, SHOW);

        }
        else // End of if(trace_hit && hit->entity)
            HIDE_FLAGS_SAFE(subtitle, SHOW);

        wait(1.0);
    } // End of while(my).

    // Frees associated resources.
    if(custom_interact_text)
        txt_remove_ex(subtitle);
    else
        txt_remove(subtitle);

    safe_remove(event_sound);
    pan_remove(fader);
}

action act_special_door()
{
    #ifdef    DEBUG
        SHOW_FLAGS_SAFE(my, PASSABLE | TRANSLUCENT);
        my->alpha = 75.0;
    #else
        SHOW_FLAGS_SAFE(my, POLYGON);
    #endif
}
