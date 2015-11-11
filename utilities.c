/* utilities.c */
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
 * void __assert( const char *message )
 * 
 * A very simple assert() implementation.
 * It has to relie on the parameter alone, because Lite-C doesn't have support for __func__
 */
void __assert( const char *message )
{
	char *__string = MALLOC(ASSERT_MESSAGE_LENGTH, char);
	fixed f = file_open_append("lassert.log");
	
	sprintf( __string, "[%i:%i, %i.%i.%i] Assertion failed: %s",
	(int) sys_hours,
	(int) sys_minutes,
	(int) sys_day,
	(int) sys_month,
	(int) sys_year,
	ifelse(message, message, _chr("Undescribed assertion.")) );
	
	file_str_write(f, __string);
	file_asc_write(f, 13);
	file_asc_write(f, 10);
	
	free(__string);
	sys_exit((void *) 0);
}

/*
 * void *calloc(int count, size_t size)
 * 
 * C's calloc() reimplementation on Lite-C.
 * Just realized that calloc() doesn't exist on Lite-C after adding the macro CALLOC(). ;)
 */
void *calloc(int count, size_t size)
{
	size_t __size = count * sizeof(size);
	
	void *ret = malloc(__size);
	memset( ret, 0, __size );
	
	return ret;
}

/*
 * Object *object_sky_create( const String *file, const int layer )
 * 
 * Quick-creates a sky cube without an ugly global struct.
 * (though you can receive the returned object for later modification).
 */
Object *object_sky_create( const String *file, const int layer )
{
	Object *sky = NULL;
	
	if(file)
	{
		sky = ent_createlayer( file, SKY | CUBE | SHOW, layer );
	}
	
	return sky; // Return the sky...
}

/*
 * void swap( int *a, int *b )
 *
 * Swaps two integers. For general-purpose swapping, please
 * refer to the SWAP() macro. (not recommended)
 */
void swap( int *a, int *b )
{
	int c = *a + *b;
	*a = c - *a;
	*b = c - *b;
}

/*
 * void copy( float **i, float **j, int num )
 * 
 * Copies a pointer-based array over another, suppressing boundary checks. Why?
 * Because we don't know the size of the memory area which the pointer is pointing to, at least not in plain C.
 */
void copy( float **i, float **j, int num )
{
	num = (int) ifelse(num > 0, num, 1);
	int k = 0;
	
	while(k < num)
	{
		*(*i + k) = *(*j + k);
		k++;
	}
}

/*
 * Pair *pair_new()
 * 
 * Creates a new, empty pair that you can use for later filling.
 */
Pair *pair_new()
{
	Pair *p = MALLOC(1, Pair);
	p->first  = 0.0;
	p->second = 0.0;
	
	return p;
}

Pair *pair_new( float first, float second )
{
	Pair *p = pair_new();
	p->first = first;
	p->second = second;
	
	return p;
}

Pair *pair_new( const Pair *p )
{
	return pair_new(p->first, p->second);
}

/*
 * void pair_free( Pair *pair )
 * 
 * Frees a pair that was allocated before via pair_new().
 */
void pair_free( Pair *pair )
{
	if(pair) free(pair);
}

/*
 * void pair_set( Pair *p, float first, float second )
 * 
 * Fills a pair with values.
 */
void pair_set( Pair *p, float first, float second )
{
	if(!p) return;
	p->first = first;
	p->second = second;
}

/*
 * void pair_swap( Pair *p1, Pair *p2 )
 * 
 * Swaps two pairs.
 */
void pair_swap( Pair *p1, Pair *p2 )
{
	if( !p1 || !p2 ) return;
	
	// FIXME: It's ugly.
	Pair p3;
	pair_set( &p3, p1->first, p1->second );
	pair_set( p1, p2->first, p2->second );
	pair_set( p2, p3.first, p3.second );
}

/*
 * double log10( double d )
 *
 * Calculates log(10, d).
 * log(10, d) = log(e, x) / log(e, 10) = ln(x) / 2.30258509.
 */
double log10( double d )
{
	return log(d) / 2.30258509;
}

/*
 * bool is_odd( int i )
 * 
 * Returns true if the given number is an odd number, false otherwise.
 */
bool is_odd( int i )
{
	return i & 1;
}

/*
 * Pair *shader_get_version()
 * 
 * Returns the current shader version supported by your graphics card.
 * The first value in the returned pair indicates the pixel shader version supported, 
 * while the second value indicates the vertex shader version.
 */
Pair *shader_get_version()
{
	Pair *p = pair_new();
	int i = d3d_shaderversion;
	int *array = MALLOC(4, int);
	
	split( array, i, 0 );
	
	p->first   = join( *(array + 3), *(array + 2) );
	p->second  = join( *(array + 1), *array );
	
	free(array);
	return p;
}

/*
 * int join( int i, int j )
 * 
 * Joins two integers.
 * Because we only need the integer fraction, a cast to int is required, otherwise serious issues would happen.
 */
long join( int i, int j )
{
	int time = 1;
	
	if( j != 0 ) time = pow( 10.0, (int) log10(j) + 1.0 );
	return (i * time) + j;
}

/*
 * void split(int *__result, int value, int slot)
 *
 * Split a value's digit characters into an integer array.
 * The third parameter controls at which position the digits are
 * inserted into the array. So you can insert the digits
 * to your array which already has data.
 */
void split(int *__result, int value, int slot)
{
	ASSERT(__result, _chr("utilities.c/split(): Uninitialized container provided."));
	
	if( slot <= 0 ) slot = 0;
	int count = 0, digit = 0;
	while(value)
	{
		digit = value % 10;
		value = value / 10;
		count += 1;
		if( __result + slot + (count - 1) ) *(__result + slot + count - 1) = digit;
	}
}

void split(int *__result, int value)
{
	split(__result, value, 0); // Spit the numbers at the beginning of the array by default.
}

/*
 * void reverse(int **array, unsigned int count)
 * 
 * Reverse count elements in the specified array.
 * Because Lite-C doesn't have built-in support for determining
 * a given array's size (sizeof() doesn't work), and to give the 
 * user more control over how many elements can be reversed,
 * an additional second parameter is provided.
 */
void reverse(int **array, unsigned int count)
{
	ASSERT( *array, _chr("utilities.c/reverse(): Uninitialized container provided.") );
	
	int i = 0, j = 0;
	while(i < count / 2)
	{
		swap( *(array + i), *(array + count - 1 - i) );
		i++;
		
		wait(1.0);
	}
}

/*
 * String *str_clip_ex( String *str, int n )
 *
 * An extension to str_clip() which returns the cut string.
 */
String *str_clip_ex( String *str, unsigned int n )
{
	ASSERT(str, _chr("utilities.c/str_clip_ex(): Uninitialized container provided."));
	
	String *temp = str_create(str);
	str_trunc(temp, str_len(str) - n);
	str_clip(str, n);
	
	return str;
}

/*
 * double deg2rad( double d )
 *
 * Converts degrees to radians.
 */
double deg2rad( double d )
{
	return (d * Pi) / 180.0;
}

/*
 * double rad2deg( double d )
 * 
 * Converts radians to degrees.
 */
double rad2deg( double d )
{
	return(d * 180.0) / Pi;
}

/*
 * int search( Text *container, const String *str )
 *
 * A __very__ slow, __unoptimized__ and basic search function
 * which looks for a specified string in a Text container.
 * Suitable for console command dictionary.
 */
int search( Text *container, const String *str )
{
	if( !container || !str ) return 0;
	
	int i = 0;
	int position = 0;
	while(i < container->strings)
	{
		if( *(((container->pstring)[i])->chars) == *(str->chars) ) // First character of both strings match.
		{
			while( *((((container->pstring)[i])->chars) + position) == *( (str->chars) + position ) ) // Keep comparing two strings character-by-character.
			{
				position += 1;
			}
			if(position - 1 == strlen(str->chars)) return (i + 1); // Because we got past the null terminator character, let's subtract the position by 1.
			else position = 0;
		}
		i++;
	}
	return 0;
}

int search( Text *container, const char *cstr )
{
	return search(container, _str(cstr));
}

/*
 * char *function_name_get( const void *f )
 *
 * Retrieve a function's name.
 */
char *function_name_get( const void *f )
{
	char *cstr = NULL;
	if(f)
	{
		if(engine_getscriptinfo(f, &cstr)) strcat(cstr, _chr("()"));
	}
	
	return cstr;
}

/*
 * void window_title_set( const char *title )
 *
 * Sets the window title.
 */
void window_title_set( const char *title )
{
	if(title) video_window( NULL, NULL, 0, title );
}

/*
 * void window_pos_set( Pair *pos )
 * 
 * Sets the window's position.
 * Two variants available taking a Pair struct (see utilities.h) or raw floats.
 */
void window_pos_set( Pair *pos )
{
	if(pos) video_window( vector(pos->first, pos->second, 0), NULL, 0, NULL );
}

void window_pos_set( float posx, float posy )
{
	Pair p;
	p.first = ifelse(posx >= 0, posx, abs(posx));
	p.second = ifelse(posy >= 0, posy, abs(posy));
	window_pos_set(&p);
}

/*
 * void window_color_set( const Vector3 *color )
 *
 * Indirectly sets the window color.
 * Two variants available taking a Vector3 and raw floats.
 * Note that, window color can only be set as long as no raw level_load() calls have been executed.
 */
void window_color_set( const Vector3 *color )
{
	if(color) vec_set(&screen_color, color);
}

void window_color_set( float r, float g, float b )
{
	window_color_set( vector(clamp(r, 0.0, 255.0), clamp(g, 0.0, 255.0), clamp(b, 0.0, 255.0) ) );
}

/*
 * bool ready()
 *
 * Returns true if both the video device and video functions are available.
 * Use this function to determine when you can access window_*() or raw video_*() calls.
 * For example:
 * {
 *   while( !ready() ) wait(1.0); // Wait until video functions and devices are initialized.
 *   window_color_set(255.0, 0.0, 0.0);
 * }
 */
bool ready()
{
	return ifelse(engine_status(0) == 5, true, false);
}

/*
 * void window_size_set( float width, float height )
 *
 * Sets the Acknex window size. 
 * Two available variants taking raw floats or a Pair struct.
 */
void window_size_set( float width, float height )
{
	width = ifelse(width > sys_metrics(0), sys_metrics(0), width);
	height = ifelse(height > sys_metrics(1), sys_metrics(1), height);
	video_set( abs(width), abs(height), 0, 0);
}

void window_size_set( const Pair *__size )
{
	if(__size) window_size_set(__size->first, __size->second);
}

/*
 * void window_aspect_set( float ratio )
 *
 * Sets the Acknex screen aspect ratio.
 * Passing a raw float is always faster than a division (constant (1.333) versus calculation (4/3)).
 * Avoid unnecessary calculations whenever possible.
 *
 * Valid aspects are: (from Wikipedia)
 * 4:3 (1.333) Some (not all) 20th century computer monitors (VGA, XGA, etc.), standard-definition television
 * sqrt(2):1 (1.414): International paper sizes (ISO 216)
 * 3:2 (1.5): 35mm still camera film, iPhone (until iPhone 5) displays
 * 16:10 (1.6): Commonly used widescreen computer displays (WXGA)
 * 1.618: Golden ratio, close to 16:10
 * 16:9 (1.777):  Widescreen TV
 * 160/45 (3.555): An's ratio for 1,60 meters and 45kg weight. 2 times bigger than the golden ratio.
 */
void window_aspect_set( float ratio )
{
	video_aspect = ifelse(ratio, ratio, video_aspect);
}

/*
 * float window_aspect_get()
 *
 * Retrieves the aspect ratio in use.
 */
float window_aspect_get()
{
	return video_aspect;
}

/*
 * void window_mode_set( int mode )
 *
 * Sets the window mode. Available options are (they are defined in utilities.h):
 * FULLSCREEN (1)
 * WINDOWED (2)
 */
void window_mode_set( int mode )
{
	video_switch(0, 0, mode);
}

/*
 * int window_mode_get()
 *
 * Retrieves the currently in use video mode. 
 * Another variant which returns a const char * is provided below,
 * which is useful for intepretation.
 */
int window_mode_get()
{
	return video_screen;
}

const char *window_mode_get()
{
	return ifelse(video_screen == FULLSCREEN, _chr("Fullscreen"), _chr("Windowed"));
}

/*
 * bool object_in_region( Object *object, const String *region )
 *
 * Tests if a specified object lies within a region.
 * Regions must be drawn before in WED and have a name.
 * Returns true if the object lies completely or partially inside the region, false otherwise.
 * For more control over the return result, use the raw region_check() call. It has a separate return value
 * for object partially lying inside the region.
 */
bool object_in_region( Object *object, const String *region )
{
	if( !object || !region ) return false;
	
	Vector3 min, max;
	vec_set( &min, &object->x );
	vec_set( &max, &object->x );
	vec_add( &min, &object->min_x );
	vec_add( &max, &object->max_x );
	
	if(region_check(region, &min, &max)) return true;
	return false;
}

/*
 * void object_scale_set( Object *object, float scale )
 *
 * Uniformly scaling an object's proportion (pro-portion).
 * For doubling the object's scale: object_scale_set(object, object->scale_x * 2)
 */
void object_scale_set( Object *object, float scale )
{
	if(object) object->scale_x = object->scale_y = object->scale_z = scale;
}

/*
 * void object_bound_srhink( Object *object )
 *
 * Shrink to fit the object's bounding box.
 */
void object_bound_srhink( Object *object )
{
	if(object)
	{
		object->frame = 1;
		vec_for_min(&object->min_x, object);
		vec_for_max(&object->max_x, object);
		
		vec_mul(&object->min_x, &object->scale_x);
		vec_mul(&object->max_x, &object->scale_x);
	}
}

__static void __object_bound_get_min( Object *object, Vector3 *vec )
{
	vec_set(vec, vector(object->min_x, object->min_y, object->min_z));
	vec_add(vec, &object->x);
	vec_rotate(vec, &object->pan);
}

__static void __object_bound_get_max( Object *object, Vector3 *vec )
{
	vec_set(vec, vector(object->max_x, object->max_y, object->max_z));
	vec_add(vec, &object->x);
	vec_rotate(vec, &object->pan);
}

/*
 * void object_bound_get( Object *object, Vector3 *min, Vector3 *max )
 *
 * Retrieves the upper and lower bounds of a object's bounding box and pass it to
 * the out parameters.
 * Retrieving individual bounds is possible, simply pass NULL to the remaining argument.
 */
void object_bound_get( Object *object, Vector3 *min, Vector3 *max )
{
	if(object)
	{
		c_setminmax(object);
		
		if(min) __object_bound_get_min(object, min);
		if(max) __object_bound_get_max(object, max);
	}
}

/*
 * void object_bound_draw( Object *object, const Vector3 *color )
 * 
 * Drawing the object's bounding box in a single frame.
 * Continuously bounding box drawing is expensive and thus should be avoided whenever possible.
 * For continuously drawing, place the function call in a while(..) loop
 * while(object) // While I'm alive
 * {
 *   object_bound_draw(object, COLOR_YELLOW);
 *   wait(1.0); // Temporarily defer to allow concurrency.
 * }
 *
 * Another variant which omits the second parameter drawing the object with COLOR_WHITE.
 */
void object_bound_draw( Object *object, const Vector3 *color )
{
	if(object)
	{
		Vector3 a, b;
		object_bound_get(object, &a, &b);
		
		draw_box3d(&a, &b, color, 100.0);
	}
}

void object_bound_draw( Object *object )
{
	object_bound_draw(object, COLOR_WHITE);
}

/*
 * void object_place( Object *object, float modifier )
 *
 * Places the object on a ground surface (terrain entity or blocks), mainly
 * to avoid tedious object placement in WED, where you have to manually place
 * them.
 * The second parameter is for soft adjusting.
 * Set to 0 for default position (the exact result returned by c_trace()).
 */
void object_place( Object *object, float modifier )
{
	if(object)
	{
		float distance = c_trace(&object->x, vector(object->x, object->y, object->z - 100000.0), IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS);
		object->z = distance + modifier;
	}
}

__static void __console_setup( Channel **console )
{	
	(*console) = (Channel *) malloc( sizeof(Channel) + sizeof(Panel) + sizeof(Bitmap) + (2 * sizeof(Text)) );
	(*console)->__background_container = pan_create(NULL, 42);
	(*console)->data = txt_create(1, 42 + 1);
	(*console)->data->font = font_create(Arial_14);
	(*console)->history = txt_create(CONSOLE_CONTAINER_CAPACITY, 42 + 1);
	
	((*console)->data->pstring)[0] = str_create_ex(CONSOLE_COMMAND_MAX_LENGTH);
	int i = 0;
	while(i < CONSOLE_CONTAINER_CAPACITY)
	{
		((*console)->history->pstring)[i] = str_create_ex(CONSOLE_COMMAND_MAX_LENGTH);
		
		i++;
	}
	
	(*console)->ready = true;
}

/*
 * void console_background_set( Channel *console, const String *file )
 * 
 * Sets the console object's background image. Omitting the first parameter makes the bitmap created for the
 * currently bound console object (Channel_active, defined in utilities.h)
 *
 * The background image file must exists, otherwise an assertion is thrown.
 */
void console_background_set( Channel *console, const String *file )
{
	ASSERT( console, _chr("utilities.c/console_background_set(): Invalid console handle."));
	ASSERT( file_exists(file), _chr("utilities.c/console_background_set(): Background image not found.") );
	
	console_background_set(console, bmap_create(file));
}

void console_background_set( Channel *console, const Bitmap *image )
{
	ASSERT( console, _chr("utilities.c/console_background_set(): Invalid console handle."));
	ASSERT( image, _chr("utilities.c/console_background_set(): Invalid bitmap pointer."));
	
	console->background = image;
	console->__background_container->bmap = console->background;
}

void console_background_set( const Bitmap *image )
{
	console_background_set(Channel_active, image);
}

void console_background_set( const String *file )
{
	console_background_set(Channel_active, bmap_create(file));
}

/*
 * void console_bind( Channel *console )
 *
 * Binds the console specified in the first parameter as the active console object.
 */
void console_bind( Channel *console )
{
	if(console) Channel_active = console;
}

/*
 * void console_free( Channel *console )
 *
 * Frees the specified console.
 * Repeatedly freeing and allocating consoles in-game is *NOT* recommended as it can cause unexpected crashes.
 * Consoles should be closed only during resource releasing and exiting the game.
 * Omitting the first parameter will get the active console object freed instead.
 */
void console_free( Channel *console )
{
	if(console)
	{
		txt_remove_ex(console->data);
		txt_remove_ex(console->history);
		console->data = NULL;
		console->history = NULL;
		
		ptr_remove(console->background);
		ptr_remove(console->__background_container);
		console->background = NULL;
		console->__background_container = NULL;
		
		free(console);
	}
}

void console_free()
{
	console_free(Channel_active);
}

/*
 * void console_new( Channel **console )
 * 
 * Initializes a new console object.
 * Note that initializing the console through console_new() will bind the console object
 * to active during initialization, whereas initializing a specific console object WON'T
 * bind it to active. This is to make sure you can initialize many console objects
 * simultaneously without altering the active console object.
 */
void console_new( Channel **console )
{
	__console_setup( console );
}

void console_new()
{
	Channel_active = Channel_singleton;
	console_new( &Channel_active );
}

/*
 * Bitmap *console_background_get( Channel *console )
 *
 * Retrieves the console object's background.
 * Another overloaded version can be used to obtain the currently bound console object's bitmap.
 */
Bitmap *console_background_get( Channel *console )
{
	return ifelse(console && console->background, console->background, NULL);
}

Bitmap *console_background_get()
{
	return console_background_get(Channel_active);
}

__static void __console_show( Channel *console )
{
	if( !console ) return;
	
	if( !(console->__background_container->flags & SHOW) ) console->__background_container->flags |= (SHOW);
	if( !(console->data->flags & SHOW) ) console->data->flags |= (SHOW);
}

/*
 * void console_show()
 *
 * Shows the currently active console object.
 * The rule is, you can create as many console objects as you want, 
 * but only one of them is bound and used at a time. 
 * For this, a variant for showing/hiding console objects which aren't in bound
 * is not provided. You can, however, invoke __console_show() with a parameter
 * specifying the console object you want to show/hide. This is not recommended.
 */
void console_show()
{
	__console_show(Channel_active);
}

__static void __console_hide( Channel *console )
{
	if(inkey_active) inkey_active = 0;
	
	if( console->__background_container->flags & SHOW ) console->__background_container->flags &= (~SHOW);
	if( console->data->flags & SHOW ) console->data->flags &= (~SHOW);
}

void console_hide()
{
	__console_hide(Channel_active);
}

__static bool __console_state_get_invisibility( Channel *console )
{
	return ifelse( (console->__background_container->flags & SHOW) && (console->data->flags & SHOW) , true, false);
}

/*
 * bool console_state_get_invisibility()
 *
 * Returns the invisibility state of the currently bound console object.
 */
bool console_state_get_invisibility()
{
	return __console_state_get_invisibility(Channel_active);
}

/*
 * bool console_state_get_ready( Channel *console )
 * 
 * Returns true if the specified console object is ready to use, false otherwise.
 * A ready console is a fully initialized console through console_new().
 */
bool console_state_get_ready( Channel *console )
{
	return ifelse(console && console->ready, true, false);
}

bool console_state_get_ready()
{
	return Channel_active->ready;
}

/*
 * void console_update()
 *
 * Updates the console for one frame.
 * This function updates the currently bound console object for receiving input.
 */
void console_update()
{
	if( Channel_active )
	{
		if( console_state_get_ready() && console_state_get_invisibility() )
		{
			Channel_active->data->pos_x = Channel_active->__background_container->pos_x;
			Channel_active->data->pos_y = (Channel_active->__background_container->pos_y + bmap_height(Channel_active->background));
			
			while( inkey((Channel_active->data->pstring)[0]) != KEY_ENTER ) wait(1.0);
			
			command_table_call( _chr((Channel_active->data->pstring)[0]) );
			str_init((Channel_active->data->pstring)[0]);
		}
	}
}

/*
 * void command_table_new()
 *
 * Initializes the command table.
 * Command tables are always global, have no instances (because apparently wrapping them into structures and creating singletons cause
 * Lite-C to crash, whereas a similar test in C/GCC 4.9.2 produces no errors whatsoever).
 * During exit, command_table_free() must be summoned to release the command table.
 */
void command_table_new()
{
	__CommandTable_pointer = 0;
	__CommandTable_command = txt_create(CONSOLE_CONTAINER_CAPACITY, 0);
	const char *cstr = MALLOC(CONSOLE_COMMAND_MAX_LENGTH, char);
	
	int i = 0;
	for(; i < CONSOLE_CONTAINER_CAPACITY; i++)
	{
		__CommandTable_action[i] = NULL;
		(__CommandTable_command->pstring)[i] = str_create_ex(CONSOLE_COMMAND_MAX_LENGTH);
	}
	
	free(cstr);
}

/*
 * void command_table_add( const char *command, const void *func )
 *
 * Adds a command to the command table.
 * The passed function must have "void" return type and have no parameters whatsoever, 
 * given consoles are mostly for retrieving and setting states, not executing complicated commands, 
 * and the devs should have control over them.
 */
void command_table_add( const char *command, const void *func )
{
	if( func && command && (__CommandTable_pointer <= CONSOLE_CONTAINER_CAPACITY - 1) )
	{
		str_cpy((__CommandTable_command->pstring)[__CommandTable_pointer], command);
		__CommandTable_action[__CommandTable_pointer] = func;
		__CommandTable_pointer += 1;
	}
}

/*
 * void command_table_remove( int id )
 *
 * Removes a command from the command table.
 * A more convenient variant (and essential for console command processing) is provided below
 * and take advantage of the search() function provided in utilities.h.
 * 
 * Console objects use the second variant to process and call commands with command_table_call() (see below).
 */
void command_table_remove( int id )
{
	if( (int) clamp(id, 0, CONSOLE_CONTAINER_CAPACITY) == id && __CommandTable_action[id] )
	{
		// No need to remove the associated string because it'll be overwritten with a new command_table_add() call.
		__CommandTable_action[id] = NULL;
	}
}

void command_table_remove( const char *command )
{
	int i = search(__CommandTable_command, command);
	if(i) command_table_remove(i - 1);
}

/*
 * void command_table_call( const char *command )
 *
 * Executes a command associated with the id-th string in the command table.
 * Another variant taking a const char * is provided below, which eventually
 * resolving to the original variant.
 */
void command_table_call( int id )
{
	if( (int) clamp(id, 0, CONSOLE_CONTAINER_CAPACITY) == id && __CommandTable_action[id] ) // == has a higher precedence than &&
	{
		void f();
		f = __CommandTable_action[id];
		if(f) f();
	}
}

void command_table_call( const char *command )
{
	int i = search(__CommandTable_command, command);
	if(i) command_table_call(i - 1);
}

/*
 * void command_table_free()
 *
 * Command tables are not recommended to be freed in-game, as for the notes with console_free().
 * (Ctrl-F for console_free() command section and have a quick look).
 */
void command_table_free()
{
	int i = 0;
	for(; i <= CONSOLE_CONTAINER_CAPACITY - 1; i++)
	{
		__CommandTable_action[i] = NULL;
		str_remove( (__CommandTable_command->pstring)[i] );
	}
	
	txt_remove(__CommandTable_command);
}

/*
 * String *str_create_ex( const int length )
 * 
 * An extension to str_create() which is more flexible (but slower).
 * It allows one to explicitly provide the string length in actual integers 
 * instead of fixed strings. With this, you can create lots of strings
 * with different lengths without tinkering with the dirty # symbol.
 */
String *str_create_ex( const int length )
{
	char *cstr = MALLOC(length, char);
	
	*cstr = ' ';
	int i = 0;
	while(i < length)
	{
		*(cstr + i) = ' ';
		i++;
	}
	*(cstr + length - 1) = '\0';
	
	String *gstr = str_create(cstr);
	free(cstr);
	
	return gstr; // Otherwise memory leak from not freeing cstr.
}

/*
 * void str_init( String *gstr )
 *
 * Initializes a given string with blank spaces.
 * This is safer (but slower) than str_cpy() as it doesn't
 * mess with the string length.
 * 
 * console_update() uses this function for clearing the stdin buffer 
 * (because the original method which involves tinkering around with the str_*() family 
 * of functions doesn't work - inkey() treats the blank spaces produced with str_cat() as
 * characters - not spaces.).
 * 
 * So I messed with String's internals - it is merely a char * wrapper after all, and
 * fill the char * array with the assignment operator instead of strcat(), and it worked -
 * although it was still blank spaces, inkey() works correctly now - it detects the string 
 * as a blank string.
 * 
 * So if you want to **really** initialize an empty string, use this. It is a little bit slow, but it works.
 */
void str_init( String *gstr )
{
	if(gstr)
	{
		int i = 0;
		*(gstr->chars) = ' ';
		while(i < gstr->length - 1)
		{
			*((gstr->chars) + i) = ' ';
			i++;
		}
		*((gstr->chars) + gstr->length - 1) = '\0';
	}
}

/*
 * void str_init( char *cstr )
 *
 * Initializes a char * array with blank spaces.
 * This is safer (but slower) than strcpy() as it doesn't
 * mess with the string length.
 */
void str_init( char *cstr )
{
	if(cstr)
	{
		int i = 0;
		*cstr = ' ';
		while(i < strlen(cstr) - 1)
		{
			*(cstr + i) = ' ';
			i++;
		}
		*(cstr + strlen(cstr) - 1) = '\0';
	}
}

/*
 * void txt_remove_ex( Text *object )
 *
 * An extension to txt_remove() which removes all strings associated with the text object.
 * Not sure if the developers missed this or it is intentional.
 */
void txt_remove_ex( Text *object )
{
	if(object)
	{	
		if(object->strings)
		{
			int i = 1;
			for(; i <= object->strings; i++) str_remove( (object->pstring)[i - 1] );
		}
		txt_remove(object);
	}
}

__static void __error_setup( Error **table )
{
	(*table) = (Error *) malloc(sizeof(int) * (ERROR_CONTAINER_CAPACITY + ERROR_HISTORY_CAPACITY + 2));
	(*table)->pos = 0;
	(*table)->count = 0;
	(*table)->message = txt_create(ERROR_HISTORY_CAPACITY, 0);
	
	int i = 0;
	while(i < ERROR_HISTORY_CAPACITY)
	{
		((*table)->message->pstring)[i] = str_create_ex(ERROR_HISTORY_MESSAGE_LENGTH);
		
		i++;
	}
}

/*
 * void error_bind( Error *table )
 * 
 * Binds the error object specified in the first parameter as the active error object.
 */
void error_bind( Error *table )
{
	if(table) Error_active = table;
}

/*
 * void error_new( Error **table )
 * 
 * Creates a new error object. As with console_new() (explained before),
 * this function is just for initializating error objects, not binding them to active. 
 * Calling error_new() without parameters cause the default error table object to be invoked, initialized, and bound.
 * As a general rule, you should have only one error table during the whole program execution flow.
 */
void error_new( Error **table )
{
	__error_setup( table );
}

void error_new()
{
	Error_active = Error_singleton;
	__error_setup( &Error_active );
}

/*
 * void error_code_push( Error *table, const unsigned int code )
 * 
 * Pushes an error code to the error table. For pushing into the currently bound error object, 
 * omit the first parameter.
 */
void error_code_push( Error *table, const unsigned int code )
{
	if(table && table->count < ERROR_CONTAINER_CAPACITY)
	{
		(table->stack)[table->count] = code;
		table->count += 1;
	}
}

void error_code_push( const unsigned int code )
{
	error_code_push(Error_active, code);
}

/*
 * void error_push( const unsigned int code, const char *message )
 * 
 * Commits an error, including the error code and explanation.
 * It can be retrieved later using error_message_get().
 */
void error_push( const unsigned int code, const char *message )
{
	if(error_history_cursor_get() > error_history_capacity_get()) return;
	
	char *cstr = MALLOC(ERROR_HISTORY_MESSAGE_LENGTH, char);
	sprintf(cstr, "(%i) %s", code, ifelse(message, _chr(message), _chr("NULL")));
	
	(Error_active->history)[Error_active->pos] = code;
	str_cpy((Error_active->message->pstring)[Error_active->pos], cstr);
	Error_active->pos += 1;
	
	free(cstr);
}

/*
 * int error_history_cursor_get()
 * 
 * Returns the currently bound error object's history stack position.
 */
int error_history_cursor_get()
{
	return (Error_active->pos + 1);
}

/*
 * int error_history_capacity_get()
 * 
 * Returns the error object's hard limit on maximum error code pushes. 
 */
int error_history_capacity_get()
{
	return ERROR_CONTAINER_CAPACITY;
}

/*
 * char *error_message_get( int id )
 * 
 * Retrieves the error message associated with the specific position in the history stack.
 */
char *error_message_get( int id )
{
	if( (int) clamp(id, 0, ERROR_CONTAINER_CAPACITY) == id )
	{
		return _chr((Error_active->message->pstring)[id]);
	}
}

/*
 * void error_free( Error *table )
 *
 * Frees the error object.
 * For freeing the currently bound error object, omit the associated parameter.
 */
void error_free( Error *table )
{
	if(table)
	{
		txt_remove_ex(table->message);
		table->message = NULL;
		
		free(table);
	}
}

void error_free()
{
	error_free(Error_active);
}


