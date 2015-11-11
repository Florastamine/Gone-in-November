/* DPFL.c */
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
#define ENFORCE_STRICT

/*
#define __RENDER_GODRAYS__
#define __RENDER_REFRACT__
#define __RENDER_REFLECT__
#define __RENDER_SHADOW_
#define __RENDER_DOF__
#define __RENDER_HDR__
*/

#include <acknex.h>
#include <litec.h>
#include <d3d9.h>
#include <stdio.h>
#include <default.c>

#include "path.h"

#include "utilities.h"
#include "file.h"
#include "crypto.h"
#include "region.h"
#include "serializer.h"

#include "render.h"
#include "render_lrays.h"
#include "render_shadow.h"
#include "render_hdr.h"
#include "render_dof.h"
#include "render_refract.h"
#include "render_reflect.h"
#include "render_water.h"
#include "render_utils.h"

#include "common.h"
#include "gui.h"

void water()
{
	// render_water(my);
}

int main( int argc, char **argl )
{
	while( !ready() ) wait(1.0);
	
	// warn_level = 6;
	
	window_size_set(1280, 720);
	
	// level_load("scene/scene.wmb");
	// object_sky_create("sample+6.tga", 1);
	
	/*
	render_new();
	render_setup_rt();
	
	render_water_new();
	*/
	
	#ifdef    __RENDER_REFRACT__
	    render_refract_new();
	    render_refract_set_queued(true);
	#endif
	
	#ifdef    __RENDER_REFLECT__
	    render_reflect_new();
	    render_reflect_set_queued(true);
	#endif
	
	#ifdef    __RENDER_DOF__
	    render_dof_new();
	    render_dof_depth_set(200, 5000, 0.1);
	    
	    render_dof_set_queued(true);
	#endif
	
	#ifdef    __RENDER_HDR__
	    render_hdr_new();
	    
	    render_hdr_set_queued(true);
	#endif
	
	#ifdef    __RENDER_SHADOW_
	    render_shadow_new();
	    
	    mat_model->effect = "obj_doShadow.fx";
	    mat_flat->effect = "level_doShadow.fx";
	    mat_shaded->effect = "level_doShadow.fx";
	    
	    render_shadow_set_queued(true);
	#endif
	
	#ifdef    __RENDER_GODRAYS__
	    render_light_rays_new();
	    if( !render_light_rays_get_debug() ) render_light_rays_set_debug();
	    
	    render_light_rays_set_queued(true);
	#endif
	
	/* render_queue_start(); */
	
	while(true)
	{		
		wait(1.0);
	}
	
	return 0;
}
