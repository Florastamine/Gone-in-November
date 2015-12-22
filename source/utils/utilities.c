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
	char *__string = MALLOC(strlen(message) + ASSERT_MESSAGE_LENGTH, char);
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

	FREE(__string);
	file_close(f);
	sys_exit((void *) 0);
}

/*
 * void *calloc(int count, size_t size)
 *
 * C's calloc() reimplementation on Lite-C.
 * Just realized that calloc() doesn't exist in Lite-C after adding the macro CALLOC(). ;)
 */
void *calloc(int count, size_t size)
{
	size_t __size = count * sizeof(size);

	void *ret = malloc(__size);
	ASSERT( ret, _chr("<utilities>/calloc(): Failed to allocate memory blocks.") );

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
 * char *strstr(char *str1, const char *str2)
 *
 * Returns a pointer to the first occurrence of str2 in str1, or a null pointer if str2 is not part of str1.
 *
 * -- Removed since 0.2.1-alpha --
 */
/*
char *strstr(char *str1, const char *str2)
{
	long index = 0;
	long length = strlen(str2);
	char *start = NULL;

	while(*str1 != '\0')
	{
		if(*str1 == str2[index])
		{
			if(index == 0)
				start = str1;

			index ++;
			if(index == length)
				return start;
		}
		else
			index = 0;

		str1 ++;
	}
	return NULL;
}
*/

/*
 * char *dump(const char *content)
 *
 * Dumps content from a file, or return a malloc-ed string if the file doesn't exist.
 */
char *dump(const char *content)
{
	char *buffer = NULL;

	if(file_exists(content)) /* Found a file, so we dump its content. */
	{
		FILE *f = fopen(content, "rb");
		long ssize = 0;

		fseek(f, 0, SEEK_END);
		ssize = ftell(f);
		rewind(f);

		if( !ssize ) {
			return NULL;
		}

		buffer = MALLOC( ssize, char );
		fread(buffer, 1, ssize, f);
		fclose(f);
	}
	else /* Okay... just allocate a string instead. */
	{
		buffer = MALLOC( strlen(content) + 1, char );
		strcpy(buffer, content);
	}

	return buffer;
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
	if(pair)
	    FREE(pair);
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
 * void pair_set( Pair *p1, Pair *p2 )
 *
 * Fills a pair with another pair.
 */
void pair_set( Pair *p1, Pair *p2 )
{
	if( !p1 || !p2 ) return;

	p1->first = p2->first;
	p1->second = p2->second;
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
 * int sgn( int i )
 *
 * Extracts the sign of a number.
 */
int sgn( int i )
{
	ASSERT( i != 0, _chr("<utilities>/sgn(): Cannot divide by 0.") );

	return i/abs(i);
}

/*
 * bool is_odd( int i )
 *
 * Returns true if the given number is an odd number, false otherwise.
 */
bool is_odd( int i )
{
	return i & 1; // Check the very last digit.
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

	FREE(array);
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

	String *temp   = str_create(str);
	int len        = str_len(str);

	str_clip(str, n);
	str_trunc(temp, len - n);

	return temp;
}

/*
 * String *str_trunc_ex( String *str, unsigned int n )
 *
 * An extension to str_trunc() which returns the cut string.
 */
String *str_trunc_ex( String *str, unsigned int n )
{
	ASSERT(str, _chr("utilities.c/str_trunc_ex(): Uninitialized container provided."));

	String *temp   = str_create(str);
	int len        = str_len(str);

	str_trunc(str, n); // Cuts the original string.
	str_clip(temp, len - n);

	return temp;
}

/*
* String *str_parse_ex( String *to, char *from, int pos, char delimiter )
*
* An extension to <strio>/str_parse() which allows for custom delimiters.
*/
String *str_parse_ex( String *to, char *from, int pos, char delimiter )
{
	if( !__from_delimstr ) // If str_parse_ex() wasn't called from str_parse_delim(), then check for the first parameter.
	    ASSERT( to, _chr("<utilities>/str_parse_ex(): Uninitialized container provided."));

	fixed old_delimiter = _str_separator;
	_str_separator = delimiter;

	String *parsed = str_parse(to, from, pos);
	_str_separator = old_delimiter;

	return parsed;
}

/*
* void str_parse_delim( Text *text, char *content, char delimiter )
*
* Breaks a string into substrings separated by the delimiter character given in
* delimiter, and pushes the result substrings into the text object.
*/
void str_parse_delim( Text *text, char *content, char delimiter )
{
	ASSERT( text, _chr("<utilities>/str_parse_delim(): Uninitialized container provided."));
	__from_delimstr = true;

	char old_delimiter = _str_separator;
	_str_separator = delimiter;
	String *parse = str_parse_ex(NULL, content, 1, delimiter ); // Fetchs the very first token.
	txt_addstring(text, parse);                                      // And push it into the text object.

	__from_delimstr = false;
	while(true) // Fetchs the remaining tokens.
	{
		parse = str_parse(parse, content, 0);

		if(parse != NULL)
		    txt_addstring(text, str_trim(parse));
		else
		    break;
	}

	_str_separator = old_delimiter;
}

/*
 * ArrayContainer *array_container_new( int elem )
 * ArrayContainer *array_container_new()
 *
 * Allocates and initializes a new array container.
 */
ArrayContainer *array_container_new( int elem )
{
	ArrayContainer *container = MALLOC(1, ArrayContainer);
	container->count          = ifelse(elem > 0, elem, 1);
	container->data           = MALLOC(container->count, int);
	container->pointer        = 0;

	return container;
}

ArrayContainer *array_container_new()
{
	return array_container_new(1);
}

/*
 * void array_container_free( ArrayContainer *container )
 *
 * Frees the specified array container.
 */
void array_container_free( ArrayContainer *container )
{
	if(container)
	{
		FREE(container->data);
		FREE(container);
	}
}

/*
 * int array_container_get_max_count( ArrayContainer *container )
 *
 * Returns the current maximum size of the array container.
 */
int array_container_get_max_count( ArrayContainer *container )
{
	if(container)
		return container->count;
}

/*
 * int array_container_get( ArrayContainer *container, int pos )
 * int array_container_get_next( ArrayContainer *container )
 * int array_container_get_prev( ArrayContainer *container )
 *
 * Fetchs the array container's data at an arbitrary position, or in a sequential order.
 */
int array_container_get( ArrayContainer *container, int pos )
{
	ASSERT( container, _chr("<utilities>/array_container_get(): Uninitialized array container provided.") );

	if( (int) clamp(pos, 1, container->count) == pos )
		return *(container->data + pos - 1);
}

int array_container_get_next( ArrayContainer *container )
{
	container->pointer += 1;
	return array_container_get( container, container->pointer );
}

int array_container_get_prev( ArrayContainer *container )
{
	container->pointer -= 1;
	return array_container_get( container, container->pointer );
}

/*
 * void num_parse_delim( ArrayContainer *container, char *content, char delimiter )
 *
 * Parses out an array of numbers seperated with delimiters into an array container.
 */
void num_parse_delim( ArrayContainer *container, char *content, char delimiter )
{
	ASSERT( container, _chr("<utilities>/num_parse_delim(): Uninitialized container provided.") );

	if( !delimiter )
		delimiter = ' ';

	Text *local = txt_create(0, 1);
	str_parse_delim(local, content, delimiter);

	container->count = local->strings;
	if(container->data)
		FREE(container->data);

	container->data  = MALLOC(container->count, int);

	int i = 0;
	for(; i < container->count; i++)
		*(container->data + i) = str_to_int((local->pstring)[i]);
}

void num_parse_delim( ArrayContainer *container, char *content )
{
	num_parse_delim(container, content, ' ');
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
 * int search( Text *container, const String *str, int case_sensivity )
 *
 * A __very__ slow, __unoptimized__ and basic search function
 * which looks for a specified string in a Text container.
 * Suitable for console command dictionary.
 */
int search( Text *container, const String *str, int case_sensivity )
{
	if( !container || !str ) return 0;

	str = str_create(str);

	// Buggy and broken, removed since v0.3.0-alpha.
	/*
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
	*/

	int cmp_func(STRING *, STRING *);

	if(case_sensivity > 0)
	    cmp_func = str_cmp;
	else
		cmp_func = str_cmpi;

	int i = 0;
	while(i < container->strings)
	{
		if(cmp_func(str, (container->pstring)[i]))
		    break;
		i++;
	}

	return (int) ifelse(i < container->strings, i + 1, -1);
}

int search( Text *container, const String *str )
{
	return search(container, str, false);
}

/*
 * char *function_name_get( const void *f )
 *
 * Retrieve a function's name.
 */
char *function_name_get( const void *f )
{
	ASSERT( f, _chr("<utilities>/function_name_get(): Invalid function provided."));

	char *cstr = NULL;
	if(engine_getscriptinfo(f, &cstr)) strcat(cstr, _chr("()"));
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
 * char *window_title_get()
 *
 * Retrieves the current title of the engine window.
 */
char *window_title_get()
{
	char *tbuffer = MALLOC(128, char); // 128 is hard limit. Increase to your favor.

	#ifdef    WINDOWS_API
	    GetWindowText(hWnd, tbuffer, 128);
	#endif

	return tbuffer;
}

/*
 * Pair *window_pos_get()
 *
 * Returns the desktop position of the engine window.
 */
Pair *window_pos_get()
{
	Pair *p = pair_new();
	p->first = window_pos.x;
	p->second = window_pos.y;

	return p;
}

/*
 * Vector3 *window_color_get()
 *
 * Returns the current background color of the window in the RGB form.
 */
Vector3 *window_color_get()
{
	return vector(screen_color.blue, screen_color.green, screen_color.red);
}

/*
 * Pair *window_size_get()
 *
 * Return the size of the window.
 */
Pair *window_size_get()
{
	Pair *p = pair_new();
	p->first = screen_size.x;
	p->second = screen_size.y;

	return p;
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
	if(__size)
	    window_size_set(__size->first, __size->second);
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
 * Another variant which omits the second parameter drawing is provided below, which uses COLOR_WHITE.
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
	(*console) = (Channel *) sys_malloc( sizeof(Channel) + sizeof(Panel) + sizeof(Bitmap) + (2 * sizeof(Text)) );
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

		FREE(console);
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

__static Bitmap *__ifelse_bitmap( Bitmap *a, Bitmap *b, Bitmap *c )
{
	if(a)  return b;
	       return c;
}

/*
 * Bitmap *console_background_get( Channel *console )
 *
 * Retrieves the console object's background.
 * Another overloaded version can be used to obtain the currently bound console object's bitmap.
 */
Bitmap *console_background_get( Channel *console )
{
	if(console)
	    return __ifelse_bitmap(console->background, console->background, NULL);

	return NULL;
}

Bitmap *console_background_get()
{
	return console_background_get(Channel_active);
}

__static void __console_show( Channel *console )
{
	if( !console ) return;

	if( !(console->__background_container->flags & SHOW) )
	    console->__background_container->flags |= (SHOW);

	if( !(console->data->flags & SHOW) )
	    console->data->flags |= (SHOW);
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

	if( console->__background_container->flags & SHOW )
	    console->__background_container->flags &= (~SHOW);
	if( console->data->flags & SHOW )
	    console->data->flags &= (~SHOW);
}

/*
 * void console_hide()
 *
 * Hides the currently bound console object.
 */
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
	if(console)
	    return ifelse(console->ready, true, false);

	return NULL;
}

bool console_state_get_ready()
{
	if(Channel_active)
	    return Channel_active->ready;

	return NULL;
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

	FREE(cstr);
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

	if(i)
	    command_table_remove(i - 1);
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
	ASSERT( length, _chr("<utilities>/str_create_ex(): An attempt to create an empty length string."));

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
	FREE(cstr);

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
 * void txt_clear( Text *object )
 *
 * Clears a text object's strings without also removing the text.
 */
void txt_clear( Text *object )
{
	if(object)
	{
		if(object->strings)
		{
			int i = 1;
			for(; i <= object->strings; i++) str_remove( (object->pstring)[i - 1] );
		}
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
	txt_clear(object);
	while(proc_status(txt_clear)) wait(1.0);

	txt_remove(object);
}

/*
 * float txt_height( Text *object )
 *
 * Returns the total height of the text.
 */
float txt_height( Text *object )
{
	float f = 0.0;

	if(object)
	{
		int i = 0;

		for(; i < object->strings; i++)
		    f += *(object->pstring)[i]->pad;
	}

	return object->font->dy * f;
}

/*
 * float txt_width( Text *object )
 *
 * Returns the width of the text object.
 * This is done by returning the longest string in the text.
 */
float txt_width( Text *object )
{
	if(object)
	{
		if(object->strings)
		{
			int   p      = 0;
			int   i      = 1;

			while(i < object->strings)
			{
				if(str_len((object->pstring)[i]) > str_len((object->pstring)[p]))
				p = i;

				i++;
			}

			return str_width((object->pstring)[p], object->font);
		}
	}

	return -1.0;
}

__static void __error_setup( Error **table )
{
	(*table) = (Error *) sys_malloc(sizeof(int) * (ERROR_CONTAINER_CAPACITY + ERROR_HISTORY_CAPACITY + 2));
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
	if(table)
	{
		if(table->count < ERROR_CONTAINER_CAPACITY)
		{
			(table->stack)[table->count] = code;
			table->count += 1;
		}
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

	char *cstr = MALLOC(strlen(message) + ERROR_HISTORY_MESSAGE_LENGTH, char);
	sprintf(cstr, "(%i) %s", code, ifelse(message, _chr(message), _chr("NULL")));

	(Error_active->history)[Error_active->pos] = code;
	str_cpy((Error_active->message->pstring)[Error_active->pos], cstr);
	Error_active->pos += 1;

	FREE(cstr);
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

		FREE(table);
	}
}

void error_free()
{
	error_free(Error_active);
}

/*
 * Vector *bgr_to_rgb(Vector *rgb)
 * void    bgr_to_rgb(Vector *rgb)
 *
 * Returns a RGB color vector from a BGR one.
 */
Vector3 *bgr_to_rgb(Vector3 *rgb)
{
	Vector3 *bgr = NULL;

	if(rgb) {
		rgb = MALLOC(1, Vector3);
		vec_set( bgr, vector(rgb->z, rgb->y, rgb->x) );
	}

	return bgr;
}

void bgr_to_rgb(Vector3 *rgb)
{
	if(rgb) swap( &rgb->x, &rgb->z );
}

/*
 * float clampf(float x, float a, float b)
 *
 * A "fall" variant of clamp() - that is, if x is greater than b,
 * returns a instead of "locking" x to b.
 */
float clampf(float x, float a, float b)
{
	if(x >= b || x <= a)
	    x = a;
	return x;
}

/*
 * char *os_get_name()
 *
 * Retrieves the current OS in use.
 */
char *os_get_name()
{
	char *cstr = (char *) sys_malloc(32); // 32 is more than enough.

	switch( sys_winversion )
	{
		case    1:    strcpy(cstr, "Windows 98 SE");                break;
		case    2:    strcpy(cstr, "Windows ME");                   break;
		case    3:    strcpy(cstr, "Windows 2000");                 break;
		case    4:    strcpy(cstr, "Windows 2003");                 break;
		case    5:    strcpy(cstr, "Windows XP");                   break;
		case    6:    strcpy(cstr, "Windows Vista/7/8/8.1/10");     break;
		default  :    strcpy(cstr, "Unidentified OS");
	}

	return cstr;
}

/*
 * char *file_get_ext ( const char *fn )
 *
 * Fetchs the extension of a given file name.
 */
char *file_get_ext ( const char *fn )
{
	int end = strlen(fn);
	int right = 0;
	char c = 0;

	while(c != '.')
	{
		c = *( (fn + end - 1) - right );
		right += 1;
	}

	if(c != '.') return NULL;

	int left     = end - right + 1; // An additional +1 is reserved for the dot character.
	char *cstr   = MALLOC(right - 1, char);
	int i        = 0;

	while(left <= end)
	{
		*(cstr + i) = *(fn + left);
		i++;
		left++;
	}

	return cstr;
}

__static void __entrluc(PANEL *p)
{
	if( p->flags & ~TRANSLUCENT )
	    p->flags |= (TRANSLUCENT);
}

__static void __fade_in( PANEL *p, float f, float t, float s )
{
	__entrluc(p);

	p->alpha = f;
	while(p->alpha < t) {
		p->alpha += s * time_step;

		wait(1.0);
	}
}

__static void __fade_out( PANEL *p, float f, float t, float s )
{
	__entrluc(p);

	p->alpha = f;
	while(p->alpha > t) {
		p->alpha -= s * time_step;

		wait(1.0);
	}
}

/*
 * void fade( PANEL *p, float a, float b, float s )
 *
 * Performs fading in/out.
 */
void fade( PANEL *p, float a, float b, float s )
{
	if( a == b || !p ) return;

	if(a < b)
	    __fade_in(p, a, b, s);
	if(a > b)
	    __fade_out(p, a, b, s);
}

/*
 * void libc_init()
 *
 * "Links" the vanilla C string functions.
 * Because of this, and because I've grown tired of reinventing the wheel,
 * strstr() and all others previously functions that was put into <utilities>
 * are now removed from the library.
 */
void libc_init()
{
	if( ___libc_init__done__ ) return;

	strstr    = DefineApi("msvcrt!strstr");
	strchr    = DefineApi("msvcrt!strchr");
	strcspn   = DefineApi("msvcrt!strcspn");
	strncat   = DefineApi("msvcrt!strncat");
	strncmp   = DefineApi("msvcrt!strncmp");
	strncpy   = DefineApi("msvcrt!strncpy");
	strcpy_s  = DefineApi("msvcrt!strcpy_s");
	strpbrk   = DefineApi("msvcrt!strpbrk");
	strrchr   = DefineApi("msvcrt!strrchr");
	strspn    = DefineApi("msvcrt!strspn");
	strtok    = DefineApi("msvcrt!strtok");
	strxfrm   = DefineApi("msvcrt!strxfrm");
	snprintf  = DefineApi("msvcrt!snprintf");

	atof      = DefineApi("msvcrt!atof");
	atoi      = DefineApi("msvcrt!atoi");
	atol      = DefineApi("msvcrt!atol");

	_tempnam = DefineApi("msvcrt!_tempnam");
	_access_s = DefineApi("msvcrt!_access_s");
	abort    = DefineApi("msvcrt!abort");
	_chmod   = DefineApi("msvcrt!_chmod");
	_creat   = DefineApi("msvcrt!_creat");
	_execl   = DefineApi("msvcrt!_execl");
	_getcwd  = DefineApi("msvcrt!_getcwd");
	getenv   = DefineApi("msvcrt!getenv");
	_strset  = DefineApi("msvcrt!_strset");
	_strrev  = DefineApi("msvcrt!_strrev");

	___libc_init__done__ = 1;
}

/*
 * bkpt(), bkptend().
 * Small functions to avoid lots of printf() when debugging/tracing for errors in the code.
 * bkptend() resets the counter.
 */
void bkpt()
{
	__bkptcnt__ += 1;
	printf("Pass %i", __bkptcnt__);
}

void bkptend()
{
	__bkptcnt__ = 0;
}

/*
 * char *os_get_user_name()
 *
 * Gets the current user name in the system.
 */
char *os_get_user_name()
{
	// http://stackoverflow.com/questions/704891/windows-username-maximum-length
	// No one cares because I've never seen a 192-character user name.
	DWORD buf_len = 192;
	char *usrlen = (char *) sys_malloc((long) buf_len);

	#ifdef    WINDOWS_API
		GetUserName(usrlen, &buf_len);
	#endif

	return usrlen;
}

/*
 * char *os_get_computer_name()
 *
 * Gets the computer name.
 */
char *os_get_computer_name()
{
	DWORD buf_len = 192;
	char *complen = (char *) sys_malloc((long) buf_len);

	#ifdef    WINDOWS_API
		GetComputerName(complen, &buf_len);
	#endif

	return complen;
}

__static char *__ifelse_chr(char *a, char *b, char *c)
{
	if(a)    return b;
	         return c;
}

/*
 * char *os_get_system_directory()
 *
 * Retrieves the system directory, based on Windows's architecture (SysWOW64 if x64 Windows version, System32 otherwise).
 */
char *os_get_system_directory()
{
	DWORD buf_len = 256;
	char *dir = (char *) sys_malloc((long) buf_len);

	#ifdef    WINDOWS_API
		if( !___libc_init__done__ )
		    libc_init(); // We need getenv().

		GetWindowsDirectory(dir, &buf_len);
		strcat(dir, __ifelse_chr(getenv("PROGRAMFILES(X86)"), _chr("\\SysWOW64\\"), _chr("\\System32\\")));
	#endif

	return dir;
}

/*
 * BOOL os_is_privileged()
 *
 * Test if the executable was executed with administrator rights.
 */
BOOL os_is_privileged()
{
	#ifdef    WINDOWS_API
	    STRING *s   = "";
		fixed hndl  = 0;

		str_cpy(s, os_get_system_directory());
		str_cat(s, "rainbow.rws");
		hndl = file_open_write(s);
		str_remove(s);

		if( hndl )
		    return true;
		return false;
	#endif

	return false;
}

/*
 * Text *os_get_drives()
 *
 * Returns the available drives on the system, in the form
 * of a Text * object.
 *
 * A note on GetLogicalDriveStrings(): The retrieved buffer is in the form:
 * A:\<n>B:\<n>C:\<n>
 * Therefore when printing the raw buffer you only get the very first drive
 * (because printf() ends if it encounters the null terminated character).
 *
 * MAX_PATH = Maximum filename length in NTFS
 */
Text *os_get_drives()
{
	Text   *object = txt_create(0, 1);

	#ifdef    WINDOWS_API
		DWORD   size = MAX_PATH;
		char    drives[MAX_PATH];
		DWORD   wresult = GetLogicalDriveStrings(size, drives);

		if((int) clamp(wresult, 1, MAX_PATH) == wresult)
		{
			char *szSingleDrive = drives;
			while( *szSingleDrive )
			{
				txt_addstring(object, szSingleDrive);
				szSingleDrive += strlen(szSingleDrive) + 1;
			}
		}
	#endif

	return object;
}
