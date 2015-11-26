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
 * 
 * __________________________________________________________________
 * 
 * <utilities>
 * Contains general-purpose small utilities and helper functions/macros.
 * Lightweight and has no dependency other than Gamestudio's headers.
 * 
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * Version: v0.1.1-alpha
 * 
 * History
 * __________________________________________________________________
 * + v0.1-alpha
 * __________________________________________________________________
 * + v0.1.1-alpha
 * - Added pair_new(), pair_free(), is_odd() and shader_get_version().
 * - log10() and related functions were moved to the numeric namespace.
 * __________________________________________________________________
 * + v0.1.2-alpha
 * - Added more variants for pair_new().
 * - Added BIND_KEY(), CALLOC() and REALLOC().
 * - Added copy().
 * __________________________________________________________________
 * + v0.2.0-alpha
 * - Added calloc(), dump(), strstr(), str_trunc_ex(), str_parse_ex(), str_parse_delim(), txt_clear().
 * 
 * - Fixed str_clip_ex() not behaving correctly.
 * - Fixed: Switched all malloc()/free() calls to sys_malloc()/sys_free() due to
 * immediate crash with the former commands in published builds. From now on,
 * use MALLOC()/FREE() for memory allocation/deallocation instead.
 * 
 * - Deprecated: calloc()/CALLOC() (as it involves malloc() and memset() calls).
 * For zero-allocation, use MALLOC().
 * __________________________________________________________________
 * + v0.2.1-alpha
 * - Added txt_width(), txt_height(), clampf() and bgr_to_rgb().
 * - Removed home-brewed str*() functions and replace them with those from the C standard library.
 * __________________________________________________________________
 * + v0.2.2-alpha
 * - Fixed txt_width() returning wrong height (not taking font->dx into account)
 * - Add the gravity acceleration constant (GRAVITY_ACCELERATION)
 */
#ifndef    _UTILITIES_H_ 
#define    _UTILITIES_H_ 

#include <stdio.h>
#include <strio.c>

#define __VERSION "v0.2.1-alpha" // Seems familiar?

#ifdef ENFORCE_STRICT
    #ifndef    PRAGMA_POINTER
    #define    PRAGMA_POINTER
    #endif
#endif

#define extern 
#define inline 
#define __static 
#define __In
#define __Out

#define Pi                                 (3.141592) /* After more than 1.5hrs of surfing around the Internet, watching Pink Panther. */
#define GRAVITY_ACCELERATION               (9.80665)  /* Standard acceleration of freefall, 9.81 m/s^2, insurance included. */

#define ASSERT_MESSAGE_LENGTH              384

#define __namespace(namespace)             {}
#define STATIC_ASSERT(condition)           {}
#define ARRAY_LENGTH(array)                (sizeof(array)/sizeof(array[0]))
// #define MALLOC(number, type)               ((type *) malloc(number * sizeof(type)))
#define MALLOC(number, type)               ((type *) sys_malloc(number * sizeof(type)))
#define CALLOC(number, type)               (type *) calloc( number, sizeof(type) )
#define REALLOC(inlet, type, number)       (type *) realloc( inlet, sizeof(type) * number )
#define FREE(block)                        sys_free(block)
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
#define PRINT_BOOL(comparison)             printf("%s", (char *) ifelse(comparison, _chr("true"), _chr("false")))
#define BIND_KEY(key, function)            on_##key = function

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
 * Defined constants for inkey() and inchar().
 */
#define KEY_ENTER                          13
#define KEY_ESC                            27
#define KEY_TAB                            9

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

/*
 * A set of default, immutable (really?) fonts.
 * Mostly because literals are evil.
 */
const Font *Arial_10 = "Arial#10";
const Font *Arial_14 = "Arial#14";
const Font *Arial_18 = "Arial#18";
const Font *Arial_25 = "Arial#25";

__namespace(utility) {
	const char *library_get_version()
	{
		return _chr(__VERSION);
	}
}

__namespace(error) {
	#define ERROR_CONTAINER_CAPACITY     256
	#define ERROR_HISTORY_CAPACITY       128
	#define ERROR_HISTORY_MESSAGE_LENGTH 96
	
	typedef struct {
		int pos;                                   /* Contains the current position of the error history stack. */
		int count;                                 /* Contains the current position of the error stack. */
		int stack[ERROR_CONTAINER_CAPACITY];       /* Error stack, containing error codes. Not much use other than acting like an error codebook. */
		int history[ERROR_HISTORY_CAPACITY];       /* Error history stack, containing occured errors (if pushed through error_push()). */
		Text *message;                             /* Error history description stack (container), containing explanation for occured errors that */
		                                           /* were previously pushed through error_push(). */
	} Error;
	
	Error *Error_active = NULL;
	Error *Error_singleton = NULL;
	
	void error_bind( __In Error *table );
	void error_new();
	void error_new( __In __Out Error **table );
	void error_free();
	void error_free( __In Error *table );
	
	void error_code_push( __In Error *table, __In const unsigned int code );
	void error_code_push( __In const unsigned int code );
	void error_push( __In const unsigned int code, __In const char *message );
	char *error_message_get( __In int id );
	int error_history_capacity_get();
	int error_history_cursor_get();
}

__namespace(console) {
	#define CONSOLE_CONTAINER_CAPACITY     256
	#define CONSOLE_COMMAND_MAX_LENGTH	   32
	
	void *__CommandTable_action[CONSOLE_CONTAINER_CAPACITY];
	Text *__CommandTable_command;
	int __CommandTable_pointer;
	
	void command_table_new();
	void command_table_free();
	void command_table_add( __In const char *command, __In const void *func );
	void command_table_remove( __In int id );
	void command_table_remove( __In const char *command );
	void command_table_call( __In int id );
	void command_table_call( __In const char *command );
	
	typedef struct {
		Bitmap *background;                   /* Background image for the console. The idea for this first came from the Half-Life console. */
		Panel *__background_container;        /* Solely serves as the container for the background image. */
		Text *data;                           /* Contains the input channel, represented as a single string. Commands are fed here. */
		Text *history;                        /* Contains the input history for everything typed. (TODO: Implement this) */
		bool ready;                           /* Flags true if the console was successfully allocated and initialized, false otherwise. */
	} Channel;
	
	Channel *Channel_active = NULL;
	Channel *Channel_singleton = NULL;
	
	void console_bind( __In Channel *console );
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
	
	/*
	void console_command_add( __In Channel *console, __In const char *command, __In const void *func );
	void console_command_add( __In const char *command, __In const void *func );
	void console_command_remove( __In Channel *console, __In const char *command );
	void console_command_remove( __In const char *command );
	*/
	
	void console_update();
	void console_show();
	void console_hide();
	bool console_state_get_invisibility();
	bool console_state_get_ready( __In Channel *console );
	bool console_state_get_ready();
}

__namespace(pair) {
	typedef struct {
		float first;
		float second;
	} Pair;
	
	typedef Pair Vector2;
	
	/*
	Vector3 *pair_new();
	Vector3 *pair_new( __In float first, __In float second );
	Vector3 *pair_new( __In const Vector3 *vektor );
	*/
	
	Pair *pair_new();
	Pair *pair_new( __In float first, __In float second );
	Pair *pair_new( __In const Pair *p );
	
	void pair_free( __In __Out Pair *pair);
	
	void pair_set( __In __Out Pair *p, float first, float second );
	void pair_swap( __In __Out Pair *p1, __In __Out Pair *p2 );
}

__namespace() {
	// Removed since 0.2.1-alpha: Use msvcrt instead. (call libc_init() to initialize these functions.)
	// char *strtok( __In const char *str, __In const char *delimiter );
	// char *strstr( __In char *str1, __In const char *str2);
	
	typedef int   errno_t;
	typedef int * intptr_i;
	
	#define    FILE_EXISTS                0
	#define    FILE_WRITABLE              2
	#define    FILE_READABLE              4
	#define    FILE_READABLE_WRITEABLE    6
	
	// Source: http://www.math.uiuc.edu/~gfrancis/illimath/windows/aszgard_mini/bin/MinGW/include/sys/stat.h
	#define    _S_IWRITE    0x0080
	#define    _S_IREAD     0x0100
	
	void abort();
	char * __cdecl strstr(const char *, const char *);
	char * __cdecl strchr(const char *, int c);
	size_t __cdecl strcspn(const char *, const char *);
	char * __cdecl strncat(char *, char *, size_t);
	int    __cdecl strncmp(char *, char *, size_t);
	char * __cdecl strncpy(char *, char *, size_t);
	char * __cdecl strpbrk(char *, char *);
	char * __cdecl strrchr(char *, int);
	size_t __cdecl strspn(char *, char *);
	char * __cdecl strtok(char *, char *);
	size_t __cdecl strxfrm(char *, char *, size_t);
	double __cdecl atof(const char *str);
	int    __cdecl atoi(const char *str);
   long   __cdecl atol(const char *str);
   char * __cdecl _tempnam( const char *dir, const char *prefix );
   int    __cdecl _chmod( const char *filename, int pmode);
   int    __cdecl _creat( const char *filename, int pmode );
   char * __cdecl _getcwd( char *buffer, int maxlen );
   char * __cdecl getenv( const char *varname );
   char * __cdecl _strset(char *str, int c );
   char * __cdecl _strrev( char *str );
   intptr_t __cdecl _execl( const char *cmdname, const char *arg0, ... );
   errno_t __cdecl strcpy_s( char *strDestination,  size_t numberOfElements,  const char *strSource );
   errno_t __cdecl _access_s( const char *path, int mode );
	
	void libc_init(); // Initializes (links) these headers
	int  ___libc_init__done__ = 0;
	
	char *dump( __In const char *content );
	void *calloc( __In int count, __In size_t size);
	void __assert( const char *message );
	char *function_name_get( __In const void *f );
	void reverse( __In __Out int **array, __In unsigned int count);
	void swap( __In __Out int *a, __In __Out int *b );
	void copy( __In __Out float **i, __In float **j, int num );
	
	long join( __In int i, __In int j );
	void split( __In __Out int *__result, __In int value, __In int slot);
	void split( __In __Out int *__result, __In int value);
	int search( __In Text *container, __In const String *str );
	int search( __In Text *container, __In const char *cstr );
	bool ready();
}

__namespace(numeric) {
	double log10( __In double d );
	bool is_odd( __In int i );
	
	double deg2rad( __In double d );
	double rad2deg( __In double d );
	
	float clampf( __In float x, __In float a, __In float b);
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

__namespace(video) {
	Pair *shader_get_version();
}

__namespace(string) {
	String *str_clip_ex( __In String *str, __In unsigned int n );
	String *str_trunc_ex( String *str, unsigned int n );
	String *str_create_ex( __In const int length );
	
	void str_init( __In String *gstr );
	void str_init( __In char *cstr );
	
	String *str_parse_ex( __In String *to, __In char *from, __In int pos, __In char delimiter );
	void str_parse_delim( __In __Out Text *text, __In char *content, __In char delimiter );
}

__namespace(text) {
	void txt_clear( __In __Out Text *object );
	void txt_remove_ex( __In __Out Text *object );
	
	float txt_width( __In Text *object );
	float txt_height( __In Text *object );
}

__namespace(object) {
	bool object_in_region( __In Object *object, __In const String *region );
	void object_bound_srhink( __In __Out Object *object );
	void object_bound_draw( __In __Out Object *object );
	void object_bound_draw( __In __Out Object *object, __In const Vector3 *color );
	void object_bound_get( __In __Out Object *object, __In __Out Vector3 *min, __In __Out Vector3 *max );
	void object_scale_set( __In Object *object, __In float scale );
	void object_place( __In __Out Object *object, __In float modifier );
	Object *object_sky_create( __In const String *file, __In const int layer );
}

__namespace(color) {
	Vector3 *bgr_to_rgb( __In Vector3 *rgb );
	void     bgr_to_rgb( __In Vector3 *rgb );
}

#include "utilities.c"
#endif /* utilities.h */
