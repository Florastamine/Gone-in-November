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

__static void __game_gui_set_reticule_pos( float x, float y )
{
    __GUIState_singleton->reticule->pos_x = abs(x);
    __GUIState_singleton->reticule->pos_y = abs(y);
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

    // Creates and sets up the reticule.
    __GUIState_singleton->reticule           = pan_create(NULL, LAYER_GUI_1);
    __GUIState_singleton->reticule->flags    = __GUIState_singleton->reticule->flags | (OVERLAY);
    __game_gui_set_reticule_pos(0.0, 0.0);
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
        __game_gui_set_reticule_pos(px, py);
    }
}

void game_gui_set_reticule( String *reticule_gstr )
{
    if(reticule_gstr)
        game_gui_set_reticule(bmap_create(reticule_gstr));
}

Bitmap *game_gui_get_reticule()
{
    if(__GUIState_singleton)
        return __GUIState_singleton->reticule->bmap;
}

void game_gui_render()
{
    if(__GUIState_singleton)
    {
        __GUIState_singleton->reticule->flags |= (SHOW);
    }
}
