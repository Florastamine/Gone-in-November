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

Object *__tracer = NULL;

/*
 * void act_rigid_body()
 *
 * Simple yet generic rigid body.
 */
void act_rigid_body()
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
 * void act_barrier()
 *
 * "Invisible" geometry handlers.
 */
void act_barrier()
{
    #ifdef    DEBUG
        my->flags |= (TRANSLUCENT);
        my->alpha  = 75.0;
    #else
        my->flags |= (INVISIBLE);
    #endif
}

/*
 * void act_ssndsrc()
 *
 * Static light source - works great for ambiance sounds.
 *
 * string1 specifies the sound file. (must be *.wav or *.ogg)
 * skill1 controls the volume.
 * skill2 controls the range (radius)
 */
void act_ssndsrc()
{
    ent_set_type(my, STATIC_SOUND_SOURCE);

    my->flags |= (INVISIBLE | PASSABLE);

    if(my->skill2 <= 0.0)
        my->skill2 = 5000.0;

    #ifdef    DEBUG
        ent_create( game_asset_get_2d_sprite("speaker1.png"), &my->x, NULL );
    #endif

    STRING *file = game_asset_get_sound(my->string1);
    SOUND  *src  = snd_create(file);

    bool b = file_exists(file); // precache

    if( b )
    {
        var handle = ent_playloop2(my, src, game_volume, my->skill2 );

        while( snd_playing(handle) ) // Continuously update the volume of the sound object.
        {
            snd_tune(handle, game_volume, 0, 0);
            wait(1.0);
        }
    }
    else
        game_log_write( str_printf(NULL, "Error executing entity %s - a static sound entity was specified but the stream <%s> couldn't be found.", _chr(str_for_entname(NULL, my)) , _chr(file) ) );
}

Object *phone = NULL;

void act_phone()
{
    while( !player )
        wait(1.0);

    phone = me;
    ent_set_type(my, STATIC_PHONE);

    VECTOR shot_target;
    long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

    my->skill71 = my->ambient;

    const String *s = region_get_string("c2-2.pad.lstr");
    const Font   *f = font_create("iCiel Andes Rounded Light#25");

    const String *s_0 = region_get_string("c2-3.pad.lstr");

    float phone_x = ((screen_size.x - str_width(s, f)) / 2.0) - 64.0;
    float phone_y = (screen_size.y / 2.0) - 256.0;

    float aphone_x = (screen_size.x - str_width(s_0, f)) / 2.0;
    float aphone_y = (screen_size.y / 2.0) - 256.0;

    my->flags &= FLAG3;

    while(my)
    {
        if(my->flags & FLAG3)
        {
            vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
            vec_rotate( &shot_target, &camera->pan);
            vec_add( &shot_target, &camera->x);
            c_trace(&camera->x, &shot_target, flags);

            if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
            {
                if(ent_get_type(hit->entity) == STATIC_PHONE) // Check if we're shooting at the right entity
                {
                    if(vec_dist(&player->x, &hit->entity->x) < my->DISTANCE) // Check if we're close enough to the entity.
                    {
                        __draw_hands();

                        if( !var_cmp(my->ambient, 100.0) )
                            my->ambient = 100.0;

                        if(mouse_left)
                        {
                            while(mouse_left)
                                wait(1.0);

                            // <...>
                            my->flags |= (FLAG2);
                            my->skill88 = 0;
                            my->ambient = my->skill71;

                            if( snd_playing(sndPhoneCallHandle) )
                                snd_stop(sndPhoneCallHandle);

                            if((my->flags & FLAG1) && __GameObjectives_singleton)
                                if(__GameObjectives_singleton->objectives < __GameObjectives_singleton->total_objectives)
                                    if( 0 == (int) my->skill69 )
                                    {
                                        __GameObjectives_singleton->objectives += 1;
                                        my->skill69 = 1;
                                    }

                            player_lock = 1;
                            snd_play( snd_create(game_asset_get_sound("phone-press.wav")) , game_volume, 0.0);

                            StaticTitleText *d1 = gui_title_new(
                                 vector(phone_x, phone_y, 0.0),
                                 COLOR_WHITE,
                                 region_get_string("c2-2.lstr"),
                                 1.0,
                                 LAYER_DEBUG_1
                             );

                            d1->__container->font = f;
                            gui_title_set_delay(d1, 0.07);
                            gui_title_set_mode(d1, FADE_OUT);
                            gui_title_show(d1, true);

                            while(!(d1->done))
                                wait(1.0);

                            player_lock = 0;

                            StaticTitleText *d2 = gui_title_new(
                                vector(aphone_x, aphone_y, 0.0),
                                COLOR_WHITE,
                                region_get_string("c2-3.lstr"),
                                3.0,
                                LAYER_DEBUG_1
                            );

                            d2->__container->font = f;
                            gui_title_set_delay(d2, 0.025);
                            gui_title_set_mode(d2, FADE_OUT);
                            gui_title_show(d2, true);
                        }
                    }
                    else
                    {
                        my->ambient = my->skill71;
                    }

                } // End of if(ent_get_type(hit->entity) == STATIC_PHONE)
                {
                    my->ambient = my->skill71;
                }
            } // End of if(trace_hit && hit->entity).
            {
                my->ambient = my->skill71;
            }

        }

        wait(1.0);
    }
}

void act_ssndsrc_obj()
{
    while( !player )
        wait(1.0);

    ent_set_type(my, STATIC_SOUND_SOURCE);

    if(my->skill2 <= 0.0)
        my->skill2 = 5000.0;

    #ifdef    DEBUG
        ent_create( game_asset_get_2d_sprite("speaker1.png"), &my->x, NULL );
    #endif

    STRING *file = game_asset_get_sound(my->string1);
    SOUND  *src  = snd_create(file);

    VECTOR shot_target;
    long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

    my->skill88 = 1;
    my->skill71 = my->ambient;

    bool b = file_exists(file);

    while( !game_intro_done )
        wait(1.0);

    if( b )
    {
        var handle = ent_playloop2(my, src, game_volume, my->skill2 );

        while( snd_playing(handle) ) // Continuously update the sound object.
        {
            snd_tune(handle, game_volume, 0, 0);

            vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
            vec_rotate( &shot_target, &camera->pan);
            vec_add( &shot_target, &camera->x);
            c_trace(&camera->x, &shot_target, flags);

            if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
            {
                if(ent_get_type(hit->entity) == STATIC_SOUND_SOURCE) // Check if we're shooting at the right entity
                {
                    if(vec_dist(&player->x, &hit->entity->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                    {
                        __draw_hands();

                        if( !var_cmp(my->ambient, 100.0) )
                            my->ambient = 100.0;

                        if(mouse_left)
                        {
                            while(mouse_left)
                                wait(1.0);

                            snd_stop(handle);

                            my->skill88 = 0;
                            my->ambient = my->skill71;
                            my->flags |= FLAG2;

                            phone->flags |= FLAG3;
                            sndPhoneCallHandle = ent_playloop2(phone, sndPhoneCall, game_volume * 0.75, 1500.0);

                            if((my->flags & FLAG1) && __GameObjectives_singleton) // If we've registered this entity for objectives completion before.
                                if(__GameObjectives_singleton->objectives < __GameObjectives_singleton->total_objectives)
                                    if( 0 == (int) my->skill69 )
                                    {
                                        __GameObjectives_singleton->objectives += 1;
                                        my->skill69 = 1;
                                    }
                        }
                    }
                    else
                    {
                        my->ambient = my->skill71;
                    }

                } // End of if(ent_get_type(hit->entity) == STATIC_SOUND_SOURCE)
                else
                {
                    my->ambient = my->skill71;
                }
            } // End of if(trace_hit && hit->entity).
            else
            {
                my->ambient = my->skill71;
            }

            wait(1.0);
        }
    }
    else
        game_log_write( str_printf(NULL, "Error executing entity %s - a static sound entity was specified but the stream <%s> couldn't be found.", _chr(str_for_entname(NULL, my)) , _chr(file) ) );
}

/*
 * void act_particle()
 *
 * General-purpose particle emitter.
 * skill1 controls the ID of the particle effect you want to render.
 * Refer to ./source/game/fx/unmanaged.h for a list of available particle IDs. Make sure to #define them before you can use them.
 */
void act_particle()
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

    // http://kinhdoanh.vnexpress.net/tin-tuc/khoi-nghiep/9x-kiem-tram-trieu-moi-thang-nho-ban-banh-mi-va-dich-vu-shipper-3392000.html
    // Bleh, mình nên kiếm nghề khác
}

/*
 * void act_door()
 *
 * A *very* simple implementation of in-game doors. For the purpose of simplicity, I've removed the
 * ability to choose between clockwise or counter-clockwise style of opening/closing doors, as well as
 * manipulating my->pan in place of c_rotate() calls.
 *
 * string1: Name of the sound file which is supposed to be played when the door is opened.
 * string2: Name of the sound file which is supposed to be played when the door is closed.
 * skill11/DISTANCE : Distance between the player and the door in order for the event to be triggered.
 * skill2 : Door opening speed.
 * skill3 : How "wide" you want the door to be.
 * skill4 : Unique ID of the door.
 * skill5: Door opening/closing mode, 0 for clockwise, 1 for counter-clockwise.
 */
void act_door()
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

    my->skill71 = my->ambient;

    VECTOR shot_target;
    long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

    while(my)
    {
        vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
        vec_rotate( &shot_target, &camera->pan);
        vec_add( &shot_target, &camera->x);
        c_trace(&camera->x, &shot_target, flags);

        if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
        {
            if(ent_get_type(hit->entity) == STATIC_DOOR) // Check if we're shooting at the right entity
            {
                if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                {
                    if(!mouse_left)
                    {
                        if( !var_cmp(my->ambient, 100.0) )
                            my->ambient = 100.0;

                        __draw_hands();
                    }
                    else
                    {
                        while(mouse_left)
                            wait(1.0);

                        angle = my->pan;
                        my->ambient = my->skill71;

                        if((int) !my->skill6)   // skill6 stores the state of the door (0 = closed, 1 = opened).
                                                // We're testing if the door was closed before.
                        {
                            if(src_open)
                                snd_play(src_open, game_volume, 0.0);

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
                                snd_play(src_close, game_volume, 0.0);

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
                {
                    my->ambient = my->skill71;
                }

            } // End of if(ent_get_type(hit->entity) == STATIC_DOOR)
            else
            {
                my->ambient = my->skill71;
            }

        } // End of if(trace_hit && hit->entity)
        else
        {
            my->ambient = my->skill71;
        }

        wait(1.0);
    }
}

/*
 * void act_lray()
 *
 * Generic light-ray.
 */
void act_lray()
{
    render_light_rays_on(me);
}

void act_glasses()
{
    SHOW_FLAGS_SAFE(my, TRANSLUCENT);

    if(var_cmp(my->pan, 0.0))
        my->pan = .001;
}

/*
 * void act_notepad()
 *
 * A very simple, mini notepad which can be picked up and viewed.
 * string1 must exists (not empty) and contains the name of the according translation
 * file (with the .lstr extension). For example:
 * "read_note.lstr" (without the double quotes).
 * During level load, the contents of the translation file given in string1 is copied to the entity's internal
 * container and will be shown when the player comes close to the notepad and hit the left mouse button.
 *
 * string2 (optional!) contains the complete name & size of the font you're going to use. For example: Arial#15b
 */
void act_notepad()
{
    while( !player ) wait(1.0);

    if(my->string1)
    {
        ent_set_type(my, STATIC_NOTEPAD);
        SHOW_FLAGS_SAFE(my, POLYGON);

        VECTOR shot_target;
        long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

        Sound *paper_sound = snd_create(game_asset_get_sound("page-flip-4.wav"));

        my->string1 = region_get_string(my->string1); // Re-use string1 to contain the translation data instead of a brand new string object, which just plainly sucks.
        my->skill71 = my->ambient;

        while(my)
        {
            vec_set(&shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
            vec_rotate(&shot_target, &camera->pan);
            vec_add(&shot_target, &camera->x);
            c_trace(&camera->x, &shot_target, flags);

            if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
            {
                if(ent_get_type(hit->entity) == STATIC_NOTEPAD) // Check if we're shooting at the right entity
                {
                    if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                    {
                        __draw_hands();

                        if( !var_cmp(my->ambient, 100.0) )
                            my->ambient = 100.0;

                        if(mouse_left)
                        {
                            snd_play(paper_sound, game_volume, 0.0);

                            if((my->flags & FLAG1) && __GameObjectives_singleton)
                                if(__GameObjectives_singleton->objectives < __GameObjectives_singleton->total_objectives)
                                    if( 0 == (int) my->skill69 )
                                    {
                                        __GameObjectives_singleton->objectives += 1;
                                        my->skill69 = 1;
                                    }

                            while(mouse_left)
                            {
                                if((txtDataHandler->pstring)[0] != my->string1)
                                    (txtDataHandler->pstring)[0] = my->string1;

                                draw_obj(panBGHandler);
                                draw_obj(txtDataHandler);

                                wait(1.0);
                            }

                        }
                    }
                    else
                    {
                        my->ambient = my->skill71;

                        // __HIDE_FLAGS_BULK( _(：3 」∠ )_ ); // Dựa trên tác phẩm "Kiều ở lầu Ngưng Bích"
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
                {
                    my->ambient = my->skill71;
                }
            } // End of if(trace_hit && hit->entity)
            else
            {
                my->ambient = my->skill71;
            }

            wait(1.0);
        } // End of while(my).

        // Frees associated resources.
        safe_remove(paper_sound);

    } // End of if(my->string1).
    else
        game_log_write(str_printf(NULL, "[WARNING] Entity %s is defined as a notepad entity but string1 is not defined.", _chr(str_for_entname(NULL, me)) ));
}

/*
 * void act_trigger()
 *
 * Creates an object at the position specified by skill1, skill2 and skill3, with orientation defined by skill4..5..6, and plays a sound
 * when the player comes closer than the distance specified in skill11 (see below).
 * string1 contains the file name of the object you want to create afterwards. (extensions cannot be left out, which means you can use wmb, mdl or hmp or whatever ent_create() supports.)
 * string2 contains the file name of the sound you want to be played afterwards. (extensions cannot be left out, which means you can use whatever format snd_create() supports, which are .ogg and .wav.)
 * Pass "nil" to one of the arguments in string1 to skip the particular argument.
 */
 SOUND *__trigger_event_sound         = NULL;
 fixed  __trigger_event_sound_hndl    = 0;

void act_trigger()
{
    while( !player && !my ) wait(1.0);

    // Parse string1. [0] contains the object filename, while [1] contains the sound.
    // We (sort of) "precache" the result of the str_cmpi() call, because doing it during runtime is generally not a good idea.
    // R.I.P. precaching 2016 - 2016.

    // Because LC can't do something like this: ifelse(struct && struct->elem), so... cumbersome, I know.
    if(my->string1)
        my->skill78 = (int) ifelse(str_cmpi(my->string1, "nil") == 1, 0, 1); // Object
    else
        my->skill78 = (int) 0;

    if(my->string2)
        my->skill79 = (int) ifelse(str_cmpi(my->string2, "nil") == 1, 0, 1); // Sound
    else
        my->skill79 = (int) 0;

    // Set the object's default scaling if the users forgot to set the values.
    if((int) my->skill7 == 0) my->skill7 = 1.0;
    if((int) my->skill8 == 0) my->skill8 = 1.0;
    if((int) my->skill9 == 0) my->skill9 = 1.0;

    // Default position & rotation.
    if((int) my->skill1 == 0) my->skill1 = my->x;
    if((int) my->skill2 == 0) my->skill2 = my->y;
    if((int) my->skill3 == 0) my->skill3 = my->z;

    if((int) my->skill4 == 0) my->skill4 = my->pan;
    if((int) my->skill5 == 0) my->skill5 = my->tilt;
    if((int) my->skill6 == 0) my->skill6 = my->roll;

    ent_set_type(my, STATIC_TRIGGER);

    VECTOR shot_target;
    VECTOR trace;
    long   flags = IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

    Object *object = NULL;

    my->skill88 = 1;
    my->skill71 = my->ambient;

    // object_blink( my, 12.0, &my->skill88 ); // Blinks the object until its FLAG2 is set (which is written after the player has pressed the button).

    while(my)
    {
        vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
        vec_rotate( &shot_target, &camera->pan);
        vec_add( &shot_target, &camera->x);
        c_trace(&camera->x, &shot_target, flags);

        vec_set(&trace, &my->x);
        /*
        if(vec_to_screen(&trace, camera) != NULL)
        {
            panObjectiveMarker->pos_x = trace.x;
            panObjectiveMarker->pos_y = trace.y;
            draw_obj(panObjectiveMarker);
        }
        */

        if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
                                                          // If it fails then we're screwed
        {
            if(ent_get_type(hit->entity) == STATIC_TRIGGER) // Check if we're shooting at the right entity
            {
                __tracer = hit->entity;

                if(vec_dist(&player->x, &__tracer->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                {
                    __draw_hands();

                    if( !var_cmp(my->ambient, 100.0) )
                        my->ambient = 100.0;

                    if(mouse_left)
                    {
                        while(mouse_left)
                            wait(1.0);

                        if((int) my->skill78 == 1)
                        {
                            VECTOR  pos;
                            VECTOR  rot;
                            VECTOR  scl;

                            pos.x    = __tracer->skill1; pos.y    = __tracer->skill2; pos.z    = __tracer->skill3;
                            rot.x    = __tracer->skill4; rot.y    = __tracer->skill5; rot.z    = __tracer->skill6;
                            scl.x    = __tracer->skill7; scl.y    = __tracer->skill8; scl.z    = __tracer->skill9;

                            object = ent_create(game_asset_get_object(my->string1), &pos, NULL);

                            vec_set(&object->pan    , &rot);
                            vec_set(&object->scale_x, &scl);
                        }

                        if(!snd_playing(__trigger_event_sound_hndl)) // If the event sound is not currently played
                        {
                            if((int) my->skill79 == 1) // If we're able to create the sound from string2,
                                                       // Then create and play it.
                            {
                                if(NULL != __trigger_event_sound) // Remove the old sound object.
                                    snd_remove(__trigger_event_sound);

                                __trigger_event_sound = snd_create(game_asset_get_sound(my->string2));

                                if(pdsbs)
                                    pdsbs = NULL;

                                __trigger_event_sound_hndl = snd_play(__trigger_event_sound, game_volume, 0.0);
                            }
                        }

                            if((__tracer->flags & FLAG1) && __GameObjectives_singleton)
                                if(__GameObjectives_singleton->objectives < __GameObjectives_singleton->total_objectives)
                                    __GameObjectives_singleton->objectives += 1;

                        __tracer->flags |= (FLAG2); // Senpai ignore me
                        __tracer->skill88 = 0;
                        __tracer->ambient = my->skill71;

                        if( (int) __tracer->skill19 == 1 ) // If you want to delete the object after pressing the button, pass 1 to skill19.
                        {
                            safe_remove(__tracer);
                        }
                    }
                }
                else
                {
                    my->ambient = my->skill71;
                }

            } // End of if(ent_get_type(hit->entity) == STATIC_TRIGGER)
            else
            {
                my->ambient = my->skill71;
            }

        } // End of if(trace_hit && hit->entity).
        else
        {
            my->ambient = my->skill71;
        }

        wait(1.0);
    } // End of while(my).

    // Frees associated resources.
}

var local = 0;

void act_day1_radio()
{
    while( !player ) wait(1.0);

    ent_set_type(my, STATIC_TRIGGER);

    VECTOR shot_target;
    VECTOR trace;
    long   flags = IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

    my->skill88 = 1;
    my->skill71 = my->ambient;

    while(my)
    {
        vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
        vec_rotate( &shot_target, &camera->pan);
        vec_add( &shot_target, &camera->x);
        c_trace(&camera->x, &shot_target, flags);

        vec_set(&trace, &my->x);

        if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
        {
            if(ent_get_type(hit->entity) == STATIC_TRIGGER) // Check if we're shooting at the right entity
            {
                __tracer = hit->entity;

                if(vec_dist(&player->x, &__tracer->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                {
                    __draw_hands();

                    if( !var_cmp(my->ambient, 100.0) )
                        my->ambient = 100.0;

                    if(mouse_left)
                    {
                        while(mouse_left)
                            wait(1.0);

                        ent_playloop2(my, sndWasher, game_volume, 1500.0);

                        if((__tracer->flags & FLAG1) && __GameObjectives_singleton)
                            if(__GameObjectives_singleton->objectives < __GameObjectives_singleton->total_objectives)
                                __GameObjectives_singleton->objectives += 1;

                        __tracer->flags |= (FLAG2); // Senpai ignore me
                        __tracer->skill88 = 0;
                        __tracer->ambient = my->skill71;
                    }
                }
                else
                {
                    my->ambient = my->skill71;
                }

            } // End of if(ent_get_type(hit->entity) == STATIC_TRIGGER)
            else
            {
                my->ambient = my->skill71;
            }

        } // End of if(trace_hit && hit->entity).
        else
        {
            my->ambient = my->skill71;
        }

        wait(1.0);
    } // End of while(my).

    // Frees associated resources.
}

/*
 * void act_level_changer()
 *
 * A generic, tweakable level changing entity. By default, this entity will closes down the current level and loads the next one if:
 * - You've completed all of the objectives in the current level.
 *
 * string1: Contains the full file name of the translation file which will be popped up when the player comes close to the trigger entity.
 * string2: Contains the full file name of the sound effect you want to be played when the player hits the left mouse button.
 * skill11/DISTANCE: Distance between the player and the entity in which the trigger is activated.
 * skill2: ID of one of the initialized ViewPoint-s.
 * skill3: Which action you want to take afterwards. The complete table of IDs can be found here.
 */

#define    __SLEEP         1
#define    __LAUNCH_PC     2

void act_level_changer()
{
    while( !player ) wait(1.0);

    ent_set_type(my, STATIC_TRIGGER_LEVEL);
    SHOW_FLAGS_SAFE(my, POLYGON);

    VECTOR shot_target;
    long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;
    bool   custom_interact_text = false;

    Text *subtitle = txt_create(1, 998);

    Panel *fader = pan_create(NULL, 997);
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

    my->skill71 = my->ambient;

    while(my)
    {
        vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
        vec_rotate( &shot_target, &camera->pan);
        vec_add( &shot_target, &camera->x);
        c_trace(&camera->x, &shot_target, flags);

        if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
        {
            if(ent_get_type(hit->entity) == STATIC_TRIGGER_LEVEL) // Check if we're shooting at the right entity
            {
                if(vec_dist(player->x, my->x) <= my->DISTANCE && (game_gui_get_state() != STATE_PC) ) // Check if we're close enough to the entity.
                {
                    if( !var_cmp(my->ambient, 100.0) )
                        my->ambient = 100.0;

                    // SHOW_FLAGS_SAFE(subtitle, SHOW);
                    __draw_hands();

                    if(mouse_left)
                    {
                        while(mouse_left)
                            wait(1.0);

                        // HIDE_FLAGS_SAFE(fader, SHOW);
                        my->ambient = my->skill71;

                        switch((int) my->skill3) {
                            case    __LAUNCH_PC:
                            {
                                snd_play(event_sound, game_volume, 0.0);
                                act_player_camera_lock_to( (int) my->skill2 );

                                SHOW_FLAGS_SAFE(fader, SHOW);
                                while(fader->alpha < 100.0)
                                {
                                    fader->alpha += 3.5 * time_step;
                                    wait(1.0);
                                }
                                HIDE_FLAGS_SAFE(fader, SHOW);

                                game_gui_set_state(STATE_PC);
                                game_gui_render();

                                break;
                            }

                            case    __SLEEP:
                            {
                                if(__GameObjectives_singleton)
                                {
                                    if(__GameObjectives_singleton->objectives >= __GameObjectives_singleton->total_objectives)
                                    {
                                        snd_play(event_sound, game_volume, 0.0);
                                        act_player_camera_lock_to( (int) my->skill2 );

                                        SHOW_FLAGS_SAFE(fader, SHOW);
                                        while(fader->alpha < 100.0)
                                        {
                                            fader->alpha += 3.5 * time_step;
                                            wait(1.0);
                                        }
                                        HIDE_FLAGS_SAFE(fader, SHOW);

                                        game_scene_switch(game_day_get() + 1);

                                    }
                                    else
                                    {
                                        if((txtSubtitleHandler->pstring)[0] != lstr_not_done_jobs)
                                            (txtSubtitleHandler->pstring)[0] = lstr_not_done_jobs;
                                        object_draw(txtSubtitleHandler, 3.0);
                                    }
                                }

                                break;
                            }
                        } // End of switch((int) my->skill3).

                    }
                }
                else
                {
                    my->ambient = my->skill71;
                }

            } // End of if(ent_get_type(hit->entity) == STATIC_TRIGGER_LEVEL)
            else
            {
                my->ambient = my->skill71;
            }

        } // End of if(trace_hit && hit->entity)
        else
        {
            my->ambient = my->skill71;
        }

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

void act_special_door()
{
    #ifdef    DEBUG
        SHOW_FLAGS_SAFE(my, PASSABLE | TRANSLUCENT);
        my->alpha = 75.0;
    #else
        SHOW_FLAGS_SAFE(my, POLYGON);
    #endif
}

// string1: filename
// string2: audioname; time
// I wrote this while having a sleep deprivation, sorry.

__static void __gui_title_show(StaticTitleText *text, bool *out) {
    gui_title_show(text);
    WAIT_PROCESS(gui_title_show);

    *out = true;
}

/*
 * void act_text()
 *
 * Displays a string (for once) if the player comes close to the entity.
 * string1 specifies the name of the translation file (including the .lstr extension).
 * string2 specifies the name & size of the font you want to use when displaying the text.
 * skill1 and skill2 specifies the x and y position of the text, with respect to the upper-left corner of the screen.
 * skill3 is the timeout (in seconds, before the text disappears).
 * skill4, 5 and 6 specifies the R, G and B component of the color vector used to paint the text.
 */
void act_text()
{
    while( !player ) wait(1.0);

    #ifndef    DEBUG
        SHOW_FLAGS_SAFE(my, INVISIBLE);
    #endif

    #ifdef    DEBUG
        Object *sprite = ent_create(game_asset_get_2d_sprite("mail.png"), &my->x, NULL);
        sprite->flags |= (PASSABLE | UNLIT);
    #endif

    if(my->string1)
    {
        ent_set_type(my, STATIC_TEXT);

        my->string1 = region_get_string(my->string1); // Inhabits string1 with the contents found in the translation file.
        my->skill3  = ifelse(var_cmp(my->skill3, 0.0), 10.0, my->skill3);
        var r = abs(my->skill4),
            g = abs(my->skill5),
            b = abs(my->skill6);

        VECTOR shot_target;
        long   flags = IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

        // Parses the actual string containing the dialogue, using a fixed font.
        StaticTitleText *container = gui_title_new(
            vector(my->skill1, my->skill2, 0.0),
            vector(b, g, r), // In reverse order, because I want to use RGB.
            my->string1,
            my->skill3,
            LAYER_DEBUG_1
        );

        if(my->string2)
        {
            safe_remove(container->__container->font);
            container->__container->font = font_create(my->string2); // Hack to enable custom fonts.
        }

        gui_title_set_mode(container, FADE_OUT);

        bool once = false;

        while(my)
        {
            vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
            vec_rotate( &shot_target, &camera->pan);
            vec_add( &shot_target, &camera->x);
            c_trace(&camera->x, &shot_target, flags);

            if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
            {
                if(ent_get_type(hit->entity) == STATIC_TEXT) // Check if we're shooting at the right entity
                {
                    if(vec_dist(player->x, my->x) <= my->DISTANCE && !once) // Check if we're close enough to the entity.
                    {
                        __gui_title_show(container, &once);

                    } // End of if(vec_dist(player->x, my->x) <= my->DISTANCE).

                } // End of if(ent_get_type(hit->entity) == STATIC_TEXT).

            } // End of if(trace_hit && hit->entity).

            wait(1.0);
        } // End of while(my).
    }
}

/*
 * void act_text_touch()
 *
 * Displays a string (for once) if the player comes close to the entity.
 * string1 specifies the name of the translation file (including the .lstr extension).
 * string2 specifies the name & size of the font you want to use when displaying the text.
 * skill1 and skill2 specifies the x and y position of the text, with respect to the upper-left corner of the screen.
 * skill3 is the timeout (in seconds, before the text disappears).
 * skill4, 5 and 6 specifies the R, G and B component of the color vector used to paint the text.
 */
void act_text_touch()
{
    while( !player ) wait(1.0);

    #ifndef    DEBUG
        SHOW_FLAGS_SAFE(my, INVISIBLE);
    #endif

    #ifdef    DEBUG
        Object *sprite = ent_create(game_asset_get_2d_sprite("mail.png"), &my->x, NULL);
        sprite->flags |= (PASSABLE | UNLIT);
    #endif

    if(my->string1)
    {
        my->string1 = region_get_string(my->string1); // Inhabits string1 with the contents found in the translation file.

        my->skill3  = ifelse(var_cmp(my->skill3, 0.0), 10.0, my->skill3);
        var r = abs(my->skill4),
            g = abs(my->skill5),
            b = abs(my->skill6);

        // Parses the actual string containing the dialogue, using a fixed font.
        StaticTitleText *container = gui_title_new(
            vector(my->skill1, my->skill2, 0.0),
            vector(b, g, r), // In reverse order, because I want to use RGB.
            my->string1,
            my->skill3,
            LAYER_DEBUG_1
        );

        if( my->string2 ) // New font specified
        {
            safe_remove( container->__container->font );
            container->__container->font = font_create( my->string2 );
        }

        if(my->flags & FLAG6) // x-align
            container->__container->pos_x = (screen_size.x - str_width(container->content, container->__container->font)) / 2.0;
        if(my->flags & FLAG7) // y-align
            container->__container->pos_y = screen_size.y / 2.0;

        if(my->flags & FLAG8) // fine-tuning
        {
            container->__container->pos_x += my->skill1;
            container->__container->pos_y += my->skill2;
        }

        gui_title_set_mode( container, FADE_OUT );
        gui_title_set_delay( container, ifelse(var_cmp(my->skill7, 0.0), 0.05, my->skill7) );

        bool once = false;

        while(my)
        {
            if(vec_dist(player->x, my->x) <= my->DISTANCE && !once) // Check if we're close enough to the entity.
            {
                __gui_title_show(container, &once);

            } // End of if(vec_dist(player->x, my->x) <= my->DISTANCE).

            wait(1.0);
        } // End of while(my).

        if(container)
            gui_title_free(container);
    }
}

void act_dream_door()
{
    while( !player )
        wait(1.0);

    my->skill1 = abs(ifelse(var_cmp(my->skill1, 0.0), 4.2, my->skill1));

    while(my) {
        my->pan += my->skill1 * time_step;
        wait(1.0);
    }
}

void act_dream2_fog()
{
    while( !player )
        wait(1.0);

    #ifndef    DEBUG
        SHOW_FLAGS_SAFE(my, INVISIBLE);
    #endif

    my->skill1 = ifelse(var_cmp(my->skill1, 0.0), 4.2, my->skill1);

    while(my)
    {
        if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
        {
            var old_fog_end   = camera->fog_end;

            while(camera->fog_end > camera->fog_start)
            {
                camera->fog_end -= my->skill1 * time_step;
                wait(1.0);
            }

            vec_set(&d3d_fogcolor1, vector(255.0, 255.0, 255.0));

            while(camera->fog_end < old_fog_end)
            {
                camera->fog_end += my->skill1 * time_step;
                wait(1.0);
            }

            safe_remove(my);

        } // End of if(vec_dist(player->x, my->x) <= my->DISTANCE).

        wait(1.0);
    } // End of while(my).
}

void act_dream3_fog()
{
    while( !player )
        wait(1.0);

    #ifndef    DEBUG
        SHOW_FLAGS_SAFE(my, INVISIBLE);
    #endif

    my->skill1 = ifelse(var_cmp(my->skill1, 0.0), 4.2, my->skill1);

    while(my)
    {
        if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
        {
            var old_fog_end   = camera->fog_end;

            game_mplayer_stop();

            while(camera->fog_end > camera->fog_start)
            {
                camera->fog_end -= my->skill1 * time_step;
                wait(1.0);
            }

            vec_set(&d3d_fogcolor1, vector(255.0, 255.0, 255.0));

            while(camera->fog_end < old_fog_end)
            {
                camera->fog_end += my->skill1 * time_step;
                wait(1.0);
            }

            safe_remove(my);

        } // End of if(vec_dist(player->x, my->x) <= my->DISTANCE).

        wait(1.0);
    } // End of while(my).
}

Object *dream3_truck = NULL;
Object *dream3_block = NULL;

void act_dream_truck()
{
    while( !player )
        wait(1.0);

    dream3_truck = my;
}

// Level closures.
void act_dream2_closure()
{
    while( !player && !dream3_truck )
        wait(1.0);

    #ifndef    DEBUG
        SHOW_FLAGS_SAFE(my, INVISIBLE);
    #endif

    dream3_block = my;

    while(my)
    {
        if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
        {
            snd_play(snd_create(game_asset_get_sound("car-horn.wav")), game_volume, 0.0);
            snd_play(snd_create(game_asset_get_sound("car-crash.wav")), game_volume, 0.0);

            while(vec_dist(my->x, dream3_truck->x) > 150.0)
            {
                dream3_truck->x -= 126.0 * time_step;
                wait(1.0);
            }

            game_scene_switch( game_day_get() + 1 );

        } // End of if(vec_dist(player->x, my->x) <= my->DISTANCE).

        wait(1.0);
    }
}

void act_dream1_closure()
{
    while( !player )
        wait(1.0);

    #ifndef    DEBUG
        SHOW_FLAGS_SAFE(my, INVISIBLE);
    #endif

    Panel *fader = pan_create(NULL, 997);
    fader->bmap  = bmap_createblack(screen_size.x, screen_size.y, 8);

    while(my)
    {
        if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
        {
            SHOW_FLAGS_SAFE(fader, SHOW);
            float time = .0;
            while(time < 1.0)
            {
                time += time_step / 16.0;
                wait(1.0);
            }
            HIDE_FLAGS_SAFE(fader, SHOW);

            game_scene_switch( game_day_get() + 1 );

        } // End of if(vec_dist(player->x, my->x) <= my->DISTANCE).

        wait(1.0);
    }
}

void act_dream3_door()
{
    while( !player ) wait(1.0);

    ent_set_type(my, STATIC_DOOR);

    // Correct the distance.
    my->DISTANCE = (var) ifelse(my->DISTANCE <= 0.0, 128.0, my->DISTANCE);

    // Create sound sources if possible - string1 contains the door opening sound, string2 contains the door closing sound.
    SOUND *src_open   = NULL;
    SOUND *src_close  = NULL;

    if(my->string1)
        src_open = snd_create(game_asset_get_sound(my->string1));
    if(my->string2)
        src_close = snd_create(game_asset_get_sound(my->string2));

    VECTOR shot_target;
    long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

    Panel *fader = pan_create(NULL, 997);
    fader->bmap  = bmap_createblack(screen_size.x, screen_size.y, 8);

    while(my)
    {
        vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
        vec_rotate( &shot_target, &camera->pan);
        vec_add( &shot_target, &camera->x);
        c_trace(&camera->x, &shot_target, flags);

        if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
        {
            if(ent_get_type(hit->entity) == STATIC_DOOR) // Check if we're shooting at the right entity
            {
                if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                {
                    if(!mouse_left)
                    {
                        if((txtSubtitleHandler->pstring)[0] != lstr_open_door)
                            (txtSubtitleHandler->pstring)[0] = lstr_open_door;

                        __draw_hands();
                    }
                    else
                    {
                        while(mouse_left)
                            wait(1.0);

                        if(src_open)
                            snd_play(src_open, game_volume, 0.0);

                        SHOW_FLAGS_SAFE(fader, SHOW);
                        float timer = 0.0;
                        while(timer < 1.0)
                        {
                            timer += time_step / 16;
                            wait(1.0);
                        }
                        HIDE_FLAGS_SAFE(fader, SHOW);

                        ending = (int) my->skill1;
                        game_scene_switch( game_day_get() + 1 );
                    }
                }

            } // End of if(ent_get_type(hit->entity) == STATIC_DOOR)
        } // End of if(trace_hit && hit->entity)

        wait(1.0);
    }
}

void act_final_door()
{
    while( !player ) wait(1.0);

    ent_set_type(my, STATIC_DOOR);

    // Correct the distance.
    my->DISTANCE = (var) ifelse(my->DISTANCE <= 0.0, 128.0, my->DISTANCE);

    // Create sound sources if possible - string1 contains the door opening sound, string2 contains the door closing sound.
    SOUND *src_open   = NULL;

    if(my->string1)
        src_open = snd_create(game_asset_get_sound(my->string1));

    VECTOR shot_target;
    long   flags = IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_WORLD | USE_POLYGON | IGNORE_FLAG2;

    Panel *fader = pan_create(NULL, 997);
    fader->bmap  = bmap_createblack(screen_size.x, screen_size.y, 8);

    var final = ent_playloop2(my, snd_create(game_asset_get_sound("door-knock.wav")), game_volume, 1500.0);

    while(my)
    {
        vec_set( &shot_target, vector(my->DISTANCE, 0.0, 0.0)); // The second parameter takes the scan range.
        vec_rotate( &shot_target, &camera->pan);
        vec_add( &shot_target, &camera->x);
        c_trace(&camera->x, &shot_target, flags);

        if(trace_hit && hit->entity && hit->entity == me) // Check if we're shooting an entity instead of static geometry/sprites/level blocks/<..>
        {
            if(ent_get_type(hit->entity) == STATIC_DOOR) // Check if we're shooting at the right entity
            {
                if(vec_dist(player->x, my->x) <= my->DISTANCE) // Check if we're close enough to the entity.
                {
                    if(!mouse_left)
                    {
                        if((txtSubtitleHandler->pstring)[0] != lstr_open_door)
                            (txtSubtitleHandler->pstring)[0] = lstr_open_door;

                        __draw_hands();
                    }
                    else
                    {
                        while(mouse_left)
                            wait(1.0);

                        if(src_open)
                            snd_play(src_open, game_volume, 0.0);

                        SHOW_FLAGS_SAFE(fader, SHOW);
                        level_load(game_asset_get_scene("empty.wmb"));

                        switch(ending) {
                            case    BAD_ENDING:
                            {
                                Sound *stab[2];
                                stab[0]  = snd_create(game_asset_get_sound("knife_stab.wav"));
                                stab[1]  = snd_create(game_asset_get_sound("knife_stab2.wav"));

                                Sound *moan[2];
                                moan[0]  = snd_create(game_asset_get_sound("moan-1.wav"));
                                moan[1]  = snd_create(game_asset_get_sound("moan-2.wav"));

                                Sound *knife_throw = snd_create(game_asset_get_sound("knife_drop.wav"));

                                int times = 0;
                                float time = .0;
                                var hx = 0;

                                for(; times < 3; ++times)
                                {
                                    float inter_time = (float)(random(4.5) + 1.25);

                                    while(time < inter_time)
                                    {
                                        time += time_step / 16.0;
                                        wait(1.0);
                                    }
                                    time = .0;

                                    hx = snd_play( stab[rand() % 2], game_volume, 0.0 );
                                    WAIT_SOUND(hx);
                                }

                                hx = snd_play( moan[rand() % 2], game_volume, 0.0 );
                                WAIT_SOUND(hx);

                                hx = snd_play( knife_throw, game_volume, 0.0 );
                                WAIT_SOUND(hx);

                                for(times = 0; times < 2; ++times)
                                {
                                    snd_remove( stab[times] );
                                    snd_remove( moan[times] );
                                }
                                snd_remove( knife_throw );

                                break;
                            }

                            case    GOOD_ENDING:
                            {
                                var hx = 0;
                                float time = .0;
                                int times = 0;

                                Sound *wind        = snd_create(game_asset_get_sound("wnd1.wav"));
                                Sound *door_close  = snd_create(game_asset_get_sound("door-1-close.wav"));
                                Sound *knife_throw = snd_create(game_asset_get_sound("knife_drop.wav"));

                                for(; times < 7; ++times)
                                {
                                    WAIT_SOUND(hx);
                                    while(time < .5)
                                    {
                                        time += time_step / 16.0;
                                        wait(1.0);
                                    }
                                    time = .0;

                                    int r = rand() % FOOTSTEP_SOUND_VARIANTS;
                                    hx = snd_play((__act_player_state_singleton->__footstep_sound)[r], game_volume * 0.2 , 0.0);
                                }

                                hx = snd_play(knife_throw, game_volume * 0.6, 0.0);
                                WAIT_SOUND(hx);
                                snd_remove(knife_throw);

                                hx = snd_play(door_close, game_volume * 0.4, 0.0);
                                WAIT_SOUND(hx);
                                snd_remove(door_close);

                                snd_play(wind, game_volume * 0.2, 0.0);
                                // ...could cause leak... we need that sound.

                                break;
                            }
                        }

                        float timer = 0.0;
                        while(timer < 1.0)
                        {
                            timer += time_step / 16;
                            wait(1.0);
                        }

                        game_gui_set_state(STATE_ENDING);
                        game_gui_render();

                        /*
                        StaticTitleText *endingTitle =  gui_title_new(
                            vector(15.0, 15.0, 0.0),
                            COLOR_WHITE,
                            sifelse(ending == BAD_ENDING, region_get_string("c5-door1.lstr"), region_get_string("c5-door2.lstr")),
                            10.0,
                            998
                        );

                        endingTitle->__container->font = font_create("UVN remind#35b");
                        gui_title_set_mode(endingTitle, FADE_OUT);
                        gui_title_set_delay(endingTitle, 0.15);
                        gui_title_show(endingTitle);
                        */

                        /*
                        if(ending == BAD_ENDING)
                            game_mplayer_play("end-1");
                        else
                            game_mplayer_play("end-2");
                        */
                    }

                }

            } // End of if(ent_get_type(hit->entity) == STATIC_DOOR)
        } // End of if(trace_hit && hit->entity)

        wait(1.0);
    }
}

action act_day2_fallbox()
{
    while( !player )
        wait(1.0);

    #ifndef    DEBUG
        SHOW_FLAGS_SAFE(my, INVISIBLE);
    #endif

    Panel *fader = pan_create(NULL, 997);
    fader->bmap  = bmap_createblack(screen_size.x, screen_size.y, 8);

    while(my)
    {
        if(vec_dist(player->x, my->x) <= my->DISTANCE && (__GameObjectives_singleton->objectives >= __GameObjectives_singleton->total_objectives) ) // Check if we're close enough to the entity.
        {
            SHOW_FLAGS_SAFE(fader, SHOW);

            var hy = snd_play(sndHumanFall, game_volume, 0.0);

            float time = .0;
            while(time < 1.0)
            {
                time += time_step / 16.0;
                wait(1.0);
            }
            HIDE_FLAGS_SAFE(fader, SHOW);

            game_scene_switch( game_day_get() + 1 );

        } // End of if(vec_dist(player->x, my->x) <= my->DISTANCE).

        wait(1.0);
    }
}
