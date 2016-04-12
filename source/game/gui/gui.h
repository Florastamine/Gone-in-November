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
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
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
#define    STATE_NULL         5

/*
 * GUIState (struct)
 *
 * This struct contains everything related to the game's GUI: main menu, reticule,
 * message boxes,... everything.
 */
typedef struct {
    int state;

    Panel *reticule;
    Panel *paper_texture;

    /* GUI_INTRO */
        Panel *intro_lang_screen;
        GUIButton *intro_lang_vn_button;
        GUIButton *intro_lang_en_button;
    /* End of GUI_INTRO. */

    Panel   *PC_boot_screen;
    Panel   *PC_wallpaper;
    Bitmap  *PC_cursor;
    Bitmap  *PC_cursor_original;

} GUIState;

GUIState *__GUIState_singleton = NULL;

__static int __GUI_done__ = 0;

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
