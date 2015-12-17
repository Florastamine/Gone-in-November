/* November.h */
#ifndef    __NOVEMBER_RAIN_H__
#define    __NOVEMBER_RAIN_H__

/*
 * In final builds (or perhaps even alpha builds),
 * PERFORM_PATH_LOOKUP and DEBUG must be disabled.
 */
// #define    A8_FREE
#define    DEBUG
#define    WIN32_LEAN_AND_MEAN
#define    WIN32_EXTRA_LEAN
#define    WINDOWS_API
#define    PERFORM_PATH_LOOKUP

#include <acknex.h>
#include <d3d9.h>
#include "./source/utils/physX/ackPhysX.h"

#include "path_source.h"
#include "./source/utils/default_ex.h"

#include "./source/game/common/path.h"
#include "./source/utils/utilities.h"
#include "./source/game/io/crypto.h"
#include "./source/game/io/file.h"
#include "./source/game/io/region.h"
#include "./source/game/io/serializer.h"

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

#include "./source/game/shared.h"
#include "./source/game/fx/unmanaged.h"
#include "./source/game/common/common.h"
#include "./source/game/gui/gui.h"
#include "./source/game/ai/behavior.h"

#endif /* November.h */
