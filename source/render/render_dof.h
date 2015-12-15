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
 * 
 * __________________________________________________________________
 * 
 * <render_dof>
 * Depth of field. 
 * Default values can be overriden with a call to render_dof_setup() and/or render_dof_depth_set().
 * To render depth of field, two calls are required: render_dof_new() and render_dof(). 
 * One thing to note though, values can only be changed as long as render_dof() hasn't been invoked.
 * 
 * Queue is also available with render_hdr_set_queued().
 * 
 * Authors: Wolfgang "BoH_Havoc" Reichardt (http://dotmos.org/)
 *          Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    A8_FREE

#ifndef    _DOF_H_
#define    _DOF_H_

#define PRAGMA_PRINT "[Depth of field] "

#define __DOF

#define __In 
#define __Out 
#define __static 

typedef struct {
	float rt_factor;
	float bit_depth; // 16, 32, 14444
	float scene_max_depth;
	
	float sharpness;
	float position;
	float blurness;
	
	BOOL queued;
} DOFState;

DOFState *DOFState_singleton = NULL;

DOFState *render_dof_get_singleton();
void render_dof_new();
void render_dof_free();
void render_dof_set_queued( __In BOOL state );
BOOL render_dof_get_queued();

void render_dof_setup( __In int rt_factor, __In int bit_depth, __In int scene_max_depth, __In int sharpness, __In int position, __In int blurness );
void render_dof_depth_set( __In float focus_speed, __In float max_depth, __In float b0n );

void __render_dof();

__static void __mtl_depth_event();
__static void __render_dof_initialize();

MATERIAL *mtl_dofDownsample = {}
MATERIAL *mtl_depth         = { flags = ENABLE_RENDER; effect = "depth.fx"; }
MATERIAL *mtl_dofHBlur      = { effect = "dofHBlur.fx";	}
MATERIAL *mtl_dofVBlur      = { effect = "dofVBlur.fx";	}
MATERIAL *mtl_dof           = { effect = "dof.fx";	}

VIEW *view_depth         = { material = mtl_depth; layer = -2; }
VIEW *view_dofDownsample = {material = mtl_dofDownsample; flags = PROCESS_TARGET; }
VIEW *view_dofHBlur      = { material = mtl_dofHBlur; flags = PROCESS_TARGET; }
VIEW *view_dofVBlur      = { material = mtl_dofVBlur; flags = PROCESS_TARGET; }
VIEW *view_dof           = { material = mtl_dof; flags = PROCESS_TARGET; }

#include "render_dof.c"
#endif /* render_dof.h */
#endif
