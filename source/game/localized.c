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
    #endif
}
