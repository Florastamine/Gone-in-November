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

#ifndef    PERFORM_PATH_LOOKUP
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
#endif

#endif /* path.h */
