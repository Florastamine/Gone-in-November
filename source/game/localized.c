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
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
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
    #endif
}

void    localized_free()
{
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
}
