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
#include <acknex.h>
#include <stdio.h>

#include "path.h"

#include "utilities.h"
#include "file.h"

#include "render.h"
#include "render_hdr.h"
#include "render_dof.h"
#include "render_utils.h"

#include "common.h"

#define __RENDER_DOF__
#define __RENDER_HDR__

int main( int argc, char **argl )
{
	while( !ready() ) wait(1.0);
	
	window_size_set(1280, 720);
	
	level_load("scene/kathetrale02.wmb");
	
	render_new();
	
	#ifdef    __RENDER_DOF__
	    render_dof_new();
	    render_dof_depth_set(200, 5000, 0.1);
	    
	    render_dof_set_active(true);
	#endif
	
	#ifdef    __RENDER_HDR__
	    render_hdr_new();
	    
	    render_hdr_set_active(true);
	#endif
	
	render_queue_start();
	
	return 0;
}
