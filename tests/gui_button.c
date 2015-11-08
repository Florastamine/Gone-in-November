/* gui_button.c */
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

#include "..\common.h"
#include "..\gui.h"

const STRING *helper = "
1 - Randomize the button's position.
2 - Toggles the button's invisibility.
3 - Resets the button's color.
4 - Randomize the button's color.
5 - Re-create the button.
6 - Executes the button's \"pushed\" event outside the button itself.
7 - Replaces the button's \"pushed\" event with another one.";

GUIButton *b = NULL;
int count = 0;

void button_randomize();
void button_randomize_color();
void button_toggle_state();
void button_reset_color();
void button_setup();
void button_execute_event_push();
void button_replace_event_push();

void event_1();
void event_2();
void event_3();

int main( int argc, char **argl )
{
	while( !ready() ) wait(1.0);
	
	random_seed(0.0);
	
	mouse_mode = 2.0;
	mouse_map  = bmap_create("arrow.tga");
	
	window_size_set(1280, 720);
	window_color_set(COLOR_WHITE);
	
	BIND_KEY( 1, button_randomize );
	BIND_KEY( 2, button_toggle_state );
	BIND_KEY( 3, button_reset_color );
	BIND_KEY( 4, button_randomize_color );
	BIND_KEY( 5, button_setup );
	BIND_KEY( 6, button_execute_event_push );
	BIND_KEY( 7, button_replace_event_push );
	
	while(true)
	{
		draw_text( helper, 15.0, 25.0, COLOR_INDIGO );
		draw_text( str_for_num(NULL, count), screen_size.x - 45.0, 0.0, COLOR_INDIGO );
		
		vec_set( &mouse_pos, &mouse_cursor );
		
		wait(1.0);
	}
	
	return 0;
}

void button_setup()
{
	if(b) gui_button_free(b);
	
	b = gui_button_new( pair_new(15.0, 75.0), "Title", "../image.jpg", "../imageoff.jpg", "../imageover.jpg", event_1, event_2, NULL);
	gui_button_set_color( b, 125.0, 147.0, 225.0 );
	gui_button_set_text_color( b, COLOR_RED );
	gui_button_show(b);
}

void button_toggle_state()
{
	if(gui_button_get_invisibility(b))
	    gui_button_hide(b);
	else
	    gui_button_show(b);
}

void button_randomize()
{
	gui_button_set_pos( b, random(screen_size.x), random(screen_size.y) );
}

void button_reset_color()
{
	gui_button_reset_color(b);
	gui_button_reset_text_color(b);
}

void button_randomize_color()
{
	gui_button_set_color(b, COLOR_RANDOM);
	gui_button_set_text_color(b, COLOR_RANDOM);
}

void button_execute_event_push()
{
	void f();
	f = gui_button_get_event(b, EVENT_BUTTON_PUSH);
	f();
}

void button_replace_event_push()
{
	gui_button_set_event(b, EVENT_BUTTON_PUSH, NULL, event_3);
}

void event_1()
{
	count++;
}

void event_2()
{
	return;
}

void event_3()
{
	count--;
}
