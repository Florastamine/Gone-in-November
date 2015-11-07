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
#include <..\gui.h>

Panel *p = NULL;

const STRING *helper = "[ 1, 2, 3, 4 ]: Align the panel.
[ 5, 6, 7 ]: Scale the panel.
[ 8, 9, 0 ]: Center the panel.";

void panel_set_align_1();
void panel_set_align_2();
void panel_set_align_3();
void panel_set_align_4();

void panel_set_scale_1();
void panel_set_scale_2();
void panel_set_scale_3();

void panel_set_center_1();
void panel_set_center_2();
void panel_set_center_3();
 
int main()
{
	while( !ready() ) wait(1.0);
	level_load(0);
	
	window_size_set( 1200.0, 600.0 );
	
	p = pan_create(NULL, 1);
	p->bmap = bmap_create("../image.jpg");
	p->flags |= (TRANSLUCENT | SHOW);
	
	gui_panel_set_pos( p, 320.0, 240.0 );
	
	on_1 = panel_set_align_1;
	on_2 = panel_set_align_2;
	on_3 = panel_set_align_3;
	on_4 = panel_set_align_4;
	
	on_5 = panel_set_scale_1;
	on_6 = panel_set_scale_2;
	on_7 = panel_set_scale_3;
	
	on_8 = panel_set_center_1;
	on_9 = panel_set_center_2;
	on_0 = panel_set_center_3;
	
	while(true)
	{
		draw_text( helper, 10.0, 10.0, COLOR_WHITE );
		
		gui_panel_set_rotation( p, gui_panel_get_rotation(p) + 0.5 * time_step );
		
		wait(1.0);
	}
	
 	return 0;
}

void panel_set_align_1()
{
	gui_panel_set_align(p, ALIGN_TOP_LEFT);
}

void panel_set_align_2()
{
	gui_panel_set_align(p, ALIGN_TOP_RIGHT);
}

void panel_set_align_3()
{
	gui_panel_set_align(p, ALIGN_BOTTOM_LEFT);
}

void panel_set_align_4()
{
	gui_panel_set_align(p, ALIGN_BOTTOM_RIGHT);
}

void panel_set_scale_1()
{
	gui_panel_set_scale(p, SCALE_X);
}

void panel_set_scale_2()
{
	gui_panel_set_scale(p, SCALE_Y);
}

void panel_set_scale_3()
{
	gui_panel_set_scale(p, SCALE_X | SCALE_Y);
}

void panel_set_center_1()
{
	gui_panel_set_center(p, CENTER_X);
}

void panel_set_center_2()
{
	gui_panel_set_center(p, CENTER_Y);
}

void panel_set_center_3()
{
	gui_panel_set_center(p, CENTER_X | CENTER_Y);
}
