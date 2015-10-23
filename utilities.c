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
	// TODO: Transform 384 into some sort of constant. 
	// TODO: Handle NULL argument.
	char *__string = MALLOC(384, char);
	fixed f = file_open_append("lassert.log");
	
	sprintf( __string, "[%i:%i, %i:%i:%i] Assertion failed: %s",
	(int) sys_hours,
	(int) sys_minutes,
	(int) sys_day,
	(int) sys_month,
	(int) sys_year,
	message );
	
	file_str_write(f, __string);
	file_asc_write(f, 13);
	file_asc_write(f, 10);
	
	free(__string);
	sys_exit((void *) 0);
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
	// TODO: Handle NULL.
	if( !__result ) return;
	
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
 * void reverse(int *array, int count)
 * 
 * Reverse count elements in the specified array.
 * Because Lite-C doesn't have built-in support for determining
 * a given array's size (sizeof() doesn't work), and to give the 
 * user more control over how many elements can be reversed,
 * an additional second parameter is provided.
 */
void reverse(int *array, int count)
{
	// TODO: Handle NULL.
	if( !array || !count ) return;
	
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
String *str_clip_ex( String *str, int n )
{
	// TODO: Handle NULL.
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

void window_title_set( const char *title )
{
	if(title) video_window( NULL, NULL, 0, title );
}

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

void window_color_set( const Vector3 *color )
{
	if(color) vec_set(&screen_color, color);
}

void window_color_set( float r, float g, float b )
{
	window_color_set( vector(clamp(r, 0.0, 255.0), clamp(g, 0.0, 255.0), clamp(b, 0.0, 255.0) ) );
}

bool ready()
{
	return ifelse(engine_status(0) == 5, true, false);
}

void window_size_set( float width, float height )
{
	// TODO: ASSERT()	
	width = ifelse(width > sys_metrics(0), sys_metrics(0), width);
	height = ifelse(height > sys_metrics(1), sys_metrics(1), height);
	video_set(width, height, 0, 0);
}

void window_size_set( const Pair *__size )
{
	if(__size) window_size_set(__size->first, __size->second);
}

void window_aspect_set( float ratio )
{
	video_aspect = ifelse(ratio, ratio, video_aspect);
}

float window_aspect_get()
{
	return video_aspect;
}

void window_mode_set( int mode )
{
	video_switch(0, 0, mode);
}

int window_mode_get()
{
	return video_screen;
}

const char *window_mode_get()
{
	return ifelse(video_screen == FULLSCREEN, _chr("Fullscreen"), _chr("Windowed"));
}

void object_scale_set( Object *object, float scale )
{
	if(object) object->scale_x = object->scale_y = object->scale_z = scale;
}

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

void object_bound_get( Object *object, Vector3 *min, Vector3 *max )
{
	if(object)
	{
		c_setminmax(object);
		
		if(min) __object_bound_get_min(object, min);
		if(max) __object_bound_get_max(object, max);
	}
}

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
	(*console)->history = txt_create(CONSOLE_CONTAINER_CAPACITY, 42 + 1);
}

void console_background_set( __In Channel *console, __In const String *file )
{
	// TODO: Handle NULL
	console_background_set(console, bmap_create(file));
}

void console_background_set( __In Channel *console, __In const Bitmap *image )
{
	// TODO: Handle NULL
	console->background = image;
	console->__background_container->bmap = console->background;
}

void console_background_set( __In const Bitmap *image )
{
	console_background_set(Channel_active, image);
}

void console_background_set( __In const String *file )
{
	console_background_set(Channel_active, bmap_create(file));
}

void console_new( __In Channel **console )
{
	// TODO: Handle NULL
	__console_setup( console );
}

void console_new()
{
	Channel_active = Channel_singleton;
	console_new( &Channel_active );
}

Bitmap *console_background_get( __In Channel *console )
{
	return ifelse(console && console->background, console->background, NULL);
}

Bitmap *console_background_get()
{
	return console_background_get(Channel_active);
}

void console_show( __In Channel *console )
{
	// TODO: Handle NULL
	if( !(console->__background_container->flags & SHOW) ) console->__background_container->flags |= (SHOW);
}

void console_show()
{
	console_show(Channel_active);
}

void console_hide( __In Channel *console )
{
	if( console->__background_container->flags & SHOW ) console->__background_container->flags &= (~SHOW);
}

void console_hide()
{
	console_hide(Channel_active);
}
