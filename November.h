/* November.h */
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
 */
#ifndef    __NOVEMBER_RAIN_H__
#define    __NOVEMBER_RAIN_H__

/*
 * "Links" with the launcher. If this is commented out, the game can be started
 * without the launcher, but I don't recommend to do so as you'll lost the ability
 * of automatically updating of the game.
 */
// #define    LAUNCHER_LINKAGE

/*
 * These switches toggle debugging panels, functionalities and texts, while DEBUG_PSSM
 * allows you to see the split passes and  manually adjust the sun position.
 */
// #define    DEBUG
// #define    DEBUG_PSSM
#define    DEBUG_HDR
#define    DEBUG_MIRROR

/*
 * Toggle this switch to play the game without any in-game UI available.
 * This is necessary, for example, to record the game's trailer/teaser, where the UI
 * can easily distract the viewer if got brought with the trailer.
 */
// #define    UI_LESS

/*
 * Uses the Windows API, but with some extra stuff (WIN32_LEAN_AND_MEAN and WIN32_EXTRA_LEAN)
 * disabled. Maybe, just maybe, these "extra" stuff wasn't included in <windows>, but I just
 * want to make sure.
 */
#define    WINDOWS_API
#define    WIN32_LEAN_AND_MEAN
#define    WIN32_EXTRA_LEAN

/*
 * Performs file lookup in subdirectories. In published builds, data are packed into
 * resource files (.wrs or .pak), so this switch must be toggled off.
 */
#define    PERFORM_PATH_LOOKUP

/*
 * Performs cryptography on everything written out.
 */
#define    PERFORM_CRYPTOGRAPHY

/*
 * Uses the built-in serializer for saving and loading games instead of the stock
 * game_save()/game_load() functions. If PERFORM_CRYPTOGRAPHY is enabled,
 * cryptography will be applied on save games.
 */
#define    PERFORM_SERIALIZATION

#include <acknex.h>
/* #include <d3d9.h> */
#include "./source/utils/physX/ackPhysX.h"

#include "path_source.h"
#include "./source/utils/default_ex.h"

#include "./source/dlls/FSAA.h"
/* #include "./source/dlls/download.h" */
/* #include "./source/dlls/bmap_ex.h" */

#include "./source/game/common/path.h"
#include "./source/utils/utilities.h"
#include "./source/utils/gui_utilities.h"
#include "./source/game/io/crypto.h"
#include "./source/game/io/file.h"
#include "./source/game/io/region.h"
#include "./source/game/io/serializer.h"
#include "./source/game/io/trophy.h"

#ifndef    __0__
    #include "./source/render/render_lrays_fixed.h"
#endif

#include "./source/game/shared.h"
#include "./source/game/localized.h"
#include "./source/game/common/scene_list.h"
#include "./source/game/common/cfuncs.h"
#include "./source/game/common/common.h"
/* #include "./source/game/common/inventory.h" */
#include "./source/game/common/scene.h"
/* #include "./source/game/fx/unmanaged.h" */
#include "./source/game/gui/gui.h"
/* #include "./source/game/gui/gui_menu.h" */
#include "./source/game/ai/behavior.h"

#endif /* November.h */

// http://cafebiz.vn/nhan-vat/neu-biet-lich-lam-viec-cua-elon-musk-ban-se-khong-the-tin-ong-ay-la-con-nguoi-20160310081641534.chn
