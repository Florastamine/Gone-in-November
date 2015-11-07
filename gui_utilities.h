/* gui_utilities.h */
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
 * <gui_utilities>
 * Contains extra helpers and utilities (some are made to replace) Gamestudio's 
 * stock (which also sucks bad) GUI system.
 * LBGUI is way too heavyweight and I don't want to deal with 3rd party code either.
 * This header was specifically made for <gui>, but you can use it independently.
 * 
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    _GUI_UTILITIES_H_
#define    _GUI_UTILITIES_H_

#define __static  
#define __In 
#define __Out 
#define __namespace(namespace)             {}

#define    ALIGN_TOP_LEFT     ( 1 << 1 )
#define    ALIGN_TOP_RIGHT    ( 1 << 2 )
#define    ALIGN_BOTTOM_LEFT  ( 1 << 3 )
#define    ALIGN_BOTTOM_RIGHT ( 1 << 4 )

#define    CENTER_X           ( 1 << 5 )
#define    CENTER_Y           ( 1 << 6 )

#define    SCALE_X            ( 1 << 7 )
#define    SCALE_Y            ( 1 << 8 )

Pair *gui_image_get_dimensions( __In const Bitmap *image );
Pair *gui_image_get_dimensions( __In const String *image_file );

Pair *gui_panel_get_pos( __In Panel *panel );
Pair *gui_text_get_pos( __In Text *text );

void gui_panel_set_pos( __In Panel *panel, __In Pair *pair );
void gui_panel_set_pos( __In Panel *panel, __In float px, __In float py );

void gui_text_set_pos( __In Text *text, __In Pair *pair );
void gui_text_set_pos( __In Text *text, __In float px, __In float py );

Pair *gui_panel_get_size( __In Panel *panel );

void gui_panel_set_size( __In Panel *panel, __In float sx, __In float sy );

void gui_panel_set_align( __In Panel *panel, __In const int mode );
void gui_panel_set_scale( __In Panel *panel, __In const int __flags );
void gui_panel_set_center( __In Panel *panel, __In const int mode );

float gui_panel_get_rotation( __In Panel *panel );
void  gui_panel_set_rotation( __In __Out Panel *panel, __In float amount );

__namespace(button) {
	#define    EVENT_BUTTON_PUSH    (1 << 1)
	#define    EVENT_BUTTON_RELEASE (1 << 2)
	#define    EVENT_BUTTON_OVER    (1 << 3)
	
	typedef struct {
		Panel *__container;
		
		Bitmap *image_on;
		Bitmap *image_off;
		Bitmap *image_over;
		
		void *fptr_on;
		void *fptr_off;
		void *fptr_over;
		
		Vector2 *position;
		Vector2 *scale;
		
		Text *string;
	} GUIButton;
	
	GUIButton *gui_button_new( __In Vector2 *pos, 
	                           __In Vector2 *scale,
	                           __In String *title, 
	                           __In String *image_on,
	                           __In String *image_off,
	                           __In String *image_over,
	                           __In const void *fptr_on,
	                           __In const void *fptr_off,
	                           __In const void *fptr_over
	);
	
	void gui_button_free( __In GUIButton *b );
	
	void gui_button_event_set( __In __Out GUIButton *b, __In int mode, __In Bitmap *avatar, __In const void *event );
}

#include "gui_utilities.c"
#endif /* gui_utilities.h */
