/* gui_utilities.c */
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
 * VECTOR *gui_screen_get_center()
 *
 * Returns the center point of the screen.
 */
VECTOR *gui_screen_get_center()
{
	return vector(screen_size.x / 2, screen_size.y / 2, 0.0);
}

/*
 * Pair *gui_panel_get_size( Panel *panel )
 *
 * Returns the size of a given panel.
 */
Pair *gui_panel_get_size( Panel *panel )
{
	Pair *p = pair_new();

	if(panel)
	{
		p->first = panel->size_x;
		p->second = panel->size_y;
	}

	return p;
}

/*
 * void gui_panel_set_size( Panel *panel, float sx, float sy )
 *
 * Overrides the current panel size's components.
 */
void gui_panel_set_size( Panel *panel, float sx, float sy )
{
	if( !panel ) return;

	panel->size_x = sx;
	panel->size_y = sy;
}

/*
 * Pair *gui_image_get_dimensions( const Bitmap *image )
 *
 * Returns the size (dimensions) of a given bitmap image.
 * Arguments can either be passed as a bitmap struct or a string containing file name.
 */
Pair *gui_image_get_dimensions( const Bitmap *image )
{
	Pair *p = pair_new();

	if(image)
	{
		p->first = bmap_width(image);
		p->second = bmap_height(image);
	}

	return p;
}

Pair *gui_image_get_dimensions( const String *image_file )
{
	return gui_image_get_dimensions(bmap_create(image_file));
}

/*
 * Pair *gui_panel_get_pos( Panel *panel )
 *
 * Returns the current position of the panel.
 */
Pair *gui_panel_get_pos( Panel *panel )
{
	Pair *p = pair_new();

	if(panel)
	{
		p->first = panel->pos_x;
		p->second = panel->pos_y;
	}

	return p;
}

/*
 * void gui_panel_set_pos( Panel *panel, Pair *pair )
 *
 * Sets the panel's position.
 */
void gui_panel_set_pos( Panel *panel, Pair *pair )
{
	if( !panel || !pair ) return;

	panel->pos_x = pair->first;
	panel->pos_y = pair->second;
}

void gui_panel_set_pos( Panel *panel, float px, float py )
{
	if(panel)
	{
		panel->pos_x = px;
		panel->pos_y = py;
	}
}

Pair *gui_text_get_pos( Text *text )
{
	Pair *p = pair_new();

	if(text)
	{
		p->first = text->pos_x;
		p->second = text->pos_y;
	}

	return p;
}

void gui_text_set_pos( Text *text, Pair *pair )
{
	if( !pair || !text ) return;

	text->pos_x = pair->first;
	text->pos_y = pair->second;
}

void gui_text_set_pos( Text *text, float px, float py )
{
	if(text)
	{
		text->pos_x = px;
		text->pos_y = py;
	}
}

/*
 * void gui_panel_set_align( Panel *panel, const int mode )
 *
 * Align a panel. The panel must contain a valid bitmap.
 */
void gui_panel_set_align( Panel *panel, const int mode )
{
	if( !panel ) return;

	Vector3 pos;

	switch( mode ) {
		case    ALIGN_TOP_LEFT:
		{
			pos.x = 0;
			pos.y = 0;
			break;
		}

		case    ALIGN_TOP_RIGHT:
		{
			pos.x = screen_size.x - bmap_width(panel->bmap);
			pos.y = 0;
			break;
		}

		case    ALIGN_BOTTOM_LEFT:
		{
			pos.x = 0;
			pos.y = screen_size.y - bmap_height(panel->bmap);
			break;
		}

		case    ALIGN_BOTTOM_RIGHT:
		{
			pos.x = screen_size.x - bmap_width(panel->bmap);
			pos.y = screen_size.y - bmap_height(panel->bmap);
			break;
		}

		default:
		{
			vec_zero(pos);
		}
	}

	gui_panel_set_pos(panel, pos.x, pos.y);
}

/*
 * void gui_panel_set_scale( Panel *panel, const int __flags )
 *
 * Shrinks a panel. The panel must contain a valid bitmap.
 */
void gui_panel_set_scale( Panel *panel, const int __flags )
{
	if( !panel ) return;

	Pair shrinkness;
	pair_set( &shrinkness, 1.0, 1.0 );

	if( __flags & SCALE_X )
	{
		shrinkness.first = screen_size.x / bmap_width(panel->bmap);
	}
	if( __flags & SCALE_Y )
	{
		shrinkness.second = screen_size.y / bmap_height(panel->bmap);
	}

	panel->scale_x = shrinkness.first;
	panel->scale_y = shrinkness.second;

	gui_panel_set_size( panel, panel->scale_x * bmap_width(panel->bmap), panel->scale_y * bmap_width(panel->bmap) );
}

/*
 * void gui_panel_set_center( Panel *panel, const int mode )
 *
 * Align a panel to center. The panel must contain a valid bitmap.
 * The reason for this separate function instead of merging into gui_panel_set_align()
 * is you can't do flag combining on gui_panel_set_align(); besides, it was intended to only
 * use a specific align mode instead of multiple modes (which are combined by the & operator).
 */
void gui_panel_set_center( Panel *panel, const int mode )
{
	if( !panel ) return;

	if( mode & CENTER_X )
	{
		panel->pos_x = (screen_size.x - bmap_width(panel->bmap)) / 2;
	}
	if( mode & CENTER_Y )
	{
		panel->pos_y = (screen_size.y - bmap_height(panel->bmap)) / 2;
	}
}

/*
 * float gui_panel_get_rotation( Panel *panel )
 *
 * Retrieves the angle of a given panel.
 */
float gui_panel_get_rotation( Panel *panel )
{
	if(panel) return panel->angle;
}

/*
 * void gui_panel_set_rotation( Panel *panel, float amount )
 *
 * Sets the rotation of a given panel element to amount.
 * Code example to do one full rotation:
 * {
 *     Panel *p = pan_create(<...>);
 *     while( gui_panel_get_rotation(p) < 360.0 )
 *     {
 *         gui_panel_set_rotation(p, gui_panel_get_rotation() + 0.5 * time_step);
 *         wait(1.0);
 *     }
 *	}
 */
void gui_panel_set_rotation( Panel *panel, float amount )
{
	if(panel)
	{
		panel->angle = amount;
	}
}

__static void __gui_button_text_align( GUIButton *b )
{
	Vector2 _tpos;

	_tpos.first     = (b->position->first + bmap_width( b->__container->bmap ) * 0.5 ) + b->margin->first;
	_tpos.second    = (b->position->second + bmap_height( b->__container->bmap ) * 0.5 ) + b->margin->second;

	gui_text_set_pos  ( b->string, &_tpos );
}

/*
 * GUIButton *gui_button_new(<...>)
 *
 * Allocates and creates a new button object. The button object must be manually freed later using gui_button_free().
 * Button objects are a healthier alternative (and cholesterol-free too!) to Acknex's disastrous Panel structs, but they are
 * a little heavier than a raw (Panel *). Use what fits you best. With raw PANELs, you'll get faster performance (not tested yet =P)
 * but in exchange for a messy basecode with global structs - variables and low-level access everywhere.
 * With button objects, you trade some performance for high-level functionalities, and, yes, cleaner code.
 *
 * For keeping the readability of the library code - the function itself doesn't check for invalid strings, pointers, and such.
 * Which means you'll get a crash whenever something goes wrong - and you'll have to verify them yourself.
 * For example, you can check if an image exists before feeding it into gui_button_new() - that's merely a file_exists() comparison.
 *
 * Still way lighter than LBGUI.
 */
GUIButton *gui_button_new( Vector2 *pos,
                           String *title,
                           int layern,
                           String *image_on,
                           String *image_off,
                           String *image_over,
                           const void *fptr_on,
                           const void *fptr_off,
                           const void *fptr_over
)
{
	GUIButton *b = MALLOC(1, GUIButton);

	b->margin              = pair_new();
	b->__container         = pan_create(NULL, layern);
	b->image_on            = bmap_create(image_on);
	b->image_off           = bmap_create(image_off);
	b->image_over          = bmap_create(image_over);
	b->__container->bmap   = bmap_createblack( bmap_width(b->image_on), bmap_height(b->image_on), 8 ); // Creates a black curtain because we need to feed something to ->bmap.
	                                                                                                   // It acts as the "bounding box" for the button object.
	                                                                                                   // For this, I recommend that all the button object's images are the same size - because creating and managing different
	                                                                                                   // bounding box for each and every single image can be time consuming and - heck it's not worth it.

	if(pos)    b->position = pair_new( pos );
	else       b->position = pair_new();

	gui_panel_set_pos( b->__container, pos );
	/* b->scale       = pair_new( scale ); */

	b->fptr_on     = fptr_on;
	b->fptr_off    = fptr_off;
	b->fptr_over   = fptr_over;

	b->string                    = txt_create(1, layern + 1); // 1 string with 2 layer
	b->string->font              = Arial_18;
	(b->string->pstring)[0]      = str_create(title);

	pan_setbutton( b->__container, 0, 0, 0, 0, b->image_on, b->image_off, b->image_over, NULL, fptr_on, fptr_off, fptr_over );

	__gui_button_text_align(b);

	return b;
}

/*
 * void gui_button_free( GUIButton *b )
 *
 * Frees a button object.
 */
void gui_button_free( GUIButton *b )
{
	if(b)
	{
		// Remove sub-components
		txt_remove_ex(b->string);

		bmap_remove(b->image_on);
		bmap_remove(b->image_off);
		bmap_remove(b->image_over);

		pan_remove(b->__container);

		b->fptr_on   = NULL;
		b->fptr_off  = NULL;
		b->fptr_over = NULL;

		FREE(b->position);
		// free(b->scale);

		// Remove the button itself
		FREE(b);
	}
}

/*
 * const void *gui_button_get_f( GUIButton *b, const int mode )
 *
 * Extracts an event from a button object for calling outside of the object.
 * The return result must then be assigned to a function pointer to be invoked.
 */
const void *gui_button_get_event( GUIButton *b, const int mode )
{
	switch( mode )
	{
		case    EVENT_BUTTON_PUSH:     return b->fptr_on;
		case    EVENT_BUTTON_RELEASE:  return b->fptr_off;
		case    EVENT_BUTTON_OVER:     return b->fptr_over;
	}

	return NULL;
}

/*
 * void gui_button_set_font( GUIButton *b, const Font *f )
 *
 * Overrides the current font in the button object with a new one.
 * For passing raw strings - use font_create(). For example:
 *     gui_button_set_font(b, font_create("Arial#25b"));
 */
void gui_button_set_font( GUIButton *b, const Font *f )
{
	if(b)
		b->string->font = f;
}

/*
 * Font *gui_button_get_font( GUIButton *b )
 *
 * Retrieves the current font being used to draw the button object's text.
 */
Font *gui_button_get_font( GUIButton *b )
{
	if(b)
		return b->string->font;
}

/*
 * void gui_button_set_pos( GUIButton *b, float x, float y )
 *
 * Sets the button object's position.
 * Texts are automagically aligned.
 */
void gui_button_set_pos( GUIButton *b, float x, float y )
{
	if(b)
	{
		pair_set( b->position, x, y );

		gui_panel_set_pos( b->__container, b->position );
		__gui_button_text_align(b); // Remember to align the text relative to the button's position, too.
	}
}

/*
 * Vector2 *gui_button_get_pos( GUIButton *b )
 *
 * Returns the current position of the button object.
 */
Vector2 *gui_button_get_pos( GUIButton *b )
{
	if(b)
		return b->position;

	return NULL;
}

void gui_button_set_margin( GUIButton *b, float lmx, float lmy )
{
	if(b)
	{
		b->margin->first = lmx;
		b->margin->second = lmy; // "lmy" instead of "my" (name collision with my ENTITY).

		__gui_button_text_align(b);
	}
}

Vector2 *gui_button_get_margin( GUIButton *b )
{
	if(b)
		return b->margin;

	return NULL;
}

/*
 * void gui_button_set_color( GUIButton *b, Vector3 *color )
 *
 * Sets the color of the button (not the text - you should use gui_button_set_text_color() for that).
 * A weird problem is, I can't seem to alter the RGB components of the PANEL * struct without  distorting the button,
 * revealing them stupid black bars  (it was b->__container->bmap). But why? I didn't do anything to modify the size_x and size_y
 * components of the button, nor touching its position.
 * Luckily there is pan_setcolor() comes to the rescue, which allows me to modify the color of the button itself without touching
 * the panel (and its dirty color vector).
 *
 * Gamestudio/Acknex is a buggy engine/toolchain and I have to live with that, the only reason I still stick with this engine
 * is it still has loads of features that Godot is currently missing (and I prefer vanilla C).
 *
 * Another variant taking raw floats can be found below.
 */
void gui_button_set_color( GUIButton *b, Vector3 *color )
{
	if(b)
	{
		if( !(b->__container->flags & LIGHT) ) b->__container->flags |= (LIGHT);

		if(color) {
			/* vec_set( &(b->__container->red), color ); */

			pan_setcolor( b->__container, 3, 1, color );
		}
	}
}

void gui_button_set_color( GUIButton *b, float __red, float __green, float __blue )
{
	gui_button_set_color(b, vector(__blue, __green, __red));
}

/*
 * void gui_button_set_text_color( GUIButton *b, Vector3 *color )
 *
 * Sets the button object's text color.
 */
void gui_button_set_text_color( GUIButton *b, Vector3 *color )
{
	if(b)
	{
		if( !(b->string->flags & LIGHT) ) b->string->flags |= (LIGHT);

		if(color) vec_set( &(b->string->blue), color ); // Changing from "red" to "blue" seems to fix the error, weird. But not b->__container?
	}
}

void gui_button_set_text_color( GUIButton *b, float __red, float __green, float __blue )
{
	gui_button_set_text_color(b, vector(__blue, __green, __red));
}

/*
 * void gui_button_reset_color( GUIButton *b )
 *
 * Turns off button object coloring.
 * For turning off text coloring, use gui_button_reset_text_color().
 */
void gui_button_reset_color( GUIButton *b )
{
	if(b)
	{
		if( b->__container->flags & LIGHT ) b->__container->flags &= ~(LIGHT);
	}
}

/*
 * void gui_button_reset_text_color( GUIButton *b )
 *
 * Resets the button object's text color.
 */
void gui_button_reset_text_color( GUIButton *b )
{
	if(b)
	{
		gui_button_set_text_color(b, COLOR_WHITE);

		if( b->string->flags & LIGHT ) b->string->flags &= ~(LIGHT);
	}
}

/*
 * void gui_button_show( GUIButton *b )
 *
 * Renders the button.
 */
void gui_button_show( GUIButton *b )
{
	if(b)
	{
		if( !gui_button_get_invisibility(b) ) {
			if(!(b->__container->flags & OVERLAY))
				b->__container->flags |= (OVERLAY);

			b->__container->flags |= (SHOW);
			b->string->flags      |= (SHOW);
		}
	}
}

/*
 * void gui_button_hide( GUIButton *b )
 *
 * Hides the button.
 * For destroying the button instead, use gui_button_free().
 */
void gui_button_hide( GUIButton *b )
{
	if(b)
	{
		if( gui_button_get_invisibility(b) ) {
			b->__container->flags &= ~(SHOW);
			b->string->flags      &= ~(SHOW);
		}
	}
}

/*
 * bool gui_button_get_invisibility( GUIButton *b )
 *
 * Returns true if the button object is being displayed, false otherwise.
 */
bool gui_button_get_invisibility( GUIButton *b )
{
	bool ret = false;

	if(b)
	    ret = ifelse( (b->__container->flags & SHOW) && (b->string->flags & SHOW), true, false );

	return ret;
}

__static Bitmap *__get_default_avatar( GUIButton *b, int mode )
{
	switch(mode) {
		case    EVENT_BUTTON_PUSH:
		{
			return b->image_on;
		}

		case    EVENT_BUTTON_RELEASE:
		{
			return b->image_off;
		}

		case EVENT_BUTTON_OVER:
		{
			return b->image_over;
		}
	}
}

__static Bitmap *__ifelse_bitmap( Bitmap *a, Bitmap *b, Bitmap *c )
{
	if(a)  return b;
	       return c;
}

/*
 * void gui_button_set_event( GUIButton *b, int mode, Bitmap *avatar, const void *__event )
 *
 * Alters the event or an image of the button object with another one. The button object must exist.
 * For changing only the event (or the bitmap image), pass NULL to the remaining parameter. It'll then use the original value.
 * Three flags are possible, corresponding to 3 button states:
 * EVENT_BUTTON_PUSH
 * EVENT_BUTTON_RELEASE
 * EVENT_BUTTON_OVER
 * Individuals events can be extracted with gui_button_get_event() with the same flags.
 *
 * By the way, 8.46.3's ifelse() sucks. It doesn't work with engine objects, so a (sigh) alternative can be found above (__ifelse_bitmap()).
 */
void gui_button_set_event( GUIButton *b, int mode, Bitmap *avatar, const void *__event )
{
	if( !b ) return;

	// Bitmap  *a = NULL;
	void    V();
	Vector3 pos_fix;
	pan_getpos( b->__container, 3, 1, &pos_fix ); // Somehow retrieving the position in the struct yields the holy black box, so pan_getpos() comes to rescue!

	switch( mode )
	{
		case    EVENT_BUTTON_PUSH :
		{
			if(__event)
			    V = __event;
			else
			    V = gui_button_get_event(b, EVENT_BUTTON_PUSH);

			pan_setbutton(  b->__container,
			                1,
			                0,
			                pos_fix.x,
			                pos_fix.y,
			                __ifelse_bitmap(avatar, avatar, __get_default_avatar(b, EVENT_BUTTON_PUSH) ),  // Fetch for push
			                __get_default_avatar(b, EVENT_BUTTON_RELEASE),
			                __get_default_avatar(b, EVENT_BUTTON_OVER),
			                NULL,
			                V,
			                gui_button_get_event(b, EVENT_BUTTON_RELEASE),
			                gui_button_get_event(b, EVENT_BUTTON_OVER)
			);

			break;
		}

		case    EVENT_BUTTON_RELEASE :
		{
			if(__event)
			    V = __event;
			else
			    V = gui_button_get_event(b, EVENT_BUTTON_RELEASE);

			pan_setbutton(  b->__container,
			                1,
			                0,
			                pos_fix.x,
			                pos_fix.y,
			                __get_default_avatar(b, EVENT_BUTTON_PUSH),
			                __ifelse_bitmap(avatar, avatar, __get_default_avatar(b, EVENT_BUTTON_RELEASE) ),
			                __get_default_avatar(b, EVENT_BUTTON_OVER),
			                NULL,
			                gui_button_get_event(b, EVENT_BUTTON_PUSH),
			                V,
			                gui_button_get_event(b, EVENT_BUTTON_OVER)
			);

			break;
		}

		case    EVENT_BUTTON_OVER :
		{
			if(__event)
			    V = __event;
			else
			    V = gui_button_get_event(b, EVENT_BUTTON_OVER);

			pan_setbutton(  b->__container,
			                1,
			                0,
			                pos_fix.x,
			                pos_fix.y,
			                __get_default_avatar(b, EVENT_BUTTON_PUSH),
			                __get_default_avatar(b, EVENT_BUTTON_RELEASE),
			                __ifelse_bitmap(avatar, avatar, __get_default_avatar(b, EVENT_BUTTON_OVER) ),
			                NULL,
			                gui_button_get_event(b, EVENT_BUTTON_PUSH),
			                gui_button_get_event(b, EVENT_BUTTON_RELEASE),
			                V
			);

			break;
		}
	}
}

/*
 * void hex_parse(STRING *gstr, fixed *vector)
 *
 * Parses a two-byte hex color to a var * pointer.
 *
 * A modified variant which returns a var instead of out parameters is provided below.
 */
void hex_parse(STRING *gstr, fixed *vector)
{
	if( !gstr || !str_len(gstr) || str_len(gstr) > 2 )
		return;

	gstr = str_create(gstr);      // If not this, passing raw strings through
	                              // parameters would invoke errors.
	int len = str_len(gstr);
	int c = 0, h = 0, factor = 1;
	char hex[2];

	for(; c < len; ++c)
		hex[c] = (gstr->chars)[c];

	for (c = len - 1; c >= 0; c--)
	{
		if ((hex[c] >= 48) && (hex[c] <= 57))         // 0 - 9
			h += (hex[c] - 48) * factor;
		else if ((hex[c] >= 65) && (hex[c] <= 70))    // A - F
			h += (hex[c] - 65 + 10) * factor;
		else if ((hex[c] >= 97) && (hex[c] <= 102))   // a - f
			h += (hex[c] - 97 + 10) * factor;
		else
			return;

		factor *= 17;
	}

	*vector = h * 256 / (16 * 16 + 16) / 2;
}

fixed hex_parse(STRING *gstr)
{
	fixed val = 0.0;
	hex_parse(gstr, &val);

	return val;
}

int __stext_valid_tag(STRING* text, int pos)
{
	if (pos > str_len(text) - 10)
		return 5;

	int res = 0;
	int count = 0;
	BOOL cnd[4];

	cnd[0] = str_getchr(text, pos+1) != '[';
	cnd[1] = str_getchr(text, pos+2) != 'C';
	cnd[2] = str_getchr(text, pos+3) != '=';
	cnd[3] = str_getchr(text, pos+10) != ']';

	while(count < 4)
	{
		if(cnd[count])
			res++;
		count++;
	}

	return res;
}

__static int __stext_search_tag(STRING* text, int from)
{
	int i = 0;

	for (i = from; i < (str_len(text)-10); i++)
		if (__stext_valid_tag(text, i) == 0)
			return i;

	return -1;
}

__static int __stext_space_parse_back(STRING* str, int from)
{
	int i = 0;

	for (i = from - 1; i > 0; i--)
		if (str_getchr(str,i) == 32)
			break;

	return i;
}

/*
 * StaticText *gui_static_text_new(STRING *text, FONT *font, float indent, float size_y)
 *
 * Allocates and initializes a new static text object.
 */
StaticText *gui_static_text_new(STRING *text, FONT *font, float indent, float size_y)
{
	if ( size_y < indent + 1 )
		return NULL;

	StaticText* ctext   = sys_malloc(sizeof(StaticText));
	ctext->texts        = NULL;

	if(font)
		ctext->font     = font;

	ctext->indent       = indent;
	ctext->sizey        = size_y;

	// First element
	CLList* current      = sys_malloc(sizeof(CLList));
	current->element     = NULL;
	current->next        = NULL;
	ctext->texts 		 = current;

	STRING* newline = "";
	STRING* curline = "";
	STRING* worktext = "";

	int found = -1, cutstart = 0, cutend = -1;

	while (cutstart < str_len(text))
	{
		current->element = txt_create( 1, 0 );

		found = __stext_search_tag(text, cutstart);
		if (found == cutstart) // Found a tagged text?
		{
			cutstart = found + 11;
			cutend = __stext_search_tag(text, cutstart);

			if (cutend < cutstart)
				cutend = str_len(text);

			// It'd be a lot faster if we could just replace the dog slow function
			// hex_parse() with hex_to_rgb() instead.
			// TODO
			str_cut(worktext, text, found+4, found+5);
			current->element->red = hex_parse(worktext);
			str_cut(worktext, text, found+6, found+7);
			current->element->green = hex_parse(worktext);
			str_cut(worktext, text, found+8, found+9);
			current->element->blue = hex_parse(worktext);
		}
		else if (found > cutstart) // Untagged text
		{
			cutend = found;
			vec_set(&current->element->blue, COLOR_WHITE);
		}
		else // Tags couldn't be found?
			cutend = str_len(text);

		str_cut((current->element->pstring)[0], text, cutstart, cutend);

		// If the text in the line is too long, it has to be wrapped
		while ( (current->linex + str_width((current->element->pstring)[0], font)) > size_y )
		{
			var current_len = str_len((current->element->pstring)[0]);
			var validspace = 0, possiblespace = 0;
			var lastspace = current_len;
			while ((lastspace > 0) && (validspace == 0))
			{
				lastspace = __stext_space_parse_back((current->element->pstring)[0], lastspace);
				if (lastspace > 0)
				{
					possiblespace = lastspace;
					str_cut(worktext, (current->element->pstring)[0], 0, possiblespace);
					if (str_width(worktext,font)+current->linex <= size_y)
						validspace = lastspace;
				}
			}

			// we now know if there is a valid space and where it is
			if (validspace > 0)
			{	// great, we have a space to lf
				str_cut(curline, (current->element->pstring)[0], 0, validspace);
				str_cut(newline, (current->element->pstring)[0], validspace+1, current_len);
			}
			else
			{	// too bad, there is no valid space
				if ( ((current->linex == indent) && (current->liney > 0)) || ((current->linex == 0) && (current->liney == 0)) )
				{	// text at the beginning of the line
					if (possiblespace > 0)
					{	// just use the next space possible
						str_cut(curline, (current->element->pstring)[0], 0, possiblespace);
						str_cut(newline, (current->element->pstring)[0], possiblespace+1, current_len);
					}
					else
					{	// no chance for a lf
						str_cpy(curline, (current->element->pstring)[0]);
						str_cpy(newline, "");
					}
				}
				else
				{	// text at the end of the line, so just lf it->
					str_cpy(curline, "");
					str_cpy(newline, (current->element->pstring)[0]);
				}
			}

			// so time for a new line
			current->next = sys_malloc(sizeof(CLList));
			current->next->next = NULL;
			current->next->element = txt_create(1,0);
			current->next->linex = indent;
			current->next->liney = current->liney + font->dy;
			vec_set(&current->next->element->blue, &current->element->blue);

			str_cpy((current->element->pstring)[0], curline);
			str_cpy((current->next->element->pstring)[0], newline);

			current = current->next;
		}

		// Create new entry for next cycle
		current->next = sys_malloc(sizeof(CLList));
		current->next->linex = current->linex + str_width_ex((current->element->pstring)[0], font);
		current->next->liney = current->liney;
		current->next->element = NULL;
		current->next->next = NULL;
		current = current->next;
		cutstart = cutend;
	}

	ptr_remove(worktext);
	ptr_remove(curline);
	ptr_remove(newline);

	return ctext;
}

/*
 * float gui_static_text_get_height(StaticText* text)
 *
 * Retrieves the height of a specified StaticText object.
 */
float gui_static_text_get_height(StaticText* text)
{
	if(!text)
		return 0.0;

	CLList *current = text->texts;
	float height = 0;

	while (current != NULL)
	{
		height = maxv(height, current->liney);
		current = current->next;
	}

	height = (float) ifelse(text->font, height + text->font->dy, 0);

	return height;
}

/*
 * void *gui_static_text_free(StaticText* text)
 *
 * Removes a static text. The freed text can no longer be rendered or modified again.
 */
void *gui_static_text_free(StaticText* text)
{
	CLList* temp;
	CLList* current = text->texts;

	while (current != NULL)
	{
		if (current->element != NULL)
		{
			ptr_remove((current->element->pstring)[0]);
			ptr_remove(current->element);
		}
		temp = current;
		current = current->next;
		sys_free(temp);
	}

	sys_free(text);

	return NULL;
}

/*
 * void gui_static_text_render(StaticText* text, float posx, float posy, float alpha, int layer)
 *
 * Renders a static text object.
 * The object must be first created and initialized through gui_static_text_new().
 */
void gui_static_text_render(StaticText* text, float posx, float posy, float alpha, int layer)
{
	CLList *current = text->texts;
	text->posx = posx;
	text->posy = posy;

	do
	{
		current->element->pos_x = posx + current->linex;
		current->element->pos_y = posy + current->liney;
		current->element->font = text->font;
		layer_sort(current->element, layer);
		current->element->alpha = clamp(alpha,0,100);

		if (alpha <= 0.0)
			current->element->flags &= ~(SHOW | LIGHT);
		else if (alpha >= 100) // Safer, what if someone passes 100.51 to the parameter?
		{
			current->element->flags &= ~(TRANSLUCENT);
			current->element->flags |= (LIGHT | SHOW);
		}
		else
			current->element->flags |= (LIGHT | SHOW);

		if (current->next != NULL)
			current = current->next;

	} while (current->next != NULL);
}

/*
 * void gui_static_text_hide(StaticText *text)
 *
 * Quickly hides a StaticText object.
 */
 void gui_static_text_hide(StaticText *text)
 {
 	if(text)
 	{
 		CLList *current = text->texts;

 		do
 		{
 			if(current->element->flags & SHOW)
 				current->element->flags &= (~SHOW);

 			if (current->next != NULL)
 				current = current->next;

 		} while (current->next != NULL);
 	}
 }

void gui_static_text_render(StaticText *text, Pair *pos, float alpha, int layer)
{
	if(!pos)
		pos = pair_new(0.0, 0.0);

	gui_static_text_render(text, pos->first, pos->second, alpha, layer);
}

/*
 * void gui_notifier_new( StaticText *stext, float pos_x, float pos_y,  float duration, int layer, int destruct )
 *
 * Creates a new notification object at the specified layer.
 * The static text object must be initialized and set up with data before it can be used within gui_notifier_new().
 *
 * Data about the old position and the layer of the StaicText object is ignored. Thus, when invoking
 * gui_notifier_new(), you'll have to pass the new position and the layer on which the object will be rendered.
 */
void gui_notifier_new( StaticText *stext, float pos_x, float pos_y,  float duration, int layer, int destruct )
{
	if(!stext)
		return;

	duration = (float) ifelse(duration > 0.0, duration, 3.0);

	float height = gui_static_text_get_height(stext) + NOTIFICATION_BOX_VERTICAL_BORDER_SIZE;      // For both the upper border and the lower border.
	float width  = stext->sizey + NOTIFICATION_BOX_HORIZONTAL_BORDER_SIZE;                         // For both the left border and the right border.

	Panel *container   = pan_create(NULL, layer - 1);
	container->bmap    = bmap_createblack(width, height, 8);
	container->pos_x   = pos_x - NOTIFICATION_BOX_HORIZONTAL_BORDER_SIZE * 0.5;
	container->pos_y   = pos_y - NOTIFICATION_BOX_VERTICAL_BORDER_SIZE * 0.5;
	container->flags  |= (TRANSLUCENT | SHOW);
	container->alpha   = 0.0;

	while(container->alpha <= NOTIFICATION_BOX_MAXIMUM_TRANSLUCENCY)
	{
		container->alpha += NOTIFICATION_BOX_FADE_IN_SPEED * time_step;
		wait(1.0);
	}

	gui_static_text_render(stext, pos_x, pos_y, 100.0, layer);
	wait(-abs(duration));

	int hid = 0;
	while(container->alpha > 0.0)
	{
		container->alpha -= NOTIFICATION_BOX_FADE_OUT_SPEED * time_step;
		if((container->alpha >= NOTIFICATION_BOX_TEXT_HIDE_POINT) && !hid)
		{
			if(!destruct)
				gui_static_text_hide(stext);
			else
				gui_static_text_free(stext);

			hid = 1;
		}
		wait(1.0);
	}
}

__static void __gui_pbar_update_bar_color( ProgressBar *pbar, VECTOR *c, int face )
{
	if(pbar)
	{
		if(face) // Update the front bar (the actual progress bar), taking transparency into account.
			bmap_fill(pbar->bar->bmap, c, gui_pbar_get_translucency(pbar));
		else     // Update the outliner.
			bmap_fill(pbar->outline->bmap, c, gui_pbar_get_translucency(pbar));
	}
}

/*
 * void gui_pbar_update_pos(  ProgressBar *pbar,  float x,  float y )
 *
 * Updates the progress bar with a new position.
 */
void gui_pbar_update_pos(  ProgressBar *pbar,  float x,  float y )
{
	if(pbar)
	{
		if(x != pbar->outline->pos_x)
		{
			pbar->outline->pos_x = x;
			pbar->bar->pos_x = pbar->outline->pos_x + PROGRESS_BAR_RELATIVE_OFFSET_X * 0.5;
		}

		if(y != pbar->outline->pos_y)
		{
			pbar->outline->pos_y = y;
			pbar->bar->pos_y = pbar->outline->pos_y + PROGRESS_BAR_RELATIVE_OFFSET_Y * 0.5;
		}

		// Update the string.
		pbar->string->pos_x = pbar->outline->pos_x + 1.5;
		pbar->string->pos_y = pbar->outline->pos_y - pbar->string->font->dy - 5.0; // Play with 5.0 - just an approximation. TODO it's a bugggggg
	}
}

/*
 * void gui_pbar_free(  ProgressBar *pbar )
 *
 * Frees a previously allocated progress bar through gui_pbar_new().
 */
void gui_pbar_free(  ProgressBar *pbar )
{
	if(pbar)
	{
		safe_remove(pbar->outline);
		safe_remove(pbar->bar);

		FREE(pbar);
	}
}

/*
 * void gui_pbar_update_progress(  ProgressBar *pbar,  float progress )
 *
 * Updates the progress bar with a new progress value (in percent).
 */
void gui_pbar_update_progress(  ProgressBar *pbar,  float progress )
{
	if(pbar)
		pbar->bar->scale_x = clamp(progress, 0.0, 100.0) / 100.0;
}

/*
 * void gui_pbar_update_layer( ProgressBar *pbar, int layer )
 *
 * Updates the progress bar with a new layer order.
 */
void gui_pbar_update_layer( ProgressBar *pbar, int layer )
{
	if(pbar)
	{
		layer_sort(pbar->outline, layer);
		layer_sort(pbar->bar, layer + 1);
	}
}

/*
 * ProgressBar *gui_pbar_new(  const STRING *outliner_file,  const STRING *bar_file,  float progress,  int layer )
 *
 * Allocates and initializes a new progress bar object.
 * If NULL was passed to either outliner_file or bar_file, a 16-bit colored bitmap is created instead.
 */
ProgressBar *gui_pbar_new(  const STRING *outliner_file,  const STRING *bar_file,  float progress,  int layer )
{
	layer        = (int) ifelse(layer > 0, layer, 1);
	progress     = (float) ifelse(progress > 0.0, progress, 100.0);

	ProgressBar *pbar = MALLOC(1, ProgressBar);
	pbar->layer   = layer;
	pbar->outline = pan_create(NULL, layer);
	pbar->string  = txt_create(1, layer);
	pbar->bar     = pan_create(NULL, layer + 1);

	if(bar_file)
		pbar->bar->bmap = bmap_create(bar_file);
	else
	{
		pbar->bar->bmap = bmap_createblack(PROGRESS_BAR_DEFAULT_BAR_SIZE_X, PROGRESS_BAR_DEFAULT_BAR_SIZE_Y, 16); // 16 bit so that it can be filled with bmap_fill().

		// bmap_fill(pbar->bar->bmap, vec_fill(nullvector, 255.0), 100.0); /* "can we get much higher, can we get much lighter?       */
		                                                                   /* Navigator to heaven..."                                 */
																		   /* Seriously, comment out this line to enjoy a beautiful   */
																		   /* lift off.                                               */

		bmap_fill(pbar->bar->bmap, vector(212, 255, 127), 100.0); // Aquamarine color.
	}

	if(outliner_file)
		pbar->outline->bmap = bmap_create(outliner_file);
	else
		pbar->outline->bmap = bmap_createblack(PROGRESS_BAR_DEFAULT_BAR_SIZE_X + PROGRESS_BAR_RELATIVE_OFFSET_X, PROGRESS_BAR_DEFAULT_BAR_SIZE_Y + PROGRESS_BAR_RELATIVE_OFFSET_Y, 16);

	if(progress > 0.0)
		gui_pbar_update_progress(pbar, progress);

	return pbar;
}

/*
 * void gui_pbar_update_text( ProgressBar *pbar, const String *string, const Font *font, const Vector *color )
 *
 * Updates the progress bar string's various attributes.
 */
void gui_pbar_update_text( ProgressBar *pbar, const String *string, const Font *font, const Vector *color )
{
	if(pbar)
	{
		if(string)
			str_cpy((pbar->string->pstring)[0], string);
		else
			str_cpy((pbar->string->pstring)[0], "Loading...");

		if(font)
			pbar->string->font = font;

		if(color)
			vec_set(&(pbar->string->blue), color);
	}
}

ProgressBar *gui_pbar_new()
{
	return gui_pbar_new(0, 0, 0.0, 1);
}

/*
 * void gui_pbar_render( ProgressBar *pbar )
 *
 * Renders an initialized progress bar.
 */
void gui_pbar_render( ProgressBar *pbar )
{
	if(pbar)
	{
		if( !(pbar->bar->flags & SHOW) )
		{
			pbar->bar->flags |= (SHOW);
			pbar->outline->flags |= (SHOW);
			pbar->string->flags |= (SHOW);
		}
	}
}

/*
 * void gui_pbar_hide(  ProgressBar *pbar )
 *
 * Hides an initialized progress bar.
 */
void gui_pbar_hide(  ProgressBar *pbar )
{
	if(pbar)
	{
		if(pbar->bar->flags & SHOW)
		{
			pbar->bar->flags &= ~(SHOW);
			pbar->outline->flags &= ~(SHOW);
			pbar->string->flags &= ~(SHOW);
		}
	}
}

/*
 * void gui_pbar_update_color_back( ProgressBar *pbar, VECTOR *color )
 * void gui_pbar_update_color_front( ProgressBar *pbar, VECTOR *color )
 *
 * Updates the front (the actual progress bar) and the back (outliner) color
 * of a given progress bar.
 */
void gui_pbar_update_color_back( ProgressBar *pbar, VECTOR *color )
{
	__gui_pbar_update_bar_color(pbar, color, 0);
}

void gui_pbar_update_color_front( ProgressBar *pbar, VECTOR *color )
{
	__gui_pbar_update_bar_color(pbar, color, 1);
}

float gui_pbar_get_progress(  ProgressBar *pbar )
{
	return (float) ifelse(pbar != NULL, pbar->progress, -1);
}

int gui_pbar_get_layer(  ProgressBar *pbar )
{
	return (int) ifelse(pbar != NULL, pbar->layer, -1);
}

float gui_pbar_get_translucency( ProgressBar *pbar )
{
	float r = -1.0;

	if(pbar)
	{
		if( !(pbar->bar->flags & TRANSLUCENT) )
			r = pbar->bar->alpha;
		else
			r = 100.0; // No transparency defaults to 100% opaque.
	}

	return r;
}

/*
 * void gui_dialogue_update_pos( StaticDialogue *dialog, float x, float y )
 *
 * Updates the current position of the dialogue object.
 * Everything will be automatically updated and adjusted.
 */
void gui_dialogue_update_pos( StaticDialogue *dialog, float x, float y )
{
	if(dialog)
	{
		// Update the GUI elements.
		if(x != dialog->x)
		{
			dialog->background->pos_x = dialog->x = x;
			dialog->avatar->pos_x = ifelse(dialog->avatar != NULL, x + STATIC_DIALOGUE_AVATAR_OFFSET_X, dialog->avatar->pos_x);
		}

		if(y != dialog->y)
		{
			dialog->background->pos_y = dialog->y = y;
			dialog->avatar->pos_y = ifelse(dialog->avatar != NULL, y - (bmap_height(dialog->avatar->bmap)/2) - STATIC_DIALOGUE_AVATAR_OFFSET_Y, dialog->avatar->pos_y);
		}

		// Now re-parse the text with the new position, which means you can modify the text one more time
		// before calling the resize function.
		gui_dialogue_parse(dialog, dialog->__cursor + 1);
	}
}

/*
 * void gui_dialogue_update_layer( StaticDialogue *dialog, int layer )
 *
 * Updates the dialogue object with a new layer order.
 */
void gui_dialogue_update_layer( StaticDialogue *dialog, int layer )
{
	if(dialog)
	{
		dialog->layer = (int) ifelse(layer > 0, layer, abs(layer));

		layer_sort(dialog->background, dialog->layer);
		layer_sort(dialog->avatar, dialog->layer + 1);

		// We don't need to update the static text layer as it always uses
		// dialog->layer, which we've changed below. We just need to re-parse the text.
		gui_dialogue_parse(dialog, dialog->__cursor + 1);
	}
}

/*
 * StaticDialogue *gui_dialogue_new( const char *text_file, const char *avatar, const char *background, int layer, int unicode, float width, float height )
 *
 * Allocates and initializes a new dialogue object. The text file must exists and contains at least one string.
 * If no background image is provided, a default, black-filled image is created instead. Otherwise, the background is cut (from top-left) to fit the given width and height.
 * Additionally, the image must be in the following formats: .tga, .bmp or .pcx.
 * Pass true to the unicode flag if you want to use UTF-8-encoded texts.
 * After the object is created, additional setup (font, alpha, ...) can be done through particular APIs (check the header), or call gui_dialogue_render() to make the object visible, and use the gui_dialogue_parse*() functions to navigate between lines.
 *
 * Objects created this way must be destroyed later via gui_dialogue_free().
 */
StaticDialogue *gui_dialogue_new( const char *text_file, const char *avatar, const char *background, int layer, int unicode, float width, float height )
{
	if(!text_file)
		return NULL;

	width = (float) ifelse(width > 0, width, 640.0);
	height = (float) ifelse(height > 0, height, 160.0);

	StaticDialogue *dialog = MALLOC(1, StaticDialogue);
	dialog->__container    = txt_create(STATIC_DIALOGUE_MAX_DIALOGUES, 1);
	dialog->__cursor       = 0;
	dialog->layer          = (int) ifelse(layer > 0, layer, 1);
	dialog->background     = pan_create(NULL, dialog->layer);
	dialog->font           = font_create("Arial#20b");
	dialog->alpha          = 100.0;
	dialog->snd_visible    = NULL;
	dialog->snd_invisible  = NULL;
	dialog->snd_traversal  = NULL;
	dialog->volume         = 100.0;

	/*
	 * Pastes the text file's content into the container, and returns the number of strings pasted.
	 * Becauses txt_load(w)() uses delimit_str in combination with file_str_readto() to parse
	 * content into the struct, we have to modify the original delimiter so the comma character (',')
	 * can be read normally.
	 */
	STRING *old_delimiter = str_create(delimit_str);
	str_cpy(delimit_str, "!##$&&^@#^^"); // Nobody would knows.

	if(unicode)
		dialog->lines = txt_loadw(dialog->__container, text_file);
	else
		dialog->lines = txt_load(dialog->__container, text_file);

	str_cpy(delimit_str, old_delimiter);

	if(avatar)
	{
		dialog->avatar           = pan_create(NULL, dialog->layer + 1);
		dialog->avatar->bmap     = bmap_create(avatar);
	}

	if(background)
		dialog->background->bmap = bmap_createpart(background, 0.0, 0.0, width, height); // Cut from the top-left corner (0; 0).
	else
		dialog->background->bmap = bmap_createblack(width, height, 16);

	// Set the default position and render the very first static text object.
	gui_dialogue_update_pos(dialog, (screen_size.x - bmap_width(dialog->background->bmap)) / 2, screen_size.y - bmap_height(dialog->background->bmap) - 16.0);

	return dialog;
}

/*
 * void gui_dialogue_free( StaticDialogue *dialog )
 *
 * Frees a previously initialized dialogue object through gui_dialogue_new().
 */
void gui_dialogue_free( StaticDialogue *dialog )
{
	if(dialog)
	{
		#ifdef    _UTILITIES_H_
			txt_remove_ex(dialog->__container);
		#else
			int i = 0;
			for(; i < dialog->__container->strings; i++)
				str_remove((dialog->__container->pstring)[i]);

			txt_remove(dialog->__container);
		#endif

		safe_remove(dialog->background);

		if(dialog->avatar)
			safe_remove(dialog->avatar);

		if(dialog->string)
			gui_static_text_free(dialog->string);

		FREE(dialog);
	}
}

/*
 * void gui_dialogue_render( StaticDialogue *dialog )
 *
 * Renders a dialogue object.
 */
void gui_dialogue_render( StaticDialogue *dialog )
{
	if(dialog)
	{
		// If the dialogue is already shown, or if there is an instance of the function is being run, then we'll just return.
		if((dialog->background->flags & SHOW) || proc_status(gui_dialogue_render) > 1)
			return;

		// Otherwise play the sound if one is present.
		if(dialog->snd_visible)
			if(!snd_playing(dialog->__snd_visible_handler))
				dialog->__snd_visible_handler = snd_play(dialog->snd_visible, gui_dialogue_get_volume(dialog), 0);

		// And then display the GUI elements.
		if(dialog->avatar)
			if( !(dialog->avatar->flags & SHOW) )
				dialog->avatar->flags |= (SHOW);

		if( !(dialog->background->flags & SHOW) )
		{
			dialog->background->flags |= (SHOW);

			// Performs fading in if the background has transparency set.
			if(dialog->alpha < 100.0)
			{
				dialog->background->alpha = 0.0;

				while(dialog->background->alpha < dialog->alpha)
				{
					dialog->background->alpha += STATIC_DIALOGUE_FADE_SPEED * time_step;
					wait(1.0);
				}
			}

			if(dialog->string) // Shows the text object.
			{
				CLList *current = dialog->string->texts;

				do
				{
					if(!(current->element->flags & SHOW))
						current->element->flags |= (SHOW);

					if (current->next != NULL)
						current = current->next;

				} while (current->next != NULL);
			}
		}
	}
}

/*
 * void gui_dialogue_hide( StaticDialogue *dialog )
 *
 * Hides a dialogue object.
 */
void gui_dialogue_hide( StaticDialogue *dialog )
{
	// If there is an instance of the function is being run, return, or the dialogue is already invisible.
	if(proc_status(gui_dialogue_hide) > 1 || !(dialog->background->flags & SHOW))
		return;

	if(dialog)
	{
		// Otherwise play the sound effect (if it's present), and then hide the object.
		if(dialog->snd_invisible)
			if(!snd_playing(dialog->__snd_invisible_handler))
				dialog->__snd_invisible_handler = snd_play(dialog->snd_invisible, gui_dialogue_get_volume(dialog), 0);

		gui_static_text_hide(dialog->string);

		if(dialog->avatar)
			if(dialog->avatar->flags & SHOW)
				dialog->avatar->flags &= ~(SHOW);

		// Performs fading out if the background image has transparency set.
		if(dialog->background->flags & SHOW)
		{
			if(dialog->background->flags & TRANSLUCENT)
			{
				while(dialog->background->alpha > 0.0)
				{
					dialog->background->alpha -= STATIC_DIALOGUE_FADE_SPEED * time_step;
					wait(1.0);
				}
			}

			dialog->background->flags &= ~(SHOW);
		}
	}
}

/*
 * void gui_dialogue_parse( StaticDialogue *dialog, int pos )
 *
 * Parses and renders a given text. Possible range: [1; dialog->lines]
 * (dialog->lines is the number of dialogue strings scanned from the text file).
 *
 * Additional functions used for sequential parsing is provided below. gui_dialogue_parse_next()
 * automatically destroys the object after the last dialogue line is reached.
 */
void gui_dialogue_parse( StaticDialogue *dialog, int pos )
{
	if(dialog)
	{
		if((pos > 0 && pos <= dialog->lines) && dialog->lines > 0 && (dialog->background->flags & SHOW))
		{
			if(dialog->string)
				gui_static_text_free(dialog->string);

			float write_x = 0, write_y = 0; // Calculate the final position we'll be writing our text to.

			write_x = dialog->background->pos_x + STATIC_DIALOGUE_TEXT_OFFSET_X; // X offset is easy.

			if(dialog->avatar)
				write_y = dialog->avatar->pos_y + bmap_height(dialog->avatar->bmap) + STATIC_DIALOGUE_TEXT_OFFSET_Y;
			else
				write_y = dialog->background->pos_y + STATIC_DIALOGUE_TEXT_OFFSET_Y;

			// Now create and parse the static string, and re-size the dialogue height to fit. (TODO!)
			dialog->string = gui_static_text_new((dialog->__container->pstring)[pos - 1], dialog->font, 0, bmap_width(dialog->background->bmap) - STATIC_DIALOGUE_TEXT_OFFSET_X);

			// Play the sound if present.
			if(dialog->snd_traversal)
				if(!snd_playing(dialog->__snd_traversal_handler))
					dialog->__snd_traversal_handler = snd_play(dialog->snd_traversal, gui_dialogue_get_volume(dialog), 0);

			// Render the text.
			gui_static_text_render(dialog->string, write_x, write_y, 100.0, dialog->layer + 2);
		}
	}
}

/*
 * void gui_dialogue_update_color( StaticDialogue *dialog, VECTOR *color )
 *
 * Replaces the color of the dialogue image with a new one.
 */
void gui_dialogue_update_color( StaticDialogue *dialog, VECTOR *color )
{
	if(dialog && color)
		bmap_fill(dialog->background->bmap, color, dialog->alpha);
}

void gui_dialogue_parse_next( StaticDialogue *dialog )
{
	if(dialog->__cursor < dialog->lines)
	{
		gui_dialogue_parse(dialog, dialog->__cursor + 1);
		dialog->__cursor += 1;
	}
	else // We've reached the end of the text, let's destroy the dialogue and (probably) add some eye-catching effects to it.
	{
		gui_dialogue_hide(dialog);

		while(proc_status(gui_dialogue_hide))
			wait(1.0);

		gui_dialogue_free(dialog);
	}
}

void gui_dialogue_parse_prev( StaticDialogue *dialog )
{
	if(dialog->__cursor >= 0)
	{
		gui_dialogue_parse(dialog, dialog->__cursor + 1);
		dialog->__cursor -= 1;
	}
}

/*
 * void gui_dialogue_update_font( StaticDialogue *dialog, consg Font *font )
 *
 * Updates the font used for text rendering with a new one.
 */
void gui_dialogue_update_font( StaticDialogue *dialog, const Font *font )
{
	if(dialog)
		dialog->font = font;
}

/*
 * void gui_dialogue_update_alpha( StaticDialogue *dialog, float alpha )
 *
 * Updates the alpha value of the dialogue image.
 * At 0.0 or lower, the image is not rendered (switched off), because <= 0.0 equals to full transparent.
 * At 100.0 or higher, the image's translucent flag is switched off, because >= 100.0 equals to full opaque.
 */
void gui_dialogue_update_alpha( StaticDialogue *dialog, float alpha )
{
	if(dialog)
	{
		if(alpha > 0.0 && alpha < 100.0)
		{
			dialog->background->flags |= (TRANSLUCENT);
			dialog->background->alpha = alpha;
		}
		else if(alpha >= 100.0)
		{
			if(dialog->background->flags & TRANSLUCENT)
				dialog->background->flags &= ~(TRANSLUCENT);
		}

		dialog->alpha = alpha; // Store for later use.
	}
}

/*
 * void gui_dialogue_update_volume( StaticDialogue *dialog, float volume )
 *
 * Updates the current volume of the dialogue sound effects.
 */
void gui_dialogue_update_volume( StaticDialogue *dialog, float volume )
{
	if(dialog)
		dialog->volume = (float) ifelse(volume > 0.0, volume, midi_vol);
}

#define __CREATE_SOUND(channel, file) if(dialog->snd_##channel##) snd_remove(dialog->snd_##channel##); dialog->snd_##channel## = snd_create(file)

/*
 * void gui_dialogue_update_sound( StaticDialogue *dialog, int channel, String *snd_file )
 *
 * Sets the sound to be played when certain events of the dialogue object is executed.
 * (visible, invisible and when traversing between dialogue lines).
 * The second parameter accepts SOUND_SHOW, SOUND_HIDE or SOUND_TRAVERSAL and the third
 * parameter gives the exact location to the sound file to be played with certain events.
 */
void gui_dialogue_update_sound( StaticDialogue *dialog, int channel, String *snd_file )
{
	if(dialog && snd_file)
	{
		switch(channel)
		{
			 case    SOUND_SHOW:
			 {
				 __CREATE_SOUND(visible, snd_file);
				 break;
			 }

			 case    SOUND_HIDE:
			 {
				 __CREATE_SOUND(invisible, snd_file);
				 break;
			 }

			 case    SOUND_TRAVERSAL:
			 {
				 __CREATE_SOUND(traversal, snd_file);
				 break;
			 }
		}
	}
}

FONT *gui_dialogue_get_font( StaticDialogue *dialog )
{
	if(!dialog)
		return NULL;

	return dialog->font;
}

float gui_dialogue_get_alpha( StaticDialogue *dialog )
{
	return (float) ifelse(dialog, dialog->alpha, -1.0);
}

int gui_dialogue_get_lines( StaticDialogue *dialog )
{
	return (int) ifelse(dialog, dialog->lines, -1);
}

int gui_dialogue_get_layer( StaticDialogue *dialog )
{
	return (int) ifelse(dialog, dialog->layer, -1);
}

float gui_dialogue_get_volume( StaticDialogue *dialog )
{
	return (float) ifelse(dialog, dialog->volume, -1.0);
}

/*
 * StaticTitleText *gui_title_new( Vector3 *pos, Vector3 *color, String *content, float time, int layer )
 *
 * Creates and returns a new static title text object, and gives it a few basic properties.
 * After the text is created, it can be shown with a call to gui_title_show().
 * Upon the completion of gui_title_show(), the text will be automatically freed (unless an additional flag
 * is specified in kill during the call to gui_title_show()).
 */
StaticTitleText *gui_title_new( Vector3 *pos, Vector3 *color, String *content, float time, int layer )
{
	/*
	* "Ông can ơi ông can,
	* Con mời ông lên con biếu ông nhang,
	* Ông can vẫn là ông can, từ Cao Bằng cho tới Long An,
	* Xin để con yên ổn làm ăn, dù cho 1 chữ con cũng quyết không than,
	* Tụi con xây đền thờ để tượng ông sau nhà, cúng bái ông để cho ông không gian,
	* Tụi con ăn tép, sao ông lại ăn tôm,
	* Nhà không có két, thì ông cũng ăn trộm,
	* Nhà con không quả cũng không trái, nhưng khi được việc ông ghé vào ăn thơm.
	* Không phải nói phét, nhưng chẳng cần xin phép, mặt mày lấm lét ông ăn cả mâm cơm,
	* Nhưng sao mọi khi thì chỉ có tụi con, mỗi khi nào ăn đạn và ăn bom?"
	*                                                B-Ray, Young H (Ông Can)
	 */
	StaticTitleText *text = MALLOC(1, StaticTitleText);
	text->position        = (VECTOR *) vifelse(pos != NULL, vector(pos->x, pos->y, pos->z), nullvector);
	text->color           = (VECTOR *) vifelse(color != NULL, vector(color->x, color->y, color->z), nullvector);
	text->time            =            ifelse(time > 0.0, time, 5.0);
	text->sound           = NULL;
	text->delay           = .35;
	text->fade_mode       = CHAR_BY_CHAR;
	text->done            = 0;

	if(content)
		text->content = str_create(content);
	else
		text->content = str_create("Unnamed StaticTitleText");

	layer = (int) ifelse(layer > 0, layer, 1);
	text->__container           = txt_create(1, layer);
	text->__container->font     = font_create("Arial#18b");
	text->__container->pos_x    = text->position->x;
	text->__container->pos_y    = text->position->y;

	vec_set(&(text->__container->blue), text->color);

	return text;
}

/*
 * void gui_title_free( StaticTitleText *text )
 *
 * Frees a previously created static text object.
 */
void gui_title_free( StaticTitleText *text )
{
	/*
	 * Roi cung anh mot lan, tan cung anh mot lan,
	 * nhu la mua tren mai ton, nhu la mua tren mai ton...
	 *                                            - Den.
	 */
	if(text != NULL) {
		// Removes the internal text container.
		str_remove((text->__container->pstring)[0]);
		txt_remove(text->__container);

		// ...and also removes the text object itself.
		str_remove(text->content);
		ptr_remove(text->sound);

		FREE(text);
		text = NULL;
	}
}

/*
 * void gui_title_show( StaticTitleText *text, int kill )
 *
 * Performs rendering the text object, and frees itself whenever it's done.
 * (text objects are intended to be only displayed one - YODO - You Only Display Once(TM)),
 * unless you explicitly state that it may not do so - just feed 0 to the second parameter.
 */
void gui_title_show( StaticTitleText *text, int kill )
{
	if(text != NULL)
	{
		text->__container->flags |= (LIGHT | SHOW);

		int len = str_len(text->content), i = 1;

		str_remove((text->__container->pstring)[0]);
		(text->__container->pstring)[0] = str_create_ex(len + 1);

		while(i <= len)
		{
			/*
			 * str_getchr() will return a single char at the position specified by i in string text->content.
			 * The whole (((text->__container->pstring)[0])->chars)[i - 1] gives access to the (i - 1)-th character
			 * in the internal (char *) array inside (STRING *), because str_cat() only accepts (STRING *) to be concatenated, and
			 * passing str_cat(str, &c) would yield errors.
			 * To make sure the internal char * array has enough room, we've allocated beforehand with str_create_ex() (which is defined in <utilities>).
			 */
			(((text->__container->pstring)[0])->chars)[i - 1] = str_getchr(text->content, i);

			int roll = rand() % 4;
			if((text->sound)[roll])
				snd_play((text->sound)[roll], 25.0, 0.0);

			wait(-text->delay);

			i++;
			wait(1.0);
		}
		(((text->__container->pstring)[0])->chars)[i - 1] = 0; // Garbage otherwise.

		// Now wait for text->time seconds before hiding the string.
		float time = 0.0;
		while(time < text->time)
		{
			time += time_step / 16;
			wait(1.0);
		}

		if(text->fade_mode == CHAR_BY_CHAR)
		{
			i = 1;
			float new_delay = text->delay * 0.5; // Cuts the delay time in half, disappear time should be only 1/2 of the total delay.
			while(i <= len)
			{
				str_setchr((text->__container->pstring)[0], i, ' ');

				int r = rand() % 4;
				if((text->sound)[r])
					snd_play((text->sound)[r], 25.0, 0.0);

				wait(-new_delay);

				i++;
				wait(1.0);
			}
		}
		else
		{
			if(!(text->__container->flags & TRANSLUCENT))
				SHOW_FLAGS_SAFE(text->__container, TRANSLUCENT);

			text->__container->alpha = 100.0;
			while(text->__container->alpha > 0.0)
			{
				text->__container->alpha -= 4.2 * time_step;
				wait(1.0);
			}
		}

		text->done += 1;

		if( kill )
			gui_title_free(text);
	}
}

void gui_title_show( StaticTitleText *text )
{
	gui_title_show(text, 1); // Kills the text object upon completion.
}

/*
 * void gui_title_set_sound( StaticTitleText *text, int slot, const char *filename )
 *
 * Sets the sound file to be played whenever a character is shown during the execution of gui_title_show().
 */
void gui_title_set_sound( StaticTitleText *text, int slot, const char *filename )
{
    if(text)
    {
		slot = (int) ifelse(slot > 0, slot, 1);

		if((text->sound)[slot])
			ptr_remove((text->sound)[slot - 1]);

		((text->sound)[slot]) = snd_create(filename);
	}
}

/*
 * void gui_title_set_delay( StaticTitleText *text, float delay )
 *
 * Sets the delay time (in seconds) between each character's appearance.
 */
void gui_title_set_delay( StaticTitleText *text, float delay )
{
	if(text)
		text->delay = delay;
}

/*
 * CreditsText *gui_credits_new( const char *text_file, const char *track_file, const char image_file, const Vector3 *image_color, int layer )
 *
 * Creates a brand new credits text object to be displayed later.
 * Just fill in some basic information, and with a call to gui_credits_show(), you're set.
 * Make sure to free your scenes, lock player controls, etc. ect. first before showing the credits, because gui_credits_*() (and everything in <gui_utilities>)
 * can only handle the UI part, and they were designed to do so.
 */
CreditsText *gui_credits_new( const char *text_file, const char *track_file, const char image_file, const Vector3 *image_color, int layer )
{
	if( !text_file )
		return NULL;

	layer = (int) ifelse(layer > 0, layer, 1);

	CreditsText *text         = MALLOC(1, CreditsText);
	text->scroll_speed        = 4.2; // https://en.wikipedia.org/wiki/42_(number)
	text->text_file           = str_create(text_file);
	text->image_file          = (STRING *) sifelse(image_file, str_create(image_file), NULL);
	text->track_file          = (STRING *) sifelse(track_file, str_create(track_file), NULL);
	text->color               = (VECTOR *) vifelse(image_color, image_color, nullvector);
	text->__container         = txt_create(CREDITS_TEXT_MAX_LINES, layer + 1);
	text->__container->font   = font_create("Arial#18");

	txt_load(text->__container, text->text_file); 	/*
													 * Parse the plain text file to the container without taking
													 * Unicode into account. Parsing Unicode can be done through
													 * txt_loadw() but it requires a little more work on the implementation.
													 */

	return text;
}

/*
 * void gui_credits_free( CreditsText *text )
 *
 * Frees a previously allocated credits text object through gui_credits_new().
 */
void gui_credits_free( CreditsText *text )
{
	if(text) {
		if(text->image_file)
			str_remove(text->image_file);
		if(text->track_file)
			str_remove(text->track_file);

		str_remove(text->text_file);

		// Remove all the strings in the credits text container...
		int i = 1;
		while(i <= text->__container->strings)
		{
			if((text->__container->pstring)[i - 1])
				str_remove((text->__container->pstring)[i - 1]);

			i += 1;
			wait(1.0);
		}
		// ...and also remove the container itself.
		txt_remove(text->__container);

		FREE(text);
	}
}

/*
 * void gui_credits_set_font( CreditsText *text, const char *font_file )
 *
 * Changes the display font of a CreditsText object.
 */
void gui_credits_set_font( CreditsText *text, const char *font_file )
{
	if(text && font_file)
		text->__container->font = font_create(font_file);
}

/*
 * void gui_credits_show( CreditsText *text, float volume )
 *
 * Performs rendering of the specified credits text object.
 * After the credits is displayed, the object will be permanently deleted through gui_credits_free().
 * (TODO: Implement keypress to finish the sequence instead of automagic deletion.)
 */
void gui_credits_show( CreditsText *text, float volume )
{
	if(text) {
		volume = ifelse( var_cmp(volume, 0.0) == true, 100.0, volume );

		// This temporary frame (BMAP container) is used to render the background image inside.
		Panel *frame = pan_create(NULL, text->__container->layer - 1);

		// If an image file was supplied before, create the image, pass it to the frame object
		// and stretches the image to fit the screen size. Otherwise, create a black background and
		// fill whatever color that is stored in text->color.
		if(text->image_file)
		{
			frame->bmap = bmap_create(text->image_file);
			gui_panel_set_scale(frame, SCALE_X | SCALE_Y);
		}
		else
		{
			frame->bmap = bmap_createblack(screen_size.x, screen_size.y, 16);
			bmap_fill(frame->bmap, text->color, 100.0);
		}

		if(text->track_file)
			text->track_handle = media_play(text->track_file, NULL, volume);

		frame->flags |= (SHOW);
		text->__container->flags |= (SHOW);

		float h = txt_height(text->__container) + text->__container->pos_y;
		while(text->__container->pos_y > -h) // Inverse the height so that we can scroll the text upwards.
		{
			text->__container->pos_y -= text->scroll_speed * time_step;
			wait(1.0);
		}

		if(media_playing(text->track_handle))
			media_stop(text->track_handle);

		frame->flags                &= ~(SHOW);
		text->__container->flags    &= ~(SHOW);

		pan_remove(frame);
		gui_credits_free(text);
	}
}

void gui_credits_set_pos( CreditsText *text, float x, float y )
{
	if(text)
		gui_text_set_pos(text->__container, x, y);
}

void gui_credits_set_pos( CreditsText *text, Vector3 *pos )
{
	gui_credits_set_pos(text, pos->x, pos->y);
}

void gui_credits_set_speed( CreditsText *text, float speed )
{
	if(text)
		text->scroll_speed = fifelse(speed > 0.0, speed, 4.2);
}

/*
 * void gui_open_eyes(float speed, int layer)
 *
 * Simulates a very simple eyes opening effect.
 */
void gui_open_eyes(float speed, int layer)
{
	#define    HALVES    2

	speed =        ifelse(speed > 0.0, speed, 3.5);
	layer = (int)  ifelse(layer > 0, layer, 1);
	PANEL *halves[HALVES];

	int i = 0;
	for(; i < HALVES; i++)
	{
		halves[i]               = pan_create(NULL, layer);
		(halves[i])->bmap       = bmap_createblack(screen_size.x, screen_size.y * 0.5, 8);
		(halves[i])->flags     |= SHOW;

		if(i == 1)
			(halves[i])->pos_y = screen_size.y * 0.5;
	}

	var dy = -bmap_height((halves[0])->bmap);

	while((halves[0])->pos_y > dy) {
		(halves[0])->pos_y -= speed * time_step;
		(halves[1])->pos_y += speed * time_step;

		wait(1.0);
	}

	for(i = 0; i < HALVES; i++)
	{
		bmap_remove((halves[i])->bmap);
		pan_remove(halves[i]);
	}
}

/*
 * void gui_title_set_mode( StaticTitleText *text, const int mode )
 *
 * Changes the disappearance mode of the static text object. Can be FADE_OUT
 * (fades out the whole text, then destroy it) or CHAR_BY_CHAR (destroys the text character-by-character).
 */
void gui_title_set_mode( StaticTitleText *text, const int mode )
{
	if(text)
		text->fade_mode = mode;
}
