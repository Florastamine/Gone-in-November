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

__namespace(global) {
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
}

__namespace(button) {
	#define    EVENT_BUTTON_PUSH    (1 << 1)
	#define    EVENT_BUTTON_RELEASE (1 << 2)
	#define    EVENT_BUTTON_OVER    (1 << 3)
	
	typedef struct {
		Panel *__container;      /* Internal container of the button. Used to draw the actual button and perform effects like coloring, and such on it. */
		
		Bitmap *image_on;        /* A bitmap image describing which image to be used when the user clicked on it. */
		Bitmap *image_off;       /* A bitmap image describing which image to be used when the user clicked, and then released the mouse on it. */
		Bitmap *image_over;      /* A bitmap image describing which image to be used when the user hover the mouse on it. */
		
		void *fptr_on;           /* A function pointer contains the address of some executable code, which is invoked whenever the user clicked on the button. */
		void *fptr_off;          /* A function pointer contains the address of some executable code, which is invoked whenever the user clicked on the button, and then release the mouse. */
		void *fptr_over;         /* A function pointer contains the address of some executable code, which is invoked whenever the user hovers the mouse over the button. */
		
		Vector2 *position;       /* A pair struct (named as Vector2) describing the absolute position of the button, relative to the top-left corner of the game window. */
		// Vector2 *scale;       /* A pair struct describing the scale of the button. (deprecated - it's not necessary) */
		
		Text *string;            /* A text object contains the button label. Can be left blank. */
	} GUIButton;
	
	GUIButton *gui_button_new( __In Vector2 *pos, 
	                           __In String *title, 
	                           __In int layern, 
	                           __In String *image_on,
	                           __In String *image_off,
	                           __In String *image_over,
	                           __In const void *fptr_on,
	                           __In const void *fptr_off,
	                           __In const void *fptr_over
	);
	
	void gui_button_free( __In GUIButton *b );
	
	void gui_button_set_event( __In __Out GUIButton *b, __In int mode, __In Bitmap *avatar, __In const void *event );
	const void *gui_button_get_event( __In GUIButton *b, __In const int mode  );
	
	void gui_button_set_font( __In __Out GUIButton *b, __In const Font *f );
	Font *gui_button_get_font( __In GUIButton *b );
	
	void gui_button_set_pos( __In __Out GUIButton *b, __In float x, __In float y );
	Vector2 *gui_button_get_pos( __In GUIButton *b );
	
	void gui_button_set_color( __In __Out GUIButton *b, __In Vector3 *color );
	void gui_button_set_color( __In __Out GUIButton *b, __In float __red, __In float __green, __In float __blue );
	void gui_button_set_text_color( __In __Out GUIButton *b, __In Vector3 *color );
	void gui_button_set_text_color( __In __Out GUIButton *b, __In float __red, __In float __green, __In float __blue );
	void gui_button_reset_color( __In __Out GUIButton *b );
	void gui_button_reset_text_color( __In __Out GUIButton *b );
	
	void gui_button_show( __In __Out GUIButton *b );
	void gui_button_hide( __In __Out GUIButton *b );
	bool gui_button_get_invisibility( __In GUIButton *b );
}

#include "gui_utilities.c"
#endif /* gui_utilities.h */