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
 * GUIState (struct)
 *
 * This struct contains everything related to the game's GUI: main menu, reticule,
 * message boxes,... everything.
 */
typedef struct {
    Panel *reticule;
} GUIState;

GUIState *__GUIState_singleton = NULL;

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

void game_gui_set_reticule( __In Bitmap *reticule );
void game_gui_set_reticule( __In String *reticule_gstr );

Bitmap *game_gui_get_reticule();

void game_gui_render();

#include "gui.c"
#endif /* gui.h */