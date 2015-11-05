/* render_utils.h */
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
 * <render_utils>
 * Additional utilities.
 * Since <render> itself doesn't have any knowledge about the preceding 
 * shaders/effects, an extra header was provided with a small set of 
 * functions for fune-tuning of each effect. After you've finished including 
 * the necessary effects' headers, "enclose" it by including <render_utils>.
 * 
 * This is not necessary and you can just ditch the header and make 
 * raw calls instead.
 * 
 * Authors: Wolfgang "BoH_Havoc" Reichardt (http://dotmos.org/)
 *          Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    _RENDER_UTILS_H_
#define    _RENDER_UTILS_H_

void render_queue_start();

#include "render_utils.c"
#endif /* render_utils.h */
