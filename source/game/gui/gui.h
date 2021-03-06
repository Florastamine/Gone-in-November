/* gui.h */
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
 *
 * __________________________________________________________________
 *
 * <gui>
 * Downpour's GUI code.
 * Dependent of the library <gui_utilities>.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    _GUI_H_
#define    _GUI_H_

#define PRAGMA_PRINT "\nGUI initialization..."

#include "gui_utilities.h"

#define __static
#define __In
#define __Out

/*
 * GUI states.
 */
#define    STATE_MAIN_MENU    1
#define    STATE_GAME_MENU    2
#define    STATE_INTRO        3
#define    STATE_PC           4
#define    STATE_ENDING       5
#define    STATE_NULL         6

/*
 * GUIState (struct)
 *
 * This struct contains everything related to the game's GUI: main menu, reticule,
 * message boxes,... everything.
 */
typedef struct {
    int state;

    /* STATE_NULL */
        Panel *reticule;
        Panel *paper_texture;
        Panel *todo_texture;
        Panel *interact_icon;
    /* End of STATE_NULL. */

    Panel *generic_black;

    /* STATE_MAIN_MENU */
        Panel     *menu_options;
        Panel     *menu_keymap;
        Panel     *menu_logo;
        Text      *menu_options_2;
        Text      *menu_options_3;
        Text      *menu_options_digit;
        Bitmap    *menu_cursor;

    /* STATE_ENDING */
        Panel     *ending_background;
        Panel     *ending_text;

        Bitmap    *ending_text_0;
        Bitmap    *ending_text_1;
        Bitmap    *ending_text_2;
        Bitmap    *ending_text_3;
        Text      *ending_secrets_found;
        Text      *ending_exit;
    /* End of STATE_ENDING. */

    /* STATE_INTRO */
        Panel *intro_screen;
        Text  *intro_text;
    /* End of STATE_INTRO. */

    /* STATE_PC */
        Panel   *PC_wallpaper;
        Panel   *PC_window; // Sk*de
        Panel   *PC_close_button;
        Panel   *PC_taskbar;
        Panel   *PC_skyde_contacts;
        Panel   *PC_skyde_messagebox;
        Panel   *PC_skyde_chatbox;
        Panel   *PC_news_list;

        Bitmap  *PC_wallpaper_wallpaper;
        Bitmap  *PC_wallpaper_logoff;
        Bitmap  *PC_wallpaper_boot;

        Bitmap  *PC_cursor;
        Bitmap  *PC_cursor_original;

        GUIButton *PC_icon_shutdown;
        GUIButton *PC_icon_skyde;
        GUIButton *PC_icon_news;
        GUIButton *PC_icon_skyde_login;
        GUIButton *PC_icon_skyde_exit;
        GUIButton *PC_icon_skyde_send;
        GUIButton *PC_icon_skyde_blob1;
        GUIButton *PC_icon_news;
        GUIButton *PC_icon_news_provider[4];

        Text *PC_string_table;
        Text *PC_string_skyde_log_in;
        Text *PC_string_skyde_chat_with;
        Text *PC_string_news;
        Text *PC_skyde_chatbox_input;
    /* End of STATE_PC. */

} GUIState;

GUIState *__GUIState_singleton = NULL;

__static int __GUI_done__ = 0;

__static    var fov            = 75.0,
                volume         = 100.0,
                sensivity_mul  = 1.0;
/*
 * void game_gui_state_new()
 *
 * Allocates and initializes a new GUI state.
 * After this call, the GUI can't still be rendered as you have to set up various parameters for the state singleton
 * (behaviors and bitmaps) with various game_gui_set_*() calls.
 * Finally, to render the GUI (taking states into account), call game_gui_render().
 * State management is done through game_gui_set_state() and game_gui_get_state().
 */
void game_gui_state_new();

/*
 * void game_gui_state_free()
 *
 * Frees the GUI state singleton.
 */
void game_gui_state_free();

void game_gui_set_state( __In const int state );

int game_gui_get_state();

void game_gui_set_reticule( __In Bitmap *reticule );
void game_gui_set_reticule( __In String *reticule_gstr );

void game_gui_set_paper_texture( __In Bitmap *texture );
void game_gui_set_paper_texture( __In String *texture_gstr );

Bitmap *game_gui_get_reticule();
Bitmap *game_gui_get_paper_texture();

void game_gui_render();
void game_gui_hide();

void game_gui_update();

#include "gui.c"
#endif /* gui.h */
