/* gui.c */
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
 */
__static int __contact_number = 0;
__static int __msg_sent       = 0;

// This small piece of local function simulates the effect of opening a Windows XP window.
// In general, you don't need to take a look at the <gui> header because it's very specific
// to the game being made and in Acknex, there isn't any tool available to do drag-dropping,
// so, in general, plain C coding is apparently the only way to make the GUI up and working.
__static void __window_open(const char *which)
{
    /*
    if(!snd_playing(sndMouseClickHandle))
        sndMouseClickHandle = snd_play(sndMouseClick, game_volume, 0.0);
    */
    snd_play(sndMouseClick, game_volume, 0.0);

    if(__GUIState_singleton->PC_window->bmap)
        bmap_remove(__GUIState_singleton->PC_window->bmap);

    __GUIState_singleton->PC_window->bmap = bmap_create(which);

    if(GET_FLAGS_SAFE(__GUIState_singleton->PC_window, SHOW) == false)
        SHOW_FLAGS_SAFE(__GUIState_singleton->PC_window, SHOW);
}

__static void __close_window()
{
    /*
    if(!snd_playing(sndMouseClickHandle))
        sndMouseClickHandle = snd_play(sndMouseClick, game_volume, 0.0);
    */
    snd_play(sndMouseClick, game_volume, 0.0);

    // Close the main window.
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_window, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_string_skyde_log_in, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_skyde_contacts, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_skyde_messagebox, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_news_list, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_string_skyde_chat_with, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_string_news, SHOW);

    // Close the associated GUI resources.
    gui_button_hide(__GUIState_singleton->PC_icon_skyde_login);
    gui_button_hide(__GUIState_singleton->PC_icon_skyde_exit);
    gui_button_hide(__GUIState_singleton->PC_icon_skyde_blob1);
    gui_button_hide(__GUIState_singleton->PC_icon_skyde_send);

    int i = 0;
    for(; i < 4; i++)
        if((__GUIState_singleton->PC_icon_news_provider)[i])
            gui_button_hide( (__GUIState_singleton->PC_icon_news_provider)[i] );

    // Reset stuff to their default values.
    (__GUIState_singleton->PC_string_skyde_log_in->pstring)[0] = (__GUIState_singleton->PC_string_table->pstring)[6];
}

__static void __skyde_send_message()
{
    /*
    if(!snd_playing(sndMouseClickHandle))
        sndMouseClickHandle = snd_play(sndMouseClick, game_volume, 0.0);
    */
    snd_play(sndMouseClick, game_volume, 0.0);

    if(__contact_number != 3) // It's not her!
    {
         // printf("Not her!");
         snd_play(sndInvalidClick, game_volume, 0.0);
    }
    else
    {
        if( !__msg_sent )
        {
            gui_button_show(__GUIState_singleton->PC_icon_skyde_blob1);

            if(__GameObjectives_singleton)
                if(__GameObjectives_singleton->objectives < __GameObjectives_singleton->total_objectives)
                    __GameObjectives_singleton->objectives += 1;

            snd_play(sndMessageSent, game_volume, 0.0);
            __msg_sent = 1;
        }
    }
}

__static void __skyde_sign_in()
{
    /*
    if(!snd_playing(sndMouseClickHandle))
        sndMouseClickHandle = snd_play(sndMouseClick, game_volume, 0.0);
    */
    snd_play(sndMouseClick, game_volume, 0.0);

    (__GUIState_singleton->PC_string_skyde_log_in->pstring)[0]     = (__GUIState_singleton->PC_string_table->pstring)[7];
    gui_button_hide(__GUIState_singleton->PC_icon_skyde_login);
    gui_button_hide(__GUIState_singleton->PC_icon_skyde_exit);

    float time         = random(2.0) + 1.5;
    float time_counter = 0.0;

    while(time_counter < time) // Make the player wait for a random amount of time, simulating the
                               // "signing in" effect.
    {
        time_counter += time_step / 16.0;
        wait(1.0);
    }

    (__GUIState_singleton->PC_string_skyde_log_in->pstring)[0]     = (__GUIState_singleton->PC_string_table->pstring)[8];
    SHOW_FLAGS_SAFE(__GUIState_singleton->PC_skyde_contacts, SHOW);
}

__static void __contacts_click(var number) // Parameter types need to be var (fixed), not int, otherwise could cause problems.
{
    /*
    if(!snd_playing(sndMouseClickHandle))
        sndMouseClickHandle = snd_play(sndMouseClick, game_volume, 0.0);
    */
    snd_play(sndMouseClick, game_volume, 0.0);

    __contact_number = (int) number; // Pass the contact ID to __contact_number so we can use it later when checking
                                     // if the "correct" contact was chosen.

    SHOW_FLAGS_SAFE(__GUIState_singleton->PC_skyde_messagebox, SHOW);
    gui_button_show(__GUIState_singleton->PC_icon_skyde_send);

    if( (int) number != 3 ) // 0.. 1.. 2.. Half-Life 3 confirmed!
    {
        HIDE_FLAGS_SAFE(__GUIState_singleton->PC_string_skyde_chat_with, SHOW);
        gui_button_hide(__GUIState_singleton->PC_icon_skyde_blob1);
    }
    else
    {
        SHOW_FLAGS_SAFE(__GUIState_singleton->PC_string_skyde_chat_with, SHOW);

        if( __msg_sent ) // We've sent the message before, so show it again.
            gui_button_show(__GUIState_singleton->PC_icon_skyde_blob1);
    }
}

__static void __skyde_click()
{
    __window_open(game_asset_get_gui("PC_window_skyde.jpg"));

    SHOW_FLAGS_SAFE(__GUIState_singleton->PC_string_skyde_log_in, SHOW);
    gui_button_show(__GUIState_singleton->PC_icon_skyde_login);
    gui_button_show(__GUIState_singleton->PC_icon_skyde_exit);
}

__static void __news_click()
{
    /*
    __window_open(game_asset_get_gui("PC_window_news.jpg"));

    SHOW_FLAGS_SAFE(__GUIState_singleton->PC_string_news, SHOW);

    int i = 0;
    for(; i < 4; i++)
        if(NULL != (__GUIState_singleton->PC_icon_news_provider)[i])
            gui_button_show( (__GUIState_singleton->PC_icon_news_provider)[i] );
    */

    snd_play(sndInvalidClick, game_volume, 0.0);

    return;
}

__static void __GUIState_hide_PC();

__static void __shutdown_click()
{
    // In short, this macro flicks the desktop icons for msec frames, simulating the behavior of the Windows XP icons when the machine is shut down.
    #define    __FLICK_ICONS(msec)  gui_button_hide(__GUIState_singleton->PC_icon_shutdown); gui_button_hide(__GUIState_singleton->PC_icon_skyde); gui_button_hide(__GUIState_singleton->PC_icon_news); wait(##msec##); gui_button_show(__GUIState_singleton->PC_icon_shutdown); gui_button_show(__GUIState_singleton->PC_icon_skyde); gui_button_show(__GUIState_singleton->PC_icon_news)

    /*
    if(!snd_playing(sndMouseClickHandle))
        sndMouseClickHandle = snd_play(sndMouseClick, game_volume, 0.0);
    */
    snd_play(sndMouseClick, game_volume, 0.0);

    if(STATE_PC == game_gui_get_state())
    {
        if(GET_FLAGS_SAFE(__GUIState_singleton->PC_window, SHOW) == true) // The same as is().
            HIDE_FLAGS_SAFE(__GUIState_singleton->PC_window, SHOW);

        __FLICK_ICONS(1.1 + random(4.2));
        __FLICK_ICONS(1.4 + random(8.7));

        gui_button_hide(__GUIState_singleton->PC_icon_shutdown);
        gui_button_hide(__GUIState_singleton->PC_icon_skyde);
        gui_button_hide(__GUIState_singleton->PC_icon_news);

        mouse_mode = 0;
        if(__GUIState_singleton->PC_cursor_original)
            mouse_map = __GUIState_singleton->PC_cursor_original;

        __GUIState_singleton->PC_wallpaper->bmap = __GUIState_singleton->PC_wallpaper_logoff;

        SHOW_FLAGS_SAFE(__GUIState_singleton->PC_wallpaper, TRANSLUCENT);
        __GUIState_singleton->PC_wallpaper->alpha = 100.0;

        snd_play(sndPCLogout, game_volume, 0.0);

        while(__GUIState_singleton->PC_wallpaper->alpha > 0.0)
        {
            __GUIState_singleton->PC_wallpaper->alpha -= 4.2* time_step;
            wait(1.0);
        }

        HIDE_FLAGS_SAFE(__GUIState_singleton->PC_wallpaper, TRANSLUCENT);
        HIDE_FLAGS_SAFE(__GUIState_singleton->PC_wallpaper, SHOW);

        game_gui_set_state( STATE_NULL );
        game_gui_render();
    }
}

__static void ___continue_event()
{
    game_gui_set_state(STATE_NULL);
    game_gui_render();

    if(player != NULL) { // This means we're in the game.

    } else {

        SHOW_FLAGS_SAFE(__GUIState_singleton->menu_keymap, SHOW);
        while(__GUIState_singleton->menu_keymap->alpha < 100.0)
        {
            __GUIState_singleton->menu_keymap->alpha += 4.2 * time_step;
            wait(1.0);
        }

        while( !key_enter )
            wait(1.0);

        while(__GUIState_singleton->menu_keymap->alpha > 0.0)
        {
            __GUIState_singleton->menu_keymap->alpha -= 4.2 * time_step;
            wait(1.0);
        }
        HIDE_FLAGS_SAFE(__GUIState_singleton->menu_keymap, SHOW);

        game_scene_switch(DAY_1);
    }
}

__static void __exit_event()
{
    __game_event_on_close2();
}

/*
 * void game_gui_state_new()
 *
 * Allocates and initializes a new GUI state.
 * After this call, the GUI can't still be rendered as you have to set up various parameters for the state singleton
 * (behaviors and bitmaps) with various game_gui_set_*() calls.
 * Finally, to render the GUI (taking states into account), call game_gui_render().
 * State management is done through game_gui_set_state() and game_gui_get_state().
 */
void game_gui_state_new()
{
    if( __GUIState_singleton )
        game_gui_state_free(); // Then we'll just re-initialize the GUI state.

    __GUIState_singleton                     = MALLOC(1, GUIState);
    __GUIState_singleton->state              = 0;

    // Creates and sets up the reticule.
    __GUIState_singleton->reticule           = pan_create(NULL, LAYER_GUI_1);
    __GUIState_singleton->paper_texture      = pan_create(NULL, LAYER_GUI_1);
    __GUIState_singleton->todo_texture       = pan_create(NULL, LAYER_GUI_1);

    __GUIState_singleton->reticule->flags    = __GUIState_singleton->reticule->flags | (OVERLAY);
    gui_panel_set_pos( __GUIState_singleton->reticule, 0.0, 0.0 );

    __GUIState_singleton->generic_black       = pan_create(NULL, LAYER_GUI_2);
    __GUIState_singleton->generic_black->bmap = bmap_createblack(screen_size.x, screen_size.y, 8);

    __GUIState_singleton->interact_icon        = pan_create(NULL, 100);
    __GUIState_singleton->interact_icon->bmap  = bmap_create(game_asset_get_gui("hand_icon.png"));
    __GUIState_singleton->interact_icon->pos_x = (screen_size.x - bmap_width(__GUIState_singleton->interact_icon->bmap)) / 2.0;
    __GUIState_singleton->interact_icon->pos_y = (screen_size.y - bmap_height(__GUIState_singleton->interact_icon->bmap)) / 2.0;

    __GUIState_singleton->todo_texture->bmap   = bmap_create(game_asset_get_gui("objectives.jpg"));
    __GUIState_singleton->todo_texture->pos_x  = (screen_size.x - bmap_width(__GUIState_singleton->todo_texture->bmap)) / 2.0;
    __GUIState_singleton->todo_texture->pos_y  = (screen_size.y - bmap_height(__GUIState_singleton->todo_texture->bmap)) / 2.0;

    /* STATE_INTRO */ {
        __GUIState_singleton->intro_screen          = pan_create(NULL, 999);
        __GUIState_singleton->intro_screen->bmap    = bmap_createblack(screen_size.x, screen_size.y, 16);

        __GUIState_singleton->intro_text                = txt_create(1, 1001);
        __GUIState_singleton->intro_text->font          = font_create("[ank]*#25b");
        __GUIState_singleton->intro_text->pos_x         = 15.0;
        __GUIState_singleton->intro_text->pos_y         = screen_size.y / 2;
        __GUIState_singleton->intro_text->flags        |= TRANSLUCENT;
        __GUIState_singleton->intro_text->alpha         = 0.0;
        (__GUIState_singleton->intro_text->pstring)[0]  = lstr_beta_warning;
    }

    /* STATE_MAIN_MENU */ {
        __GUIState_singleton->menu_cursor           = bmap_create(game_asset_get_gui("arrow.bmp"));

        __GUIState_singleton->menu_options          = pan_create(NULL, 999);
        __GUIState_singleton->menu_options->pos_x   = 0;
        __GUIState_singleton->menu_options->pos_y   = 0;
        __GUIState_singleton->menu_options->bmap    = bmap_createblack(screen_size.x, screen_size.y, 16);

        __GUIState_singleton->menu_keymap           = pan_create(NULL, 1000);
        __GUIState_singleton->menu_keymap->bmap     = bmap_create(game_asset_get_gui("key_layout.bmp"));
        __GUIState_singleton->menu_keymap->pos_x    = (screen_size.x - bmap_width(__GUIState_singleton->menu_keymap->bmap)) / 2.0;
        __GUIState_singleton->menu_keymap->pos_y    = (screen_size.y - bmap_height(__GUIState_singleton->menu_keymap->bmap)) / 2.0;
        __GUIState_singleton->menu_keymap->flags   |= TRANSLUCENT;
        __GUIState_singleton->menu_keymap->alpha    = 0.0;

        __GUIState_singleton->menu_logo             = pan_create(NULL, 1000);
        __GUIState_singleton->menu_logo->bmap       = bmap_create(game_asset_get_gui("title.bmp"));
        __GUIState_singleton->menu_logo->pos_x      = (screen_size.x - bmap_width(__GUIState_singleton->menu_logo->bmap)) / 2.0;
        __GUIState_singleton->menu_logo->pos_y      = 50.0;
        __GUIState_singleton->menu_logo->flags     |= OVERLAY;

        __GUIState_singleton->menu_options_digit    = pan_create(NULL, 1000);
        pan_setdigits(__GUIState_singleton->menu_options_digit, 0,
            0.0, 5.0,
            "%.2f",
            font_create("UVN remind#25b"),
            1,
            fov);

        pan_setdigits(__GUIState_singleton->menu_options_digit, 0,
            0.0, 92.0,
            "%.1f",
            font_create("UVN remind#25b"),
            1,
            sensivity_mul);

        pan_setdigits(__GUIState_singleton->menu_options_digit, 0,
            0.0, 165.0,
            "%.1f",
            font_create("UVN remind#25b"),
            1,
            volume);

        __GUIState_singleton->menu_options_2                = txt_create(1, 1000);
        __GUIState_singleton->menu_options_2->font          = Loading_Font;
        __GUIState_singleton->menu_options_2->pos_x         = (screen_size.x - str_width(lstr_options_2, __GUIState_singleton->menu_options_2->font)) / 2.0;
        __GUIState_singleton->menu_options_2->pos_y         = screen_size.y - 50.0;
        (__GUIState_singleton->menu_options_2->pstring)[0]  = lstr_options_2;

        __GUIState_singleton->menu_options_3                = txt_create(1, 1000);
        __GUIState_singleton->menu_options_3->font          = Loading_Font;
        (__GUIState_singleton->menu_options_3->pstring)[0]  = lstr_options_3;

        float sx = __GUIState_singleton->menu_logo->pos_x / 2.0;
        float sy = (__GUIState_singleton->menu_logo->pos_y + bmap_height(__GUIState_singleton->menu_logo->bmap)) - 42.0;

        pan_setslider(__GUIState_singleton->menu_options, 0, sx, sy + 42.0, bmap_create(game_asset_get_gui("button_options_slider_map.bmp")), bmap_create(game_asset_get_gui("button_options_slider.bmp")), 60.0, 120.0, fov );
        pan_setslider(__GUIState_singleton->menu_options, 0, sx, sy + 126.0, bmap_create(game_asset_get_gui("button_options_slider_map.bmp")), bmap_create(game_asset_get_gui("button_options_slider.bmp")), 0.5, 3.0, sensivity_mul );
        pan_setslider(__GUIState_singleton->menu_options, 0, sx, sy + 200.0, bmap_create(game_asset_get_gui("button_options_slider_map.bmp")), bmap_create(game_asset_get_gui("button_options_slider.bmp")), 0.0, 100.0, volume );

        __GUIState_singleton->menu_options_3->pos_x         = sx;
        __GUIState_singleton->menu_options_3->pos_y         = (sy + 42.0) - 25.0;

        __GUIState_singleton->menu_options_digit->pos_x     = sx + 256.0 + 16.0;
        __GUIState_singleton->menu_options_digit->pos_y     = sy + 28.0;

        float px = __GUIState_singleton->menu_logo->pos_x + (0.5 * bmap_width(__GUIState_singleton->menu_logo->bmap));
        float py = __GUIState_singleton->menu_logo->pos_y + bmap_height(__GUIState_singleton->menu_logo->bmap);

        pan_setbutton(__GUIState_singleton->menu_options, 0, 0, px, py + 16.0 + 64.0,
            bmap_create(game_asset_get_gui("button_exit_on.bmp")),
            bmap_create(game_asset_get_gui("button_exit_off.bmp")),
            bmap_create(game_asset_get_gui("button_exit_on.bmp")),
            bmap_create(game_asset_get_gui("button_exit_off.bmp")),
            __exit_event,
            NULL,
            NULL);

        pan_setbutton(__GUIState_singleton->menu_options, 0, 0, px, py + 16.0 + 32.0,
            bmap_create(game_asset_get_gui("button_continue_on.bmp")),
            bmap_create(game_asset_get_gui("button_continue_off.bmp")),
            bmap_create(game_asset_get_gui("button_continue_on.bmp")),
            bmap_create(game_asset_get_gui("button_continue_off.bmp")),
            ___continue_event,
            NULL,
            NULL);
    }

    /* STATE_ENDING */ {
        __GUIState_singleton->ending_background          = pan_create(NULL, 999);
        __GUIState_singleton->ending_background->bmap    = bmap_create(game_asset_get_gui("ending_cover.bmp"));
        __GUIState_singleton->ending_background->pos_x   = (screen_size.x - bmap_width(__GUIState_singleton->ending_background->bmap)) / 2.0;
        __GUIState_singleton->ending_background->pos_y   = (screen_size.y - bmap_height(__GUIState_singleton->ending_background->bmap)) / 2.0;

        __GUIState_singleton->ending_text                = pan_create(NULL, 1000);
        __GUIState_singleton->ending_text->pos_x         = 0;
        __GUIState_singleton->ending_text->pos_y         = 0;
        __GUIState_singleton->ending_text->flags        |= TRANSLUCENT;

        __GUIState_singleton->ending_text_0              = bmap_create(game_asset_get_gui("ending_cover_0.bmp"));
        __GUIState_singleton->ending_text_1              = bmap_create(game_asset_get_gui("ending_cover_1.bmp"));
        __GUIState_singleton->ending_text_2              = bmap_create(game_asset_get_gui("ending_cover_2.bmp"));
        __GUIState_singleton->ending_text_3              = bmap_create(game_asset_get_gui("ending_cover_3.bmp"));

        __GUIState_singleton->ending_secrets_found         = txt_create(1, 1000);
        __GUIState_singleton->ending_secrets_found->flags |= TRANSLUCENT;
        __GUIState_singleton->ending_secrets_found->font   = font_create("[ank]*#30b");

        __GUIState_singleton->ending_exit                  = txt_create(1, 1000);
        str_cpy( (__GUIState_singleton->ending_exit->pstring)[0], "Press [Enter] to exit" );
        __GUIState_singleton->ending_exit->flags          |= (TRANSLUCENT);
        __GUIState_singleton->ending_exit->font            = font_create("[ank]*#20");
        __GUIState_singleton->ending_exit->alpha           = .0;
        gui_text_set_pos( __GUIState_singleton->ending_exit,
            (screen_size.x - str_width((__GUIState_singleton->ending_exit->pstring)[0], __GUIState_singleton->ending_exit->font)) / 2.0,
            screen_size.y   - 45.0 );
    }

    /* STATE_PC */ {
        __GUIState_singleton->PC_wallpaper_wallpaper = bmap_create(game_asset_get_gui("PC_wallpaper.jpg"));
        __GUIState_singleton->PC_wallpaper_logoff = bmap_create(game_asset_get_gui("PC_logout.jpg"));
        __GUIState_singleton->PC_wallpaper_boot = bmap_create(game_asset_get_gui("PC_boot.jpg"));

        #define    __CREATE_GUI_BACKGROUNDS(elem, img, layer) __GUIState_singleton->##elem## = pan_create(NULL, ##layer##); __GUIState_singleton->##elem##->bmap = bmap_create(game_asset_get_gui(##img##)); gui_panel_set_center(__GUIState_singleton->##elem##, CENTER_X | CENTER_Y)

        __CREATE_GUI_BACKGROUNDS(PC_wallpaper,       "PC_wallpaper.jpg",    LAYER_GUI_3);
        __CREATE_GUI_BACKGROUNDS(PC_window,          "PC_window_skyde.jpg", LAYER_GUI_6);

        // Uses txt_loadw() directly without taking the current used language into consideration. This is fine, because:
        // 1. Unicode texts can render normal, non-Unicode texts just fine.
        // 2. The GUI subsystem gets initialized before the language system.
        // Edit: Nevermind, I switched back to txt_load() because apparently assigning a str_createw()-returned STRING to
        // a str_create()'d one causes some problems which I don't have time to fix yet.
        if(!__GUIState_singleton->PC_string_table)
        {
            __GUIState_singleton->PC_string_table = txt_create(64, LAYER_DEBUG_1);

            const char *d = _chr(delimit_str);
            str_cpy(delimit_str, "∠"); // If My Heart Had Wings
            txt_load(__GUIState_singleton->PC_string_table, "./etc/pc_string_table.lstr");
            str_cpy(delimit_str, d);
        }

        __GUIState_singleton->PC_icon_shutdown       = gui_button_new(
            pair_new(__GUIState_singleton->PC_wallpaper->pos_x + 10.0, __GUIState_singleton->PC_wallpaper->pos_y + 54.0),
            "Shut Down",
            LAYER_GUI_4,
            "PC_shutdown.png",
            "PC_shutdown.png",
            "PC_shutdown.png",
            __shutdown_click,
            NULL,
            NULL);
        gui_button_set_margin(__GUIState_singleton->PC_icon_shutdown, -28.0, 25.0);

        __GUIState_singleton->PC_icon_skyde          = gui_button_new(
            pair_new(__GUIState_singleton->PC_wallpaper->pos_x + 10.0, __GUIState_singleton->PC_wallpaper->pos_y + 132.0),
            "Messaging",
            LAYER_GUI_4,
            "PC_bubble.png",
            "PC_bubble.png",
            "PC_bubble.png",
            __skyde_click,
            NULL,
            NULL);
        gui_button_set_margin(__GUIState_singleton->PC_icon_skyde, -30.0, 25.0);

        __GUIState_singleton->PC_icon_news            = gui_button_new(
            pair_new(__GUIState_singleton->PC_wallpaper->pos_x + 10.0, __GUIState_singleton->PC_wallpaper->pos_y + 200.0),
            "News",
            LAYER_GUI_4,
            "PC_mail.png",
            "PC_mail.png",
            "PC_mail.png",
            __news_click,
            NULL,
            NULL);
        gui_button_set_margin(__GUIState_singleton->PC_icon_news, -22.0, 25.0);

        __GUIState_singleton->PC_icon_skyde_login      = gui_button_new(
            pair_new( (__GUIState_singleton->PC_window->pos_x + bmap_width(__GUIState_singleton->PC_window->bmap)) / 2, (__GUIState_singleton->PC_window->pos_y + bmap_height(__GUIState_singleton->PC_window->bmap)) - 150.0 ),
            "Login",
            LAYER_GUI_7,
            "button_skyde_on.jpg",
            "button_skyde_off.jpg",
            "button_skyde_on.jpg",
            __skyde_sign_in,
            NULL,
            NULL);
        gui_button_set_margin(__GUIState_singleton->PC_icon_skyde_login, -25.0, -7.0);

        __GUIState_singleton->PC_icon_skyde_exit        = gui_button_new(
            pair_new(__GUIState_singleton->PC_icon_skyde_login->position->first, __GUIState_singleton->PC_icon_skyde_login->position->second + 42.0),
            "Exit",
            LAYER_GUI_7,
            "button_skyde_on.jpg",
            "button_skyde_off.jpg",
            "button_skyde_on.jpg",
            __close_window,
            NULL,
            NULL);
        gui_button_set_margin(__GUIState_singleton->PC_icon_skyde_exit, -20.0, -5.0);

        __GUIState_singleton->PC_close_button       = pan_create(NULL, LAYER_GUI_7);
        __GUIState_singleton->PC_close_button->bmap = bmap_create(game_asset_get_gui("PC_close_handler.jpg"));
        pan_setbutton(__GUIState_singleton->PC_close_button, 0, 0, 0, 0,
            bmap_create(game_asset_get_gui("PC_close_on.jpg")),
            bmap_create(game_asset_get_gui("PC_close_off.jpg")),
            bmap_create(game_asset_get_gui("PC_close_off.jpg")),
            bmap_create(game_asset_get_gui("PC_close_off.jpg")),
            __close_window,
            NULL,
            NULL);

        __GUIState_singleton->PC_cursor             = bmap_create(game_asset_get_gui("PC_cursor.png"));

        __GUIState_singleton->PC_taskbar            = pan_create(NULL, LAYER_GUI_4);
        __GUIState_singleton->PC_taskbar->bmap      = bmap_create(game_asset_get_gui("PC_Taskbar.jpg"));
        gui_panel_set_pos(__GUIState_singleton->PC_taskbar, __GUIState_singleton->PC_wallpaper->pos_x, __GUIState_singleton->PC_wallpaper->pos_y + bmap_height(__GUIState_singleton->PC_wallpaper->bmap) - bmap_height(__GUIState_singleton->PC_taskbar->bmap));

        __GUIState_singleton->PC_skyde_contacts       = pan_create(NULL, LAYER_GUI_7);
        __GUIState_singleton->PC_skyde_contacts->bmap = bmap_create(game_asset_get_gui("PC_contacts.jpg"));
        __GUIState_singleton->PC_skyde_contacts->flags |= (OVERLAY);
        gui_panel_set_pos(__GUIState_singleton->PC_skyde_contacts, __GUIState_singleton->PC_wallpaper->pos_x + 15.0 , __GUIState_singleton->PC_wallpaper->pos_y + 150.0);

        #define    __CREATE_GUI_SKYDE_CONTACTS(x, y, on, off, over) pan_setbutton(__GUIState_singleton->PC_skyde_contacts, 0, 0, ##x##, ##y##, bmap_create(game_asset_get_gui(##on##)), bmap_create(game_asset_get_gui(##off##)), bmap_create(game_asset_get_gui(##over##)), bmap_create(game_asset_get_gui(##off##)), __contacts_click, NULL, NULL)

        __CREATE_GUI_SKYDE_CONTACTS(0.0, 42.0, "PC_skyde_contact1.bmp", "PC_skyde_contact1.bmp", "PC_skyde_contact1.bmp");
        __CREATE_GUI_SKYDE_CONTACTS(0.0, 84.0, "PC_skyde_contact2.bmp", "PC_skyde_contact2.bmp", "PC_skyde_contact2.bmp");
        __CREATE_GUI_SKYDE_CONTACTS(0.0, 126.0, "PC_skyde_contact3.bmp", "PC_skyde_contact3.bmp", "PC_skyde_contact3.bmp");
        __CREATE_GUI_SKYDE_CONTACTS(0.0, 168.0, "PC_skyde_contact4.bmp", "PC_skyde_contact4.bmp", "PC_skyde_contact4.bmp");

        __GUIState_singleton->PC_skyde_messagebox       = pan_create(NULL, LAYER_GUI_7);
        __GUIState_singleton->PC_skyde_messagebox->bmap = bmap_create(game_asset_get_gui("PC_messages.jpg"));
        __GUIState_singleton->PC_skyde_messagebox->flags |= (OVERLAY);
        gui_panel_set_pos(__GUIState_singleton->PC_skyde_messagebox, __GUIState_singleton->PC_skyde_contacts->pos_x + 256.0 , __GUIState_singleton->PC_skyde_contacts->pos_y);

        __GUIState_singleton->PC_icon_skyde_send        = gui_button_new(
            pair_new(__GUIState_singleton->PC_wallpaper->pos_x + 804.0, __GUIState_singleton->PC_wallpaper->pos_y + 618.0),
            "Send",
            LAYER_GUI_8,
            "button_skyde_send_on.png",
            "button_skyde_send_off.png",
            "button_skyde_send_on.png",
            __skyde_send_message,
            NULL,
            NULL);
        gui_button_set_margin(__GUIState_singleton->PC_icon_skyde_send, -10.0, -10.0);

        // Message blobs.
        __GUIState_singleton->PC_icon_skyde_blob1        = gui_button_new(
            pair_new(__GUIState_singleton->PC_skyde_messagebox->pos_x, __GUIState_singleton->PC_skyde_messagebox->pos_y + 100.0),
            "[Message no. #8992fc4d sent]",
            LAYER_GUI_8,
            "PC_skyde_msgblob.bmp",
            "PC_skyde_msgblob.bmp",
            "PC_skyde_msgblob.bmp",
            NULL,
            NULL,
            NULL);
        gui_button_set_margin(__GUIState_singleton->PC_icon_skyde_blob1, -6.0, -6.0);

        #define    __CREATE_GUI_BUTTON_NEWS(x, y, str, on, off, over) gui_button_new(pair_new(##x##, ##y##), ##str##, LAYER_GUI_7, game_asset_get_gui(##on##), game_asset_get_gui(##off##), game_asset_get_gui(##over##), NULL, NULL, NULL )

        float base_point_x = __GUIState_singleton->PC_wallpaper->pos_x + 30.0;
        float base_point_y = __GUIState_singleton->PC_wallpaper->pos_y;

        (__GUIState_singleton->PC_icon_news_provider)[0] = __CREATE_GUI_BUTTON_NEWS(base_point_x, base_point_y + 50.0, "This is my string", "button_news.jpg", "button_news.jpg", "button_news.jpg"); // We've had more than enough long ass block-like function calls.
        (__GUIState_singleton->PC_icon_news_provider)[1] = __CREATE_GUI_BUTTON_NEWS(base_point_x, base_point_y + 100.0, "This is my string", "button_news.jpg", "button_news.jpg", "button_news.jpg");

        // Now that we've done with setting up GUI elements, let's deal with texts.
        __GUIState_singleton->PC_string_skyde_log_in                   = txt_create(1, LAYER_GUI_7);
        __GUIState_singleton->PC_string_skyde_log_in->font             = Loading_Font;
        (__GUIState_singleton->PC_string_skyde_log_in->pstring)[0]     = (__GUIState_singleton->PC_string_table->pstring)[6];

        vec_set(&(__GUIState_singleton->PC_string_skyde_log_in->blue), COLOR_BLACK);
        gui_text_set_pos(__GUIState_singleton->PC_string_skyde_log_in,
            __GUIState_singleton->PC_window->pos_x + 100.0,
            __GUIState_singleton->PC_window->pos_y + 100.0);

        __GUIState_singleton->PC_string_skyde_chat_with                 = txt_create(1, LAYER_GUI_8);
        __GUIState_singleton->PC_string_skyde_chat_with->font           = Loading_Font;
        (__GUIState_singleton->PC_string_skyde_chat_with->pstring)[0]   = str_create("Last seen: 121 days ago");

        vec_set(&(__GUIState_singleton->PC_string_skyde_chat_with->blue), COLOR_BLACK);
        gui_text_set_pos(__GUIState_singleton->PC_string_skyde_chat_with,
            __GUIState_singleton->PC_skyde_messagebox->pos_x + 35.0,
            __GUIState_singleton->PC_skyde_messagebox->pos_y + 60.0);

        __GUIState_singleton->PC_string_news                             = txt_create(1, LAYER_GUI_7);
        __GUIState_singleton->PC_string_news->font                       = Loading_Font;
        (__GUIState_singleton->PC_string_news->pstring)[0]               = str_create("123456");
        vec_set(&(__GUIState_singleton->PC_string_news->blue), COLOR_BLACK);
        gui_text_set_pos(__GUIState_singleton->PC_string_news,
            __GUIState_singleton->PC_wallpaper->pos_x + 25.0,
            __GUIState_singleton->PC_wallpaper->pos_y + 50.0);

    } /* End of STATE_PC. */

    __GUI_done__ = 1;
}

/*
 * void game_gui_state_free()
 *
 * Frees the GUI state singleton.
 */
void game_gui_state_free()
{
    if( __GUIState_singleton )
    {
        safe_remove(__GUIState_singleton->reticule);
        safe_remove(__GUIState_singleton->paper_texture);

        /* STATE_PC */ {
            txt_remove_ex(__GUIState_singleton->PC_string_table);
            txt_remove_ex(__GUIState_singleton->PC_string_skyde_chat_with);
            txt_remove(__GUIState_singleton->PC_string_skyde_log_in); // Because the string is already erased when PC_string_table gets wiped
                                                                      // (with the call to txt_remove_ex() below),
                                                                      // we don't need to erase it again (overkill - still dominating an already dominated enemy?)

            gui_button_free(__GUIState_singleton->PC_icon_shutdown);
            gui_button_free(__GUIState_singleton->PC_icon_skyde);
            gui_button_free(__GUIState_singleton->PC_icon_news);
            gui_button_free(__GUIState_singleton->PC_icon_skyde_exit);
            gui_button_free(__GUIState_singleton->PC_icon_skyde_login);
            gui_button_free(__GUIState_singleton->PC_icon_skyde_send);
            gui_button_free(__GUIState_singleton->PC_icon_skyde_blob1);

            int i = 0;
            for(; i < 4; i++)
                if(NULL != (__GUIState_singleton->PC_icon_news_provider)[i])
                    gui_button_free( (__GUIState_singleton->PC_icon_news_provider)[i] );

            safe_remove(__GUIState_singleton->PC_wallpaper);
            safe_remove(__GUIState_singleton->PC_window);
            safe_remove(__GUIState_singleton->PC_close_button);
            safe_remove(__GUIState_singleton->PC_taskbar);
            safe_remove(__GUIState_singleton->PC_skyde_contacts);
            safe_remove(__GUIState_singleton->PC_skyde_messagebox);
            bmap_remove(__GUIState_singleton->PC_cursor);

            if(__GUIState_singleton->PC_cursor_original)
            {
                mouse_map = bmap_create(__GUIState_singleton->PC_cursor_original);
                bmap_remove(__GUIState_singleton->PC_cursor_original);
            }

        }

        FREE(__GUIState_singleton);
    }
}

void game_gui_set_reticule( Bitmap *reticule )
{
    if(reticule && __GUIState_singleton)
    {
        __GUIState_singleton->reticule->bmap = reticule;
        float px = (screen_size.x - bmap_width(__GUIState_singleton->reticule->bmap)) * 0.5;
        float py = (screen_size.y - bmap_height(__GUIState_singleton->reticule->bmap)) * 0.5;
        gui_panel_set_pos( __GUIState_singleton->reticule, px, py );
    }
}

void game_gui_set_paper_texture( Bitmap *texture )
{
    if(texture && __GUIState_singleton )
    {
        __GUIState_singleton->paper_texture->bmap = texture;

        float px = (screen_size.x - bmap_width(texture)) * 0.5;
        float py = 45.0; // Thụt lề, thụt hoài, thụt thụt hoàiiiiii. Kí tên: Lợn khó tính
        gui_panel_set_pos( __GUIState_singleton->paper_texture, px, py );
    }
}

void game_gui_set_paper_texture( String *texture_gstr )
{
    if(texture_gstr)
        game_gui_set_paper_texture(bmap_create(texture_gstr));
}

void game_gui_set_reticule( String *reticule_gstr )
{
    if(reticule_gstr)
        game_gui_set_reticule(bmap_create(reticule_gstr));
}

Bitmap *game_gui_get_paper_texture()
{
    if(__GUIState_singleton)
        return __GUIState_singleton->paper_texture->bmap;
}

Bitmap *game_gui_get_reticule()
{
    if(__GUIState_singleton)
        return __GUIState_singleton->reticule->bmap;
}

__static void __GUIState_show_main_menu()
{
    SHOW_FLAGS_SAFE(__GUIState_singleton->menu_options, SHOW);
    SHOW_FLAGS_SAFE(__GUIState_singleton->menu_options_3, SHOW);
    SHOW_FLAGS_SAFE(__GUIState_singleton->menu_options_2, SHOW);
    SHOW_FLAGS_SAFE(__GUIState_singleton->menu_logo, SHOW);
    SHOW_FLAGS_SAFE(__GUIState_singleton->menu_options_digit, SHOW);

    mouse_map   = __GUIState_singleton->menu_cursor;
    mouse_mode  = 4;
    player_lock = 1;
}

__static void __GUIState_hide_main_menu()
{
    HIDE_FLAGS_SAFE(__GUIState_singleton->menu_options, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->menu_options_3, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->menu_options_2, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->menu_logo, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->menu_options_digit, SHOW);

    mouse_mode = 0;
    player_lock = 0;

    if(__GameState_singleton)
        __GameState_singleton->menu_switch = 0;
}

__static void __GUIState_show_PC()
{
    if( __can_press_esc )
        __can_press_esc = 0;

    //if(!snd_playing(sndPCBootupHandle))
    //    sndPCBootupHandle = snd_play(sndPCBootup, game_volume, 0.0);
    snd_play(sndPCBootup, game_volume, 0.0);

    /*
    if(camera) // Also means that a scene is loaded.
        ppViewCreateStaged(camera, matCrt);
    */

    __GUIState_singleton->PC_wallpaper->bmap = __GUIState_singleton->PC_wallpaper_wallpaper;

    SHOW_FLAGS_SAFE(__GUIState_singleton->generic_black, SHOW);
    SHOW_FLAGS_SAFE(__GUIState_singleton->PC_wallpaper, SHOW);
    SHOW_FLAGS_SAFE(__GUIState_singleton->PC_taskbar, SHOW);
    gui_button_show(__GUIState_singleton->PC_icon_shutdown);
    gui_button_show(__GUIState_singleton->PC_icon_skyde);
    gui_button_show(__GUIState_singleton->PC_icon_news);

    __GUIState_singleton->PC_cursor_original = mouse_map;
    mouse_map   = __GUIState_singleton->PC_cursor;
    mouse_mode  = 4;
}

__static void __GUIState_hide_PC()
{
    if( !__can_press_esc )
        __can_press_esc = 1;

    //if(snd_playing(sndPCBootupHandle))
    //    snd_stop(sndPCBootupHandle);

    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_window, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_string_skyde_log_in, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_skyde_contacts, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_skyde_messagebox, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_news_list, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_string_skyde_chat_with, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_string_news, SHOW);

    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_wallpaper, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->PC_taskbar, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->generic_black, SHOW);
    gui_button_hide(__GUIState_singleton->PC_icon_shutdown);
    gui_button_hide(__GUIState_singleton->PC_icon_skyde);
    gui_button_hide(__GUIState_singleton->PC_icon_news);

    mouse_mode = 0;
    __camera_locked = 0;
    view_set_custom(true);
}

__static void __GUIState_show_game_menu()
{
    __GUIState_show_main_menu();
}

__static void __GUIState_hide_game_menu()
{
    __GUIState_hide_main_menu();
}

__static void __GUIState_show_intro()
{
    SHOW_FLAGS_SAFE(__GUIState_singleton->intro_screen, SHOW);

    SHOW_FLAGS_SAFE(__GUIState_singleton->intro_text, SHOW);
    while(__GUIState_singleton->intro_text->alpha < 100.0)
    {
        __GUIState_singleton->intro_text->alpha += 4.2 * time_step;
        wait(1.0);
    }

    while( !key_enter )
        wait(1.0);

    HIDE_FLAGS_SAFE(__GUIState_singleton->intro_screen, SHOW);
    while(__GUIState_singleton->intro_text->alpha > 0.0)
    {
        __GUIState_singleton->intro_text->alpha -= 4.2 * time_step;
        wait(1.0);
    }
    HIDE_FLAGS_SAFE(__GUIState_singleton->intro_text, SHOW);

    game_gui_set_state(STATE_MAIN_MENU);
    game_gui_render();

    game_mplayer_play("main");
}

__static void __GUIState_hide_intro()
{
    HIDE_FLAGS_SAFE(__GUIState_singleton->intro_screen, SHOW);
    HIDE_FLAGS_SAFE(__GUIState_singleton->intro_text, SHOW);
}

__static void __GUIState_hide_ending()
{
    return;
}

#define     __ENDING_TEXT_FADE(id, time, speed)    __GUIState_singleton->ending_text->flags |= SHOW;  __GUIState_singleton->ending_text->bmap  = __GUIState_singleton->ending_text_##id##; gui_panel_resize(__GUIState_singleton->ending_text); __GUIState_singleton->ending_text->alpha = 0.0; gui_panel_set_pos( __GUIState_singleton->ending_text, b##id##x, b##id##y ); while( __GUIState_singleton->ending_text->alpha < 100.0 ) { __GUIState_singleton->ending_text->alpha += ##speed## * time_step; wait(1.0); timer = .0; } while( timer < ##time## ) { timer += time_step / 16.0; wait(1.0); } while( __GUIState_singleton->ending_text->alpha > 0.0 ) { __GUIState_singleton->ending_text->alpha -= ##speed## * time_step; wait(1.0); } __GUIState_singleton->ending_text->flags &= ~(SHOW)

__static void __GUIState_show_ending()
{
    /*
    str_printf( (__GUIState_singleton->ending_secrets_found->pstring)[0], "%i out of %i secrets found.", (int) __found_secrets, (int) __max_secrets );
    __GUIState_singleton->ending_secrets_found->pos_x = (screen_size.x - str_width((__GUIState_singleton->ending_secrets_found->pstring)[0], __GUIState_singleton->ending_secrets_found->font)) / 2.0;
    __GUIState_singleton->ending_secrets_found->pos_y = screen_size.y / 2.0;
    */

    game_mplayer_play( "end-01" );

    float   b1x = (screen_size.x - bmap_width(__GUIState_singleton->ending_text_1)) / 2.0,
            b1y = (screen_size.y - bmap_height(__GUIState_singleton->ending_text_1)) / 2.0,
            b2x = (screen_size.x - bmap_width(__GUIState_singleton->ending_text_2)) / 2.0,
            b2y = (screen_size.y - bmap_height(__GUIState_singleton->ending_text_2)) / 2.0,
            b3x = (screen_size.x - bmap_width(__GUIState_singleton->ending_text_3)) / 2.0,
            b3y = (screen_size.y - bmap_height(__GUIState_singleton->ending_text_3)) / 2.0,
            b0x = (screen_size.x - bmap_width(__GUIState_singleton->ending_text_0)) / 2.0,
            b0y = (screen_size.y - bmap_height(__GUIState_singleton->ending_text_0)) / 2.0;

    float timer = .0;

    SHOW_FLAGS_SAFE( __GUIState_singleton->ending_background, SHOW );

    __ENDING_TEXT_FADE(1, 2.0, 4.2);
    __ENDING_TEXT_FADE(2, 2.0, 4.2);
    __ENDING_TEXT_FADE(3, 2.0, 4.2);
    __ENDING_TEXT_FADE(0, 2.0, 4.2);

    /*
    __GUIState_singleton->ending_secrets_found->flags |= SHOW;
    __GUIState_singleton->ending_secrets_found->alpha  = 0.0;

    while( __GUIState_singleton->ending_secrets_found->alpha < 100.0 )
    {
        __GUIState_singleton->ending_secrets_found->alpha += 4.2 * time_step;
        wait(1.0);
    }

    if( __found_secrets >= __max_secrets )
    {
        str_printf( (__GUIState_singleton->ending_secrets_found->pstring)[0], "Password for \"%s\": %s", _chr(STR_CODE_FILE), _chr(STR_CODE) );
        __GUIState_singleton->ending_secrets_found->pos_x = (screen_size.x - str_width((__GUIState_singleton->ending_secrets_found->pstring)[0], __GUIState_singleton->ending_secrets_found->font)) / 2.0;
        __GUIState_singleton->ending_secrets_found->pos_y = screen_size.y / 2.0;
    }
    */

    SHOW_FLAGS_SAFE( __GUIState_singleton->ending_exit, SHOW );
    while( __GUIState_singleton->ending_exit->alpha < 100.0 )
    {
        __GUIState_singleton->ending_exit->alpha += 4.2 * time_step;
        wait(1.0);
    }

    while( !key_enter )
        wait(1.0);

    wait(4.2);
    __game_event_on_close();
}

void game_gui_render()
{
    if(__GUIState_singleton)
    {
        switch(__GUIState_singleton->state)
        {
            case STATE_MAIN_MENU: {
                __GUIState_hide_PC();
                __GUIState_hide_intro();
                __GUIState_hide_ending();

                __GUIState_show_main_menu();
                break;
            }

            case STATE_GAME_MENU: {
                __GUIState_hide_PC();
                __GUIState_hide_intro();
                __GUIState_hide_ending();

                __GUIState_show_game_menu();
                break;
            }

            case STATE_PC: {
                __GUIState_hide_main_menu();
                __GUIState_hide_intro();
                __GUIState_hide_ending();

                __GUIState_show_PC();
                break;
            }

            case STATE_INTRO: {
                __GUIState_hide_main_menu();
                __GUIState_hide_PC();
                __GUIState_hide_ending();

                __GUIState_show_intro();
                break;
            }

            case STATE_ENDING: {
                __GUIState_hide_main_menu();
                __GUIState_hide_PC();
                __GUIState_hide_intro();

                __GUIState_show_ending();
                break;
            }

            case STATE_NULL: {
                __GUIState_hide_main_menu();
                __GUIState_hide_intro();
                __GUIState_hide_PC();
                __GUIState_hide_ending();

                __GUIState_singleton->reticule->flags |= (SHOW);

                break;
            }
        }
    }
}

void game_gui_update()
{
    while(__GUI_done__ == 0)
        wait(1.0);

    int state = game_gui_get_state();

    switch(state) {

        case STATE_PC : // Updating the GUI while we're using the game's "portable" PC.
        {
            if(GET_FLAGS_SAFE(__GUIState_singleton->PC_window, SHOW) && __GUIState_singleton->PC_window->bmap)
            {
                SHOW_FLAGS_SAFE(__GUIState_singleton->PC_close_button, SHOW);
                __GUIState_singleton->PC_close_button->pos_x = __GUIState_singleton->PC_window->pos_x + bmap_width(__GUIState_singleton->PC_window->bmap) - 24.0;
                __GUIState_singleton->PC_close_button->pos_y = __GUIState_singleton->PC_window->pos_y + 14.2;
            }
            else
                HIDE_FLAGS_SAFE(__GUIState_singleton->PC_close_button, SHOW);
        }

        default:
            break;
    }

    if(!var_cmp(mouse_mode, 0)) // Because mouse_mode is a var.
        vec_set( &mouse_pos, &mouse_cursor );
}

void game_gui_set_state( const int state )
{
    if(__GUIState_singleton)
        __GUIState_singleton->state = (int) ifelse(state > 0, state, STATE_MAIN_MENU);
}

int game_gui_get_state()
{
    if(__GUIState_singleton)
        return __GUIState_singleton->state;
}
