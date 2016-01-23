/* November.h */
#ifndef    __NOVEMBER_RAIN_H__
#define    __NOVEMBER_RAIN_H__

/*
 * Turn on this switch if you're using the free version of Gamestudio/A8
 * for building/running Gone In November.
 */
// #define    A8_FREE

/*
 * These switches toggle debugging panels, functionalities and texts, while DEBUG_PSSM
 * allows you to see the split passes and  manually adjust the sun position.
 */
#define    DEBUG
#define    DEBUG_PSSM
#define    DEBUG_HDR
#define    DEBUG_MIRROR

/*
 * Uses the Windows API, but with some extra stuff (WIN32_LEAN_AND_MEAN and WIN32_EXTRA_LEAN)
 * disabled. Maybe, just maybe, these "extra" stuff wasn't included in Lite-C's <windows>, but I just
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
 * game_save()/game_load() functions. If PERFORM_CRYPTOGRAPHY was enabled,
 * cryptography will be applied on save games.
 */
#define    PERFORM_SERIALIZATION

#include <acknex.h>
#include <d3d9.h>
#include "./source/utils/physX/ackPhysX.h"

#include "path_source.h"
#include "./source/utils/default_ex.h"

#include "./source/dlls/download.h"
#include "./source/dlls/bmap_ex.h"

#include "./source/game/common/path.h"
#include "./source/utils/utilities.h"
#include "./source/game/io/crypto.h"
#include "./source/game/io/file.h"
#include "./source/game/io/region.h"
#include "./source/game/io/serializer.h"

#ifndef    A8_FREE
#include "./source/render/mtlView.c"
#include "./source/render/mtlFX.c"

#include "./source/render/render.h"
#include "./source/render/render_pp.h"
#include "./source/render/render_shadows.h"
#include "./source/render/render_surface.h"
#include "./source/render/render_lrays.h"
#include "./source/render/render_rain.h"
#include "./source/render/render_dof.h"
#include "./source/render/render_hdr.h"
#include "./source/render/render_reflect.h"
#include "./source/render/render_refract.h"
#include "./source/render/render_utils.h"
#endif

#include "./source/game/shared.h"
#include "./source/game/common/cfuncs.h"
#include "./source/game/common/common.h"
#include "./source/game/common/inventory.h"
#include "./source/game/common/scene_list.h"
#include "./source/game/common/scene.h"
#include "./source/game/fx/unmanaged.h"
#include "./source/game/gui/gui.h"
#include "./source/game/gui/gui_menu.h"
#include "./source/game/ai/behavior.h"

#endif /* November.h */
