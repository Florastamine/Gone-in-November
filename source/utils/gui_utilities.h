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

	Pair *gui_image_get_dimensions( const Bitmap *image );
	Pair *gui_image_get_dimensions( const String *image_file );

	Pair *gui_panel_get_pos( Panel *panel );
	Pair *gui_text_get_pos( Text *text );

	void gui_panel_set_pos( Panel *panel, Pair *pair );
	void gui_panel_set_pos( Panel *panel, float px, float py );

	void gui_text_set_pos( Text *text, Pair *pair );
	void gui_text_set_pos( Text *text, float px, float py );

	Pair *gui_panel_get_size( Panel *panel );

	void gui_panel_set_size( Panel *panel, float sx, float sy );

	void gui_panel_set_align( Panel *panel, const int mode );
	void gui_panel_set_scale( Panel *panel, const int __flags );
	void gui_panel_set_center( Panel *panel, const int mode );

	float gui_panel_get_rotation( Panel *panel );
	void  gui_panel_set_rotation( __Out Panel *panel, float amount );
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

	GUIButton *gui_button_new( Vector2 *pos,
	                           String *title,
	                           int layern,
	                           String *image_on,
	                           String *image_off,
	                           String *image_over,
	                           const void *fptr_on,
	                           const void *fptr_off,
	                           const void *fptr_over
	);

	void gui_button_free( GUIButton *b );

	void gui_button_set_event( __Out GUIButton *b, int mode, Bitmap *avatar, const void *event );
	const void *gui_button_get_event( GUIButton *b, const int mode  );

	void gui_button_set_font( __Out GUIButton *b, const Font *f );
	Font *gui_button_get_font( GUIButton *b );

	void gui_button_set_pos( __Out GUIButton *b, float x, float y );
	Vector2 *gui_button_get_pos( GUIButton *b );

	void gui_button_set_color( __Out GUIButton *b, Vector3 *color );
	void gui_button_set_color( __Out GUIButton *b, float __red, float __green, float __blue );
	void gui_button_set_text_color( __Out GUIButton *b, Vector3 *color );
	void gui_button_set_text_color( __Out GUIButton *b, float __red, float __green, float __blue );
	void gui_button_reset_color( __Out GUIButton *b );
	void gui_button_reset_text_color( __Out GUIButton *b );

	void gui_button_show( __Out GUIButton *b );
	void gui_button_hide( __Out GUIButton *b );
	bool gui_button_get_invisibility( GUIButton *b );
}

__namespace(StaticText) {
	/*
	 * This StaticText thing is borrowed from Lemming's colored text, whose original code
	 * was posted here: http://www.opserver.de/ubb7/ubbthreads.php?ubb=showflat&Number=423300
	 * but I modified his code slightly for better coding structure, naming convention, also removed lots
	 * of unnecessary code and make it safer to use.
	 */

	typedef struct CLList {
		TEXT          *element;
		float          linex;
		float          liney;
		struct CLList *next;
	} CLList;

	/*
	 * StaticText (struct)
	 * This struct contains everything related to the static text object,
	 * which can be colored using hex color tags. [C=ffffff]
	 */
	typedef struct {
		CLList       *texts;
		FONT         *font;
		float         indent;
		float         sizey;

		float         layer;
		float         posx;
		float         posy;
	} StaticText;

	/*
	 * StaticText *gui_static_text_new(STRING *text, FONT *font, float indent, float size_y)
	 *
	 * Allocates and initializes a new static text object.
	 */
	StaticText *gui_static_text_new(STRING *text, FONT *font, float indent, float size_y);

	/*
	 * void *gui_static_text_free(StaticText* text)
	 *
	 * Removes a static text. The freed text can no longer be rendered or modified again.
	 */
	void *gui_static_text_free(StaticText* text);

	/*
	 * void gui_static_text_render(StaticText* text, float posx, float posy, float alpha, int layer)
	 *
	 * Renders a static text object.
	 * The object must be first created and initialized through gui_static_text_new().
	 */
	void gui_static_text_render(StaticText* text, float posx, float posy, float alpha, int layer);
	void gui_static_text_render(StaticText *text, Vector2 *pos, float alpha, int layer);

	/*
	 * void gui_static_text_hide(StaticText *text)
	 *
	 * Quickly hides a StaticText object.
	 */
	void gui_static_text_hide(StaticText *text);

	/*
	 * float gui_static_text_get_height(StaticText* text)
	 *
	 * Retrieves the height of a specified StaticText object.
	 */
	float gui_static_text_get_height(StaticText* text);

	/*
	 * void hex_parse(STRING *gstr, fixed *vector)
	 *
	 * Parses a two-byte hex color to a var * pointer.
	 *
	 * A modified variant which returns a var instead of out parameters is provided below.
	 */
	void hex_parse(STRING *gstr, fixed *vector);
	fixed hex_parse(STRING *gstr);
}

__namespace(NotifierBox) {
	#define    NOTIFICATION_BOX_MAXIMUM_TRANSLUCENCY    75
	#define    NOTIFICATION_BOX_FADE_IN_SPEED           4.5
	#define    NOTIFICATION_BOX_FADE_OUT_SPEED          8.5
	#define    NOTIFICATION_BOX_TEXT_HIDE_POINT         40
	#define    NOTIFICATION_BOX_VERTICAL_BORDER_SIZE    35.0
	#define    NOTIFICATION_BOX_HORIZONTAL_BORDER_SIZE  35.0

	/*
	 * void gui_notifier_new( StaticText *stext, float pos_x, float pos_y,  float duration, int layer, int destruct )
	 *
	 * Creates a new notification object at the specified layer.
	 * The static text object must be initialized and set up with data before it can be used within gui_notifier_new().
	 */
	void gui_notifier_new( __In StaticText *stext, __In float pos_x, __In float pos_y,  __In float duration, __In int layer, __In int destruct );
}

__namespace(ProgressBar) {
	// TODO: Add support for custom bar size and transparency.

	#define    PROGRESS_BAR_DEFAULT_BAR_SIZE_X 256.0
	#define    PROGRESS_BAR_DEFAULT_BAR_SIZE_Y 10.0

	#define    PROGRESS_BAR_RELATIVE_OFFSET_X  10.0
	#define    PROGRESS_BAR_RELATIVE_OFFSET_Y  10.0

	/*
	 * ProgressBar (struct)
	 * A *very* simple and lightweight progress bar widget implementation.
	 */
	typedef struct {
		Panel *outline;           /* Internal container that contains the outliner of the progress bar. */
		Panel *bar;               /* Internal container that contains the progress bar itself.          */

		float progress;           /* Current progress of the bar, in percent.                           */
		int   layer;              /* Uniform layer for both the outliner and the bar.                   */
	} ProgressBar;

	/*
	 * ProgressBar *gui_pbar_new(  const STRING *outliner_file,  const STRING *bar_file,  float progress,  int layer )
	 *
	 * Allocates and initializes a new progress bar object.
	 */
	ProgressBar *gui_pbar_new( __In const STRING *outliner_file, __In const STRING *bar_file, __In float progress, __In int layer );
	ProgressBar *gui_pbar_new();

	/*
	 * void gui_pbar_free(  ProgressBar *pbar )
	 *
	 * Frees a previously allocated progress bar through gui_pbar_new().
	 */
	void gui_pbar_free( __In ProgressBar *pbar );

	/*
	 * void gui_pbar_update_progress(  ProgressBar *pbar,  float progress )
	 *
	 * Updates the progress bar with a new progress value (in percent).
	 */
	void gui_pbar_update_progress( __In ProgressBar *pbar, __In float progress );

	/*
	 * void gui_pbar_update_pos(  ProgressBar *pbar,  float x,  float y )
	 *
	 * Updates the progress bar with a new position.
	 */
	void gui_pbar_update_pos( __In ProgressBar *pbar, __In float x, __In float y );

	/*
	 * void gui_pbar_update_layer( ProgressBar *pbar, int layer )
	 *
	 * Updates the progress bar with a new layer order.
	 */
	void gui_pbar_update_layer( __In ProgressBar *pbar, __In int layer );

	/*
	 * void gui_pbar_update_color_back( ProgressBar *pbar, VECTOR *color )
	 * void gui_pbar_update_color_front( ProgressBar *pbar, VECTOR *color )
	 *
	 * Updates the front (the actual progress bar) and the back (outliner) color
	 * of a given progress bar.
	 */
	void gui_pbar_update_color_back( __In ProgressBar *pbar, __In VECTOR *color );
	void gui_pbar_update_color_front( __In ProgressBar *pbar, __In VECTOR *color );

	/*
	 * void gui_pbar_render( ProgressBar *pbar )
	 *
	 * Renders a progress bar.
	 */
	void gui_pbar_render( __In ProgressBar *pbar );

	/*
	 * void gui_pbar_hide(  ProgressBar *pbar )
	 *
	 * Hides a progress bar.
	 */
	void gui_pbar_hide( __In ProgressBar *pbar );

	float gui_pbar_get_progress( __In ProgressBar *pbar );
	float gui_pbar_get_translucency( __In ProgressBar *pbar );
	int gui_pbar_get_layer( __In ProgressBar *pbar );
}

__namespace(StaticDialogue) {
	#define    STATIC_DIALOGUE_MAX_DIALOGUES      92

	#define    STATIC_DIALOGUE_AVATAR_OFFSET_X    15.0
	#define    STATIC_DIALOGUE_AVATAR_OFFSET_Y    0.0
	#define    STATIC_DIALOGUE_TEXT_OFFSET_X      15.0
	#define    STATIC_DIALOGUE_TEXT_OFFSET_Y      10.0
	#define    STATIC_DIALOGUE_FADE_SPEED         8.5

	// Immutable, don't modify!
	#define    SOUND_SHOW                         0
	#define    SOUND_HIDE                         1
	#define    SOUND_TRAVERSAL                    2

	typedef struct {
		Text       *__container; /* Internal container which contains the list of scanned, unparsed strings.                                     */
		int         __cursor;    /* Internal counter used in gui_dialogue_parse_next() and gui_dialogue_parse_prev() which acts as a "pointer".  */

		StaticText *string;      /* A static string object which is displayed together with the background/avatar image.                         */

		Sound      *snd_visible;     /* Sound played when the dialogue is shown.                                                                     */
		Sound      *snd_invisible;   /* Sound played when the dialogue is invisible.                                                                 */
		Sound      *snd_traversal;   /* Sound played whenever a dialogue line is shown.                                                              */

		float       volume;

		fixed      __snd_visible_handler;      /* Handlers to the three sound effects mentioned above. You can then use these handlers to, e.g., */
		fixed      __snd_invisible_handler;    /* perform various sound operations on it (snd_tune(), snd_pause(), snd_add(), ...)               */
		fixed      __snd_traversal_handler;

		Panel      *avatar;      /* Pointer to the dialogue object's avatar.                                                                     */
		Panel      *background;  /* Pointer to the dialogue object's background image, can either be a simple bitmap or a cut-off from an image. */

		Font       *font;        /* Pointer to the font used for text rendering.                                                                 */

		float       x;           /* Attributes related to the dialogue object that can simply be retrieved through helper                        */
		float       y;           /* functions or by direct access. It's better not to modify them.                                               */
		float       alpha;
		int         layer;
		int         lines;
	} StaticDialogue;

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
	StaticDialogue *gui_dialogue_new( __In const char *text_file, __In const char *avatar, __In const char *background, __In int layer, __In int unicode, __In float width, __In float height );

	/*
	 * void gui_dialogue_free( StaticDialogue *dialog )
	 *
	 * Frees a previously initialized dialogue object through gui_dialogue_new().
	 */
	void gui_dialogue_free( __In StaticDialogue *dialog );

	/*
	 * void gui_dialogue_update_pos( StaticDialogue *dialog, float x, float y )
	 *
	 * Updates the current position of the dialogue object.
	 * Everything will be automatically updated and adjusted.
	 */
	void gui_dialogue_update_pos( __In __Out StaticDialogue *dialog, __In float x, __In float y );

	/*
	 * void gui_dialogue_update_layer( StaticDialogue *dialog, int layer )
	 *
	 * Updates the dialogue object with a new layer order.
	 */
	void gui_dialogue_update_layer( __In __Out StaticDialogue *dialog, __In int layer );

	/*
	 * void gui_dialogue_update_font( StaticDialogue *dialog, consg Font *font )
	 *
	 * Updates the font used for text rendering with a new one.
	 */
	void gui_dialogue_update_font( __In StaticDialogue *dialog, __In const Font *font );

	/*
	 * void gui_dialogue_update_alpha( StaticDialogue *dialog, float alpha )
	 *
	 * Updates the alpha value of the dialogue image.
	 */
	void gui_dialogue_update_alpha( __In StaticDialogue *dialog, __In float alpha );

	/*
	 * void gui_dialogue_update_sound( StaticDialogue *dialog, int channel, String *snd_file )
	 *
	 * Sets the sound to be played when certain events of the dialogue object is executed.
	 * (visible, invisible and when traversing between dialogue lines).
	 * The second parameter accepts SOUND_SHOW, SOUND_HIDE or SOUND_TRAVERSAL and the third
	 * parameter gives the exact location to the sound file to be played with certain events.
	 *
	 * To adjust the current volume of the sound effects, use gui_dialogue_update_volume().
	 */
	void gui_dialogue_update_sound( __In __Out StaticDialogue *dialog, __In int channel, __In String *snd_file );

	/*
	 * void gui_dialogue_update_volume( StaticDialogue *dialog, float volume )
	 *
	 * Updates the current volume of the dialogue sound effects.
	 */
	void gui_dialogue_update_volume( __In __Out StaticDialogue *dialog, __In float volume );

	/*
	 * void gui_dialogue_update_color( StaticDialogue *dialog, VECTOR *color )
	 *
	 * Replaces the color of the dialogue image with a new one.
	 */
	void gui_dialogue_update_color( __In StaticDialogue *dialog, __In VECTOR *color );

	/*
	 * void gui_dialogue_render( StaticDialogue *dialog )
	 *
	 * Renders a dialogue object.
	 */
	void gui_dialogue_render( __In StaticDialogue *dialog );

	/*
	 * void gui_dialogue_hide( StaticDialogue *dialog )
	 *
	 * Hides a dialogue object.
	 */
	void gui_dialogue_hide( __In StaticDialogue *dialog );

	/*
	 * void gui_dialogue_parse( StaticDialogue *dialog, int pos )
	 *
	 * Parses and renders a given text. Possible range: [1; dialog->lines]
	 * (dialog->lines is the number of dialogue strings scanned from the text file).
	 *
	 * Additional functions used for sequential parsing is provided below. gui_dialogue_parse_next()
	 * automatically destroys the object after the last dialogue line is reached.
	 */
	void gui_dialogue_parse( __In StaticDialogue *dialogue, __In int pos );
	void gui_dialogue_parse_next( __In StaticDialogue *dialog );
	void gui_dialogue_parse_prev( __In StaticDialogue *dialog );

	FONT *gui_dialogue_get_font( __In StaticDialogue *dialog );
	int gui_dialogue_get_lines( __In StaticDialogue *dialog );
	int gui_dialogue_get_layer( __In StaticDialogue *dialog );
	float gui_dialogue_get_alpha( __In StaticDialogue *dialog );
	float gui_dialogue_get_volume( __In StaticDialogue *dialog );
}

#include "gui_utilities.c"
#endif /* gui_utilities.h */
