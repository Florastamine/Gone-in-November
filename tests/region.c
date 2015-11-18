/* region.c */
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
#include <stdio.h>
#include <default.c>

#include "..\path.h"

#include "..\utilities.h"
#include "..\gui.h"
#include "..\region.h"

int main( int argc, char **argl )
{
	while( !ready() ) wait(1.0);
	
	draw_textmode( "Arial#25b", 1, 42.0, 50.0 );
	
	region_new();
	region_scan_from("../translation/");
	
	STRING *f = region_get_languages();
	// PRINT_STRING(_chr(f));
	
	BOOL b = region_set_language("vn");
	// PRINT_STRING(_chr((region_get_singleton())->language_active));n");
	
	Region_singleton->languages->flags |= (SHOW); // For debugging purposes.
	
	STRING *s = region_get_string("SAMPLE_TAG.txt");
	
	Panel *p  = pan_create(NULL, 1);
	p->bmap   = region_get_language_flag();
	p->flags |= (SHOW);
	
	gui_panel_set_pos(p, pair_new(15.0, 100.0));
	
	while(true)
	{
		draw_text(s, 10.0, 10.0, COLOR_WHITE);
		
		wait(1.0);
	}
	
	return 0;
}
