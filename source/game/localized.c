/* localized.c */
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
 * <localized>
 * Header containing the string table of the game.
 * Strings are initialized based on the chosen language. Therefore,
 * you have to make a call to localized_init(), passing the chosen language
 * to the parameter after a language was picked.
 * Dependent of <region>.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */

 /*
  * void localized_init()
  *
  * Initializes the game's strings based on the chosen language.
  * If you change the current language, for example, through a call to region_set_language(),
  * call this function once again to perform re-initialization.
  */
void    localized_init()
{
    #ifndef    UI_LESS
        REINIT_STRING(lstr_open_door, "open_door.lstr");
        REINIT_STRING(lstr_close_door, "close_door.lstr");
        REINIT_STRING(lstr_interact, "interact.lstr");
        REINIT_STRING(lstr_interact_sleep, "interact_sleep.lstr");
        REINIT_STRING(lstr_read_note, "read_note.lstr");
        REINIT_STRING(lstr_room_1stbedroom, "room_1stbedroom.lstr");
        REINIT_STRING(lstr_room_2ndbedroom, "room_2ndbedroom.lstr");
        REINIT_STRING(lstr_room_1sthallway, "room_1sthallway.lstr");
        REINIT_STRING(lstr_room_2ndhallway, "room_2ndhallway.lstr");
        REINIT_STRING(lstr_rooom_kitchen, "room_kitchen.lstr");
        REINIT_STRING(lstr_room_livingroom, "room_living.lstr");
        REINIT_STRING(lstr_room_bathroom, "room_bathroom.lstr");
        REINIT_STRING(lstr_room_yard, "room_yard.lstr");
        REINIT_STRING(lstr_room_balcony, "room_balcony.lstr");
        REINIT_STRING(lstr_room_workspace, "room_workspace.lstr");
        REINIT_STRING(lstr_intro_c1, "c1-prologue.lstr");
        REINIT_STRING(lstr_intro_c3, "c3-prologue.lstr");
        REINIT_STRING(lstr_not_done_jobs, "objective_failed.lstr");
        REINIT_STRING(lstr_options_1, "options-1.lstr");
        REINIT_STRING(lstr_options_2, "options-2.lstr");
        REINIT_STRING(lstr_options_3, "options-3.lstr");
        REINIT_STRING(lstr_beta_warning, "beta-warning.lstr");
        REINIT_STRING(lstr_induction_c1, "c1-prologue-ind.lstr");
        REINIT_STRING(lstr_induction_c2, "c2-prologue-ind.lstr");
        REINIT_STRING(lstr_induction_c3, "c3-prologue-ind.lstr");
        REINIT_STRING(lstr_induction_c4, "c4-prologue-ind.lstr");
        REINIT_STRING(lstr_induction_c5, "c5-prologue-ind.lstr");
        REINIT_STRING(lstr_induction_c6, "c6-prologue-ind.lstr");
        REINIT_STRING(lstr_todo_c1, "c1-todo.lstr");
        REINIT_STRING(lstr_todo_c2, "c2-todo.lstr");
        REINIT_STRING(lstr_todo_c3, "c3-todo.lstr");
        REINIT_STRING(lstr_todo_c6, "c6-todo.lstr");
    #endif
}

void    localized_free()
{
    #ifndef    UI_LESS
        safe_remove(lstr_open_door);
        safe_remove(lstr_close_door);
        safe_remove(lstr_interact);
        safe_remove(lstr_interact_sleep);
        safe_remove(lstr_read_note);
        safe_remove(lstr_room_1stbedroom);
        safe_remove(lstr_room_2ndbedroom);
        safe_remove(lstr_room_1sthallway);
        safe_remove(lstr_room_2ndhallway);
        safe_remove(lstr_rooom_kitchen);
        safe_remove(lstr_room_livingroom);
        safe_remove(lstr_room_bathroom);
        safe_remove(lstr_room_yard);
        safe_remove(lstr_room_balcony);
        safe_remove(lstr_room_workspace);
        safe_remove(lstr_intro_c1);
        safe_remove(lstr_intro_c3);
        safe_remove(lstr_not_done_jobs);
        safe_remove(lstr_options_1);
        safe_remove(lstr_options_2);
        safe_remove(lstr_options_3);
        safe_remove(lstr_beta_warning);
        safe_remove(lstr_induction_c1);
        safe_remove(lstr_induction_c2);
        safe_remove(lstr_induction_c3);
        safe_remove(lstr_induction_c4);
        safe_remove(lstr_induction_c5);
        safe_remove(lstr_induction_c6);
        safe_remove(lstr_todo_c1);
        safe_remove(lstr_todo_c2);
        safe_remove(lstr_todo_c3);
        safe_remove(lstr_todo_c1);
    #endif
}
