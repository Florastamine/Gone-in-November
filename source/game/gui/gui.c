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

    __GUIState_singleton->reticule->flags    = __GUIState_singleton->reticule->flags | (OVERLAY);
    gui_panel_set_pos( __GUIState_singleton->reticule, 0.0, 0.0 );

    /* STATE_INTRO */ {
        #define    MARGIN    142.0

        VECTOR xx, xy;
        vec_zero(xx);
        vec_zero(xy);

        xx.x = MARGIN;
        xx.y = xy.y = screen_size.y * 0.5;
        xy.x = screen_size.x - MARGIN;

        __GUIState_singleton->intro_lang_vn_button = gui_button_new(
            pair_new(xx.x, xx.y),
            "Vietnamese",
            LAYER_GUI_2,
            game_asset_get_gui("flag-vietnam.png"),
            game_asset_get_gui("flag-vietnam.png"),
            game_asset_get_gui("flag-vietnam.png"),
            NULL,
            NULL,
            NULL
        );

        __GUIState_singleton->intro_lang_en_button = gui_button_new(
            pair_new(xy.x, xy.y),
            "English",
            LAYER_GUI_2,
            game_asset_get_gui("flag-united_kingdom_great_britainpng"),
            game_asset_get_gui("flag-united_kingdom_great_britain.png"),
            game_asset_get_gui("flag-united_kingdom_great_britain.png"),
            NULL,
            NULL,
            NULL
        );

        gui_button_set_font(__GUIState_singleton->intro_lang_vn_button, Loading_Font);
        gui_button_set_font(__GUIState_singleton->intro_lang_en_button, Loading_Font);

        __GUIState_singleton->intro_lang_screen        = pan_create(NULL, LAYER_GUI_1);
        __GUIState_singleton->intro_lang_screen->bmap  = bmap_createblack(screen_size.x, screen_size.y, 8);

        /* End of STATE_INTRO. */

        __GUIState_singleton->PC_boot_screen        = pan_create(NULL, LAYER_GUI_4);
        __GUIState_singleton->PC_boot_screen->bmap  = bmap_create(game_asset_get_gui("PC_boot.jpg"));
        gui_panel_set_center(__GUIState_singleton->PC_boot_screen, CENTER_X | CENTER_Y);

        __GUIState_singleton->PC_wallpaper          = pan_create(NULL, LAYER_GUI_3);
        __GUIState_singleton->PC_wallpaper->bmap    = bmap_create(game_asset_get_gui("PC_wallpaper.jpg"));
        gui_panel_set_center(__GUIState_singleton->PC_wallpaper, CENTER_X | CENTER_Y);

        __GUIState_singleton->PC_cursor             = bmap_create(game_asset_get_gui("PC_cursor.bmp"));
    }

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

        /* STATE_INTRO */ {
            pan_remove(__GUIState_singleton->intro_lang_screen);
            gui_button_free(__GUIState_singleton->intro_lang_en_button);
            gui_button_free(__GUIState_singleton->intro_lang_vn_button);
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
    return;
}

__static void __GUIState_show_intro()
{
    SHOW_FLAGS_SAFE(__GUIState_singleton->intro_lang_screen, SHOW);

    gui_button_show(__GUIState_singleton->intro_lang_en_button);
    gui_button_show(__GUIState_singleton->intro_lang_vn_button);

    mouse_mode = 4;
}

__static void __GUIState_hide_intro()
{
    HIDE_FLAGS_SAFE(__GUIState_singleton->intro_lang_screen, SHOW);

    gui_button_hide(__GUIState_singleton->intro_lang_en_button);
    gui_button_hide(__GUIState_singleton->intro_lang_vn_button);
}

__static void __GUIState_show_PC()
{
    SHOW_FLAGS_SAFE(__GUIState_singleton->PC_boot_screen, SHOW);
    SHOW_FLAGS_SAFE(__GUIState_singleton->PC_wallpaper, SHOW);

    __GUIState_singleton->PC_cursor_original = mouse_map;
    mouse_map   = __GUIState_singleton->PC_cursor;
    mouse_mode  = 4;
}

__static void __GUIState_show_game_menu()
{
    return;
}

void game_gui_render()
{
    if(__GUIState_singleton)
    {
        #ifdef    DEBUG
            __GUIState_singleton->reticule->flags |= (SHOW);
        #endif

        switch(__GUIState_singleton->state)
        {
            case STATE_MAIN_MENU: {
                __GUIState_hide_intro();
                __GUIState_show_main_menu();

                break;
            }

            case STATE_GAME_MENU: {
                __GUIState_hide_intro();
                __GUIState_show_game_menu();

                break;
            }

            case STATE_PC: {
                __GUIState_hide_intro();
                __GUIState_show_PC();

                break;
            }

            case STATE_INTRO: {
                __GUIState_show_intro();

                break;
            }

            case STATE_NULL: {
                break;
            }
        }
    }
}

void game_gui_update()
{
    while(!__GUI_done__)
        wait(1.0);
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
