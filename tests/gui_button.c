/* gui.c */
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

#include <acknex.h>
#include <default.c>

#include <..\utilities.h>

#include <..\common.h>
#include <..\gui_utilities.h>

GUIButton *b = NULL;

int main()
{
	while( !ready() ) wait(1.0);
	level_load(0);
	
	window_size_set( 1200.0, 600.0 );
	
	b = gui_button_new();
	
	
	while(true)
	{
		
		wait(1.0);
	}
	
 	return 0;
}

