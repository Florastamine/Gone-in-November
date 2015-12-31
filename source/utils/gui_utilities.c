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

	_tpos.first     = (b->position->first + bmap_width( b->__container->bmap ) * 0.5 ) - 1/4;
	_tpos.second    = (b->position->second + bmap_height( b->__container->bmap ) * 0.5 ) - 1/4;

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
 * const void *gui_button_get_event( GUIButton *b, const int mode )
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

float gui_static_text_get_width(StaticText *text)
{
	if( !text )
		return 0.0;

	CLList *current = text->texts;
	float width = 0.0;
	float old_width = txt_width(current->element);

	while(current != NULL)
	{
		width = txt_width(current->element);
		if(width > old_width)
			old_width = width;

		current = current->next;
	}

	return old_width;
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
 * void gui_notifier_new( StaticText *stext, float pos_x, float pos_y,  float duration, int layer )
 *
 * Creates a new notification object at the specified layer.
 * The static text object must be initialized and set up with data before it can be used within gui_notifier_new().
 *
 * Data about the old position and the layer of the StaicText object is ignored. Thus, when invoking
 * gui_notifier_new(), you'll have to pass the new position and the layer on which the object will be rendered.
 */
void gui_notifier_new( StaticText *stext, float pos_x, float pos_y,  float duration, int layer )
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
			gui_static_text_hide(stext);
			hid = 1;
		}
		wait(1.0);
	}
}
