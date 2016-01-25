/* render_utils.c */
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

/*
 * void render_queue_start()
 * 
 * Processes items marked as queued for rendering in the *exact* order as described below.
 */
void render_queue_start()
{
	#ifdef    __DOF
	    if( render_dof_get_queued() ) render_dof();
	#endif
	
	#ifdef    __HDR
	    if( render_hdr_get_queued() ) render_hdr();
	#endif
	
	/*
	#ifdef    __SHADOW
	    if( render_shadow_get_queued() ) render_shadow();
	#endif
	*/
	
	#ifdef    __GOD_RAYS
	    if( render_light_rays_get_queued() ) render_light_rays();
	#endif
} 
