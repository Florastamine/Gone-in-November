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
 * Processes items marked as active for rendering.
 */
void render_queue_start()
{
	#ifdef    __DOF
	    if(!(RenderState_get_singleton())->rt) render_setup_rt(); // scene RT wasn't manually activated by the user?
	    if(render_dof_is_active()) render_dof();
	#endif
	
	#ifdef    __HDR
	    if(!(RenderState_get_singleton())->rt) render_setup_rt();
	    if(render_hdr_is_active()) render_hdr();
	#endif
} 
