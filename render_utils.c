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
 * Processes items marked as queued for rendering.
 */
void render_queue_start()
{
	#ifdef    __DOF
	    if( render_dof_get_queued() ) render_dof();
	#endif
	
	#ifdef    __HDR
	    if( render_hdr_get_queued() ) render_hdr();
	#endif
	
	#ifdef    __REFLECT
	    if( render_reflect_get_queued() ) render_reflect();
	#endif
	
	#ifdef    __REFRACT
	    if( render_refract_get_queued() ) render_refract();
	#endif
} 
