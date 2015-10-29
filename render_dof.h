/* render_dof.h */
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
 */
#ifndef    _DOF_H_
#define    _DOF_H_

#define __In 
#define __Out 
#define __static 

#define __DOF

typedef struct {
	float rt_factor;
	float bit_depth; // 16, 32, 14444
	float scene_max_depth;
	
	float sharpness;
	float position;
	float blurness;
	
	BOOL active;
} DOFState;

DOFState *DOFState_singleton = NULL;

DOFState *DOFState_get_singleton();
void render_dof_new();
void render_dof_free();
void render_dof_set_active( __In BOOL state );
BOOL render_dof_is_active();

void render_dof_setup( __In int rt_factor, __In int bit_depth, __In int scene_max_depth, __In int sharpness, __In int position, __In int blurness );
void render_dof_depth_set( __In float focus_speed, __In float max_depth, __In float b0n );

void render_dof();

__static void __sc_mtl_depth_event();
__static void __render_dof_initialize();

MATERIAL *sc_mtl_dofDownsample = {}
MATERIAL *sc_mtl_depth         = { flags = ENABLE_RENDER; effect = "sc_depth.fx"; }
MATERIAL *sc_mtl_dofHBlur      = { effect = "sc_dofHBlur.fx";	}
MATERIAL *sc_mtl_dofVBlur      = { effect = "sc_dofVBlur.fx";	}
MATERIAL *sc_mtl_dof           = { effect = "sc_dof.fx";	}

VIEW *sc_view_depth         = { material = sc_mtl_depth; layer = -2; }
VIEW *sc_view_dofDownsample = {material = sc_mtl_dofDownsample; flags = PROCESS_TARGET; }
VIEW *sc_view_dofHBlur      = { material = sc_mtl_dofHBlur; flags = PROCESS_TARGET; }
VIEW *sc_view_dofVBlur      = { material = sc_mtl_dofVBlur; flags = PROCESS_TARGET; }
VIEW *sc_view_dof           = { material = sc_mtl_dof; flags = PROCESS_TARGET; }

#include "render_dof.c"
#endif /* render_dof.h */
