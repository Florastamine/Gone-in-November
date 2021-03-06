/* localized.h */
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
#ifndef    __LOCALIZED_H__
#define    __LOCALIZED_H__

#define    REINIT_STRING(STR, FILE) if(NULL != STR) str_remove(STR); STR =  region_get_string(FILE)

/*
 * void localized_init()
 *
 * Initializes the game's strings based on the chosen language.
 * If you change the current language, for example, through a call to region_set_language(),
 * call this function again to perform re-initialization.
 */
void    localized_init();

String *lstr_open_door                       = "";
String *lstr_close_door                      = "";
String *lstr_interact                        = "";
String *lstr_interact_sleep                  = "";
String *lstr_read_note                       = "";
String *lstr_room_1stbedroom                 = "";
String *lstr_room_2ndbedroom                 = "";
String *lstr_room_1sthallway                 = "";
String *lstr_room_2ndhallway                 = "";
String *lstr_rooom_kitchen                   = "";
String *lstr_room_livingroom                 = "";
String *lstr_room_bathroom                   = "";
String *lstr_room_yard                       = "";
String *lstr_room_balcony                    = "";
String *lstr_room_workspace                  = "";

String *lstr_induction_c1                    = "";
String *lstr_induction_c2                    = "";
String *lstr_induction_c3                    = "";
String *lstr_induction_c4                    = "";
String *lstr_induction_c5                    = "";
String *lstr_induction_c6                    = "";
String *lstr_induction_c2                    = "";
String *lstr_intro_c1                        = "";
String *lstr_intro_c3                        = "";
String *lstr_not_done_jobs                   = "";
String *lstr_todo_c1                         = "";
String *lstr_todo_c2                         = "";
String *lstr_todo_c3                         = "";
String *lstr_todo_c6                         = "";

String *lstr_options_1                       = "";
String *lstr_options_2                       = "";
String *lstr_options_3                       = "";

String *lstr_beta_warning                    = "";

#include "localized.c"
#endif /* localized.h */
