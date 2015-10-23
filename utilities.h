/* utilities.h */
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
#ifndef    _UTILITIES_H_ 
#define    _UTILITIES_H_ 

#include <stdio.h>

#define extern 
#define inline 
#define __static 
#define __In
#define __Out

#define Pi (3.141592) /* After more than 1.5hrs of surfing around the Internet, watching Pink Panther. */

#define __namespace(namespace)             {}
#define STATIC_ASSERT(condition)           {}
#define ARRAY_LENGTH(array)                (sizeof(array)/sizeof(array[0]))
#define MALLOC(number, type)               ((type *) malloc(number * sizeof(type)))
#define ASSERT(condition, message)         do { if( !(condition) ) __assert(message); } while(false)
#define WAIT_PROCESS(process)              while( proc_status(process) ) wait(1.0)
#define KILL_PROCESS(process)              proc_kill(4) /* This macro exists because it helps eliminating magic numbers. */
#define DEFER(argument)                    wait(1.0)
#define WALK_THROUGH(object, function)     object = ptr_first(object); while(object) { function(object); o = o.link.next; wait(1.0); }
#define SWAP(a, b, type)                   { type X = a; a = b; b = X; } while(false)
#define UNLESS(condition)                  if( !(condition) )
#define PRINT_INT(i)                       printf("%i", (int) i)
#define PRINT_DOUBLE(d)                    printf("%f", (double) d)
#define PRINT_STRING(string)               printf("%s", (char *) string)

#define COLOR_RANDOM                       (vector( rand() % 255, rand() % 255, rand() % 255 ))
#define COLOR_SOFT_PURPLE                  (vector(219, 112, 147))
#define COLOR_LIME                         (vector(29, 230, 181))
#define COLOR_CHERRY                       (vector(99, 49, 222))
#define COLOR_INDIGO                       (vector(130, 0, 75))
#define COLOR_CRIMSON                      (vector(60, 20, 220))
#define COLOR_YELLOW                       (vector(0, 255, 255))
#define COLOR_LAVENDER_ROSE                (vector(227, 160, 251))
#define COLOR_LEMON_BITTER                 (vector(202, 224, 13))
#define COLOR_PEACH_ORANGE                 (vector(153, 204, 255))
#define COLOR_PALE_LILAC                   (vector(187, 187, 255))
#define COLOR_TEAL                         (vector(136, 117, 54))

/*
 * Because no one likes capitalizing their structs' names all the way.
 * Capitialized identifiers should be reserved for macro definitions instead.
 */
typedef STRING                             String;
typedef PANEL                              Panel;
typedef MATERIAL                           Material;
typedef BMAP                               Bitmap;
typedef ENTITY                             Object;
typedef TEXT                               Text;
typedef VIEW                               View;
typedef FONT                               Font;
typedef SOUND                              Stream;
typedef PARTICLE                           Particle;
typedef VECTOR                             Vector3;
typedef ANGLE                              Angle;
typedef COLOR                              Color;
typedef BOOL                               bool;

__namespace(error) {
	#define ERROR_CONTAINER_CAPACITY     256
	
	typedef struct {
		int pos;
		int stack[ERROR_CONTAINER_CAPACITY];
		Text *message;
	} Error;
	
	Error *Error_active = NULL;
	Error *Error_singleton = NULL;
}

__namespace(console) {
	#define CONSOLE_CONTAINER_CAPACITY     256
	
	typedef struct {
		Bitmap *background;
		Panel *__background_container;
		Text *data;
		Text *history;
	} Channel;
	
	Channel *Channel_active = NULL;
	Channel *Channel_singleton = NULL;
	
	void console_new( __In __Out Channel **console );
	void console_new();
	
	void console_free( __In Channel *console );
	void console_free();
	
	void console_background_set( __In Channel *console, __In const Bitmap *image );
	void console_background_set( __In Channel *console, __In const String *file );
	void console_background_set( __In const Bitmap *image );
	void console_background_set( __In const String *file );
	Bitmap *console_background_get( __In Channel *console );
	Bitmap *console_background_get();
	
	void console_show( __In Channel *console );
	void console_show();
	
	void console_hide( __In Channel *console );
	void console_hide();
	
	void console_toggle( __In Channel *console );
	void console_toggle();
}

__namespace(pair) {
	typedef struct {
		float first;
		float second;
	} Pair;
	
	void pair_set( __In __Out Pair *p, float first, float second );
	void pair_swap( __In __Out Pair *p1, __In __Out Pair *p2 );
}

__namespace() {
	void __assert( const char *message );
	char *strtok( __In const char *str, __In const char *delimiter );
	void reverse( __In __Out int *array, __In int count);
	void swap( __In __Out int *a, __In __Out int *b );
	double log10( __In double d );
	long join( __In int i, __In int j );
	void split( __In __Out int *__result, __In int value, __In int slot);
	void split( __In __Out int *__result, __In int value);
	double deg2rad( __In double d );
	double rad2deg( __In double d );
	bool ready();
}

__namespace(window) {
	#define FULLSCREEN          1
	#define WINDOWED            2
	
	void window_title_set( __In const char *title );
	const char *window_title_get();
	
	void window_pos_set( __In Pair *pos );
	void window_pos_set( __In float posx, __In float posy );
	Pair *window_pos_get();
	
	void window_color_set( __In const Vector3 *color );
	void window_color_set( __In float r, __In float g, __In float b );
	Vector3 *window_color_get();
	
	void window_size_set( __In const Pair *__size );
	void window_size_set( __In float width, __In float height );
	Pair *window_size_set();
	
	void window_aspect_set( __In float ratio );
	float window_aspect_get();
	
	void window_mode_set( __In int mode );
	int window_mode_get();
	const char *window_mode_get();
}

__namespace(string) {
	String *str_clip_ex( __In String *str, __In int n );
}

__namespace(object) {
	bool object_in_region( __In Object *object, __In const String *region );
	void object_bound_srhink( __In __Out Object *object );
	void object_bound_draw( __In __Out Object *object );
	void object_bound_draw( __In __Out Object *object, __In const Vector3 *color );
	void object_bound_get( __In __Out Object *object, __In __Out Vector3 *min, __In __Out Vector3 *max );
	void object_scale_set( __In Object *object, __In float scale );
	void object_place( __In __Out Object *object, __In float modifier );
}

#include "utilities.c"
#endif /* utilities.h */
