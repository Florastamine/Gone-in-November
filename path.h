/* path.h */
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
 * <path>
 * Contains common paths for which game assets and code are searched.
 * In the actual game build (with PERFORM_PATH_LOOKUP disabled), 
 * they are expected to be in the root folder (or in resource packages)
 * instead.
 * 
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    _PATH_H_
#define    _PATH_H_

#define PRAGMA_PRINT "\nResources lookup module initialization..."

#ifdef     PERFORM_PATH_LOOKUP
    #define PRAGMA_PRINT "PERFORM_PATH_LOOKUP was defined. Will be looking for resources in subdirectories..."
    
    #define PRAGMA_PATH "./2d/"
    #define PRAGMA_PATH "./2d/fx/"
    #define PRAGMA_PATH "./2d/gui/"
    #define PRAGMA_PATH "./2d/sprites/"
    
    #define PRAGMA_PATH "./fx/"
    
    #define PRAGMA_PATH "./object/"
    
    #define PRAGMA_PATH "./scene/"
    
    #define PRAGMA_PATH "./sound/"
    #define PRAGMA_PATH "./sound/stream/"
    
    #define PRAGMA_PATH "./translation/"
    
    #define PRAGMA_PATH "./utils/"
    
    // Because we can't name the PRAGMA_PATH definitions - 
    // separate definitions are provided in game_asset_get_*() functions instead.
    // What a waste.
    #define __PATH_LOOKUP_SCENE        "./scene/"
    #define __PATH_LOOKUP_OBJECT       "./object/"
    #define __PATH_LOOKUP_SOUND        "./sound/"
    #define __PATH_LOOKUP_GUI_2D       "./2d/gui/"
    #define __PATH_LOOKUP_SPRITE_2D    "./2d/sprites/"
    #define __PATH_LOOKUP_GENERIC_2D   "./2d/"
    #define __PATH_LOOKUP_FX_2D        "./2d/fx/"
#else
    #include "bindings.h" // If PERFORM_PATH_LOOKUP isn't enabled, we have to directly looking in the root folder, 
                          // and in this case, a convenient bindings.h can be provided (generated from ./utils/__bind.c)
    
    #define PRAGMA_PATH "./"
    #define PRAGMA_PATH "./sound/"
    #define PRAGMA_PATH "./sound/stream/"
#endif

STRING *game_asset_get_object(const STRING *asset);
STRING *game_asset_get_scene(const STRING *asset);

STRING *game_asset_get_stream(const STRING *asset);
STRING *game_asset_get_sound(const STRING *asset);

STRING *game_asset_get_gui(const STRING *asset);
STRING *game_asset_get_2d_sprite(const STRING *asset);
STRING *game_asset_get_2d_generic(const STRING *asset);
STRING *game_asset_get_2d_fx(const STRING *asset);

#include "path.c"
#endif /* path.h */
