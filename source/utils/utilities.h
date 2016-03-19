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
 * Version: Call library_get_version() to get the current version.
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
 * - Fixed: txt_width() returning the wrong height (not taking font->dx into account)
 * - Fixed: __assert() won't close the file after writing data.
 *
 * - Added the gravity acceleration constant (GRAVITY_ACCELERATION)
 * - Added a lot more color vectors (sand, peach, pearl, aquamarine, orchid, green, arctic lime, blue, scarlet.)
 * - Added snprintf() to libc_init().
 * - Added more ASSERT() macros to library functions.
 *
 * - Versioning bumped.
 * __________________________________________________________________
 * + v0.3.0-alpha
 * - Added os_get_name(), file_get_ext(), fade(), pair_set(), bkpt()/bkptend(), window_pos_get(), window_color_get(),
 *         window_size_get(), window_title_get(), ArrayContainer, num_parse_delim(), sgn().
 * - Added two macros FLT_MAX (maximum float value) and DBL_MAX (maximum double value); RETURN(0).
 * - Added ASSERT_ON() and ASSERT_OFF() for switching assertions state.
 * - Added short comments to the .h interface. (full comments with side notes can still be viewed in the implementation code).
 * - (Partially) added the Windows API set wrappers, which can be used if WINDOWS_API is defined.
 * -- Functions available:  os_get_user_name(), os_get_computer_name(), os_get_system_directory(), os_is_privileged(),
 *                          os_get_drives().
 *
 * - Removed DEFER() macro.
 * - Removed the <windows+> header and from now on all APIs will reside together with the WINDOWS_API switch.
 *
 * - Fixed a bug in str_parse_ex() which crashes str_parse_delim().
 * - Rewrote search().
 * __________________________________________________________________
 * + v0.3.1-alpha
 * - Added Lemming's str_width_ex().
 * - Added hex_to_rgb(), object_draw().
 * - Added a set of functionalities for reading from/writing to .ini files.
 * __________________________________________________________________
 * + v0.3.2-alpha
 * - Cleaner assertions, and they are enabled by default (just put a #define DISABLE_ASSERT somewhere to disable asserts).
 * __________________________________________________________________
 * + v0.4.0-alpha
 * - Added a set of simple functions for writing to and reading from the Windows registry.
 * - Added fifelse(), a float variant of ifelse(), along with vifelse() (VECTOR), sifelse() (STRING) and difelse() (double).
 * __________________________________________________________________
 * TODO:
 * - Implement STATIC_ASSERT().
 */
#ifndef    _UTILITIES_H_
#define    _UTILITIES_H_

#include <stdio.h>
#include <strio.c>

#define __static
#define __In
#define __Out

#define __VERSION "v0.4.0-alpha" // Seems familiar?

#ifdef    ENFORCE_STRICT
    #ifndef    PRAGMA_POINTER
    #define    PRAGMA_POINTER
    #endif
#endif

#ifdef    WINDOWS_API
    #include <windows.h>

    /*
     * Additional APIs that are currently missing in Lite-C's <windows>.
     */

    // Collection of functions for working with console windows.
    long WINAPI WriteConsole(int Handle, char* Buffer, int CharsToWrite, int* CharsWritten, int reserved);
    long WINAPI CreateConsoleScreenBuffer(long dwDesiredAccess, long dwShareMode, long *lpSecurityAttributes, long dwFlags, long lpScreenBufferData);
    long WINAPI SetConsoleActiveScreenBuffer(long hConsoleOutput);
    BOOL WINAPI SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes );
#endif

/*
 * Constants.
 */
#define Pi                                 (3.141592) /* After more than 1.5hrs of surfing around the Internet, watching Pink Panther. */
#define GRAVITY_ACCELERATION               (9.80665)  /* Standard acceleration of freefall, 9.81 m/s^2, insurance included. */
#define FLT_MAX                            (3.4 * pow(10, 38))
#define DBL_MAX                            (1.8 * pow(10, 308))
#define POW_10_6                           (double) 1000000.0
#define ASSERT_MESSAGE_LENGTH              128

/*
 * Macros, which is distinguishable by their capitalized names.
 * Because of that, I strongly encourage you to use typedef-ed names for Gamestudio
 * built-in types (Materials, Panels, Texts, ...) rather than its original, capitialized name (could cause confusion).
 */
#define __namespace(namespace)             {}
#define STATIC_ASSERT(condition)           {}

#define ARRAY_LENGTH(array)                (sizeof(array)/sizeof(array[0]))
#define RETURN(code)                       sys_exit(NULL)
// #define MALLOC(number, type)               ((type *) malloc(number * sizeof(type)))
#define MALLOC(number, type)               ((type *) sys_malloc(number * sizeof(type)))
#define CALLOC(number, type)               (type *) calloc( number, sizeof(type) )
#define REALLOC(inlet, type, number)       (type *) realloc( inlet, sizeof(type) * number )
#define FREE(block)                        sys_free(block)
#define ASSERT(condition, message)         do { if( !(condition) ) __assert(message); } while(false)
#define WAIT_PROCESS(process)              while( proc_status(process) ) wait(1.0)
#define KILL_PROCESS(process)              proc_kill(4) /* This macro exists because it helps eliminating magic numbers. */
#define WALK_THROUGH(object, function)     object = ptr_first(object); while(object) { function(object); o = o.link.next; wait(1.0); }
#define SWAP(a, b, type)                   { type X = a; a = b; b = X; } while(false)
#define UNLESS(condition)                  if( !(condition) )
#define PRINT_INT(i)                       printf("%i", (int) i)
#define PRINT_DOUBLE(d)                    printf("%f", (double) d)
#define PRINT_STRING(string)               printf("%s", (char *) string)
#define PRINT_BOOL(comparison)             printf("%s", (char *) ifelse(comparison, _chr("true"), _chr("false")))
#define BIND_KEY(key, function)            on_##key = function

/*
 * A small selection of ready-to-use color vectors.
 */
#define COLOR_RANDOM                       (vector(rand() % 255, rand() % 255, rand() % 255))
#define COLOR_SOFT_PURPLE                  (vector(219, 112, 147))
#define COLOR_LIME                         (vector(29, 230, 181))
#define COLOR_CHERRY                       (vector(99, 49, 222))
#define COLOR_INDIGO                       (vector(130, 0, 75))
#define COLOR_CRIMSON                      (vector(60, 20, 220))
#define COLOR_YELLOW                       (vector(0, 255, 255))
#define COLOR_LAVENDER_ROSE                (vector(227, 160, 251))
#define COLOR_BITTER_LEMON                 (vector(202, 224, 13))
#define COLOR_PEACH_ORANGE                 (vector(153, 204, 255))
#define COLOR_PALE_LILAC                   (vector(187, 187, 255))
#define COLOR_TEAL                         (vector(136, 117, 54))
#define COLOR_ORCHID                       (vector(214, 112, 218))
#define COLOR_APPLE_GREEN                  (vector(0, 182, 141))
#define COLOR_ARCTIC_LIME                  (vector(20, 255, 208))
#define COLOR_AQUAMARINE                   (vector(212, 255, 127))
#define COLOR_BLEU_DE_FRANCE               (vector(231, 140, 49))
#define COLOR_SCARLET                      (vector(0, 36, 255))
#define COLOR_SAND                         (vector(128, 178, 194))
#define COLOR_PEACH                        (vector(164, 203, 255))
#define COLOR_PEAR                         (vector(49, 226, 209))
#define COLOR_PEARL                        (vector(198, 224, 234))

/*
 * Defined constants for inkey() and inchar().
 */
#define KEY_ENTER                          13
#define KEY_ESC                            27
#define KEY_TAB                            9

/*
 * Because no one likes capitalizing their structs' names all the way.
 * Capitialized identifiers should be reserved for macro definitions instead. (see above)
 */
typedef STRING                             String;
typedef PANEL                              Panel;
typedef MATERIAL                           Material;
typedef BMAP                               Bitmap;
typedef ENTITY                             Object;
typedef TEXT                               Text;
typedef VIEW                               View;
typedef FONT                               Font;
typedef SOUND                              Sound;
typedef PARTICLE                           Particle;
typedef VECTOR                             Vector3;
typedef VECTOR                             Vector;
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
	/*
	 * const char *library_get_version()
	 *
	 * Returns the current version of the library.
	 */
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

    /*
     * void error_bind( Error *table )
     *
     * Binds the error object specified in the first parameter as the active error object.
     */
	void error_bind( __In Error *table );

    /*
     * void error_new( Error **table )
     *
     * Creates a new error object. As with console_new() (explained before),
     * this function is just for initializating error objects, not binding them to active.
     * Calling error_new() without parameters cause the default error table object to be invoked, initialized, and bound.
     * You should have only one error table during the whole program execution flow.
     */
	void error_new( __In __Out Error **table );
    void error_new();

    /*
     * void error_free( Error *table )
     *
     * Frees the error object.
     * For freeing the currently bound error object, use the second variant.
     */
    void error_free( __In Error *table );
	void error_free();

    /*
     * void error_code_push( Error *table, const unsigned int code )
     *
     * Pushes an error code to the error table. For pushing into the currently bound error object,
     * omit the first parameter.
     */
	void error_code_push( __In Error *table, __In const unsigned int code );
	void error_code_push( __In const unsigned int code );

    /*
     * void error_push( const unsigned int code, const char *message )
     *
     * Commits an error, including the error code and explanation.
     * It can be retrieved later using error_message_get().
     */
	void error_push( __In const unsigned int code, __In const char *message );

    /*
     * char *error_message_get( int id )
     *
     * Retrieves the error message associated with the specific position in the history stack.
     */
	char *error_message_get( __In int id );

    /*
     * int error_history_capacity_get()
     *
     * Returns the error object's hard limit on maximum error code pushes.
     */
	int error_history_capacity_get();

    /*
     * int error_history_cursor_get()
     *
     * Returns the currently bound error object's history stack position.
     */
	int error_history_cursor_get();
}

__namespace(console) {
	#define CONSOLE_CONTAINER_CAPACITY     256
	#define CONSOLE_COMMAND_MAX_LENGTH	   32

	void *__CommandTable_action[CONSOLE_CONTAINER_CAPACITY];
	Text *__CommandTable_command;
	int __CommandTable_pointer;

	typedef struct {
		Bitmap *background;                   /* Background image for the console. The idea for this first came from the Half-Life console. */
		Panel *__background_container;        /* Solely serves as the container for the background image. */
		Text *data;                           /* Contains the input channel, represented as a single string. Commands are fed here. */
		Text *history;                        /* Contains the input history for everything typed. (TODO: Implement this) */
		bool ready;                           /* Flags true if the console was successfully allocated and initialized, false otherwise. */
	} Channel;

	Channel *Channel_active = NULL;
	Channel *Channel_singleton = NULL;

    /*
     * void console_bind( Channel *console )
     *
     * Binds the console specified in the first parameter as the active console object.
     */
	void console_bind( __In Channel *console );

    /*
     * void console_new( Channel **console )
     *
     * Initializes a new console object.
     * Note that initializing the console through console_new() will bind the console object
     * to active during initialization, whereas initializing a specific console object WON'T
     * bind it to active. This is to make sure you can initialize many console objects
     * simultaneously without altering the active console object.
     */
	void console_new( __In __Out Channel **console );
	void console_new();

    /*
     * void console_free( Channel *console )
     *
     * Frees the specified console.
     * Repeatedly freeing and allocating consoles in-game is *NOT* recommended as it can cause unexpected crashes.
     * Consoles should be closed only during resource releasing and exiting the game.
     * Omitting the first parameter will get the active console object freed instead.
     */
	void console_free( __In Channel *console );
	void console_free();

    /*
     * void command_table_new()
     *
     * Initializes the command table.
     * During exit, command_table_free() must be summoned to release the command table.
     */
    void command_table_new();

    /*
     * void command_table_free()
     *
     * Command tables are not recommended to be freed in-game, as for the notes with console_free().
     * (Ctrl-F for console_free() command section and have a quick look).
     */
    void command_table_free();

    /*
     * void command_table_add( const char *command, const void *func )
     *
     * Adds a command to the command table.
     * The passed function must have "void" return type and no parameters.
     */
    void command_table_add( __In const char *command, __In const void *func );

    /*
     * void command_table_remove( int id )
     *
     * Removes a command from the command table.
     * A more convenient variant (and essential for console command processing) is provided below.
     */
    void command_table_remove( __In int id );
    void command_table_remove( __In const char *command );

    /*
     * void command_table_call( int id )
     *
     * Executes a command associated with the id-th string in the command table.
     * Another variant taking a const char * is provided below.
     */
    void command_table_call( __In int id );
    void command_table_call( __In const char *command );

    /*
     * void console_background_set( Channel *console, const String *file )
     *
     * Sets the console object's background image. Omitting the first parameter makes the bitmap created for the
     * currently bound console object.
     */
	void console_background_set( __In Channel *console, __In const Bitmap *image );
	void console_background_set( __In Channel *console, __In const String *file );
	void console_background_set( __In const Bitmap *image );
	void console_background_set( __In const String *file );

    /*
     * Bitmap *console_background_get( Channel *console )
     * Bitmap *console_background_get()
     *
     * Retrieves the console object's background.
     */
	Bitmap *console_background_get( __In Channel *console );
	Bitmap *console_background_get();

	/*
	void console_command_add( __In Channel *console, __In const char *command, __In const void *func );
	void console_command_add( __In const char *command, __In const void *func );
	void console_command_remove( __In Channel *console, __In const char *command );
	void console_command_remove( __In const char *command );
	*/

    /*
     * void console_update()
     *
     * Updates the console for one frame.
     * This function updates the currently bound console object for receiving input, and thus
     * must be placed in a loop.
     */
	void console_update();

    /*
     * void console_show()
     *
     * Shows the currently active console object.
     * The rule is, you can create as many console objects as you want,
     * but only one of them is bound and used at a time.
     * For this, a variant for showing/hiding console objects which aren't in bound
     * is not provided. You can, however, call __console_show() with a parameter
     * specifying the console object you want to show/hide.
     */
	void console_show();

    /*
     * void console_hide()
     *
     * Hides the currently bound console object.
     */
	void console_hide();

    /*
     * bool console_state_get_invisibility()
     *
     * Returns the invisibility state of the currently bound console object.
     */
	bool console_state_get_invisibility();

    /*
     * bool console_state_get_ready( Channel *console )
     *
     * Returns true if the specified console object is ready to use, false otherwise.
     * A ready console is a fully initialized console through console_new().
     */
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

    /*
     * Pair *pair_new()
     *
     * Creates a new, empty pair that you can use for later filling.
     */
	Pair *pair_new();
	Pair *pair_new( __In float first, __In float second );
	Pair *pair_new( __In const Pair *p );

    /*
     * void pair_free( Pair *pair )
     *
     * Frees a pair that was allocated before via pair_new().
     */
	void pair_free( __In __Out Pair *pair);

    /*
     * void pair_set( Pair *p, float first, float second )
     *
     * Fills a pair with values.
     */
	void pair_set( __In __Out Pair *p, float first, float second );

    /*
     * void pair_set( Pair *p1, Pair *p2 )
     *
     * Fills a pair with another pair.
     */
	void pair_set( __In __Out Pair *p1, __In Pair *p2 );

    /*
     * void pair_swap( Pair *p1, Pair *p2 )
     *
     * Swaps two pairs.
     */
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
   int    __cdecl snprintf ( char * s, size_t n, const char * format, ... );
   char * __cdecl _getcwd( char *buffer, int maxlen );
   char * __cdecl getenv( const char *varname );
   char * __cdecl _strset(char *str, int c );
   char * __cdecl _strrev( char *str );
   intptr_t __cdecl _execl( const char *cmdname, const char *arg0, ... );
   errno_t __cdecl strcpy_s( char *strDestination,  size_t numberOfElements,  const char *strSource );
   errno_t __cdecl _access_s( const char *path, int mode );
   int __cdecl system(const char *command);

	int  ___libc_init__done__ = 0;

    /*
     * void libc_init()
     *
     * "Links" the vanilla C string functions.
     */
     void libc_init();

    /*
     * char *dump(const char *content)
     *
     * Dumps content from a file, or return a string if the file doesn't exist.
     */
	char *dump( __In const char *content );

    /*
     * void *calloc(int count, size_t size)
     *
     * C's calloc() reimplementation in Lite-C.
     */
	void *calloc( __In int count, __In size_t size);

    /*
     * void __assert( const char *message )
     *
     * A very simple assert() implementation.
     */
	void __assert( const char *message );

    /*
     * char *function_name_get( const void *f )
     *
     * Retrieve a function's name.
     */
	char *function_name_get( __In const void *f );

    /*
     * void reverse(int **array, unsigned int count)
     *
     * Reverse count elements in the specified array.
     */
	void reverse( __In __Out int **array, __In unsigned int count);

    /*
     * void swap( int *a, int *b )
     *
     * Swaps two integers. For general-purpose swapping, please
     * refer to the SWAP() macro.
     */
	void swap( __In __Out int *a, __In __Out int *b );

    /*
     * void copy( float **i, float **j, int num )
     *
     * Copies a pointer-based array over another, suppressing boundary checks.
     */
	void copy( __In __Out float **i, __In float **j, int num );

    /*
     * int join( int i, int j )
     *
     * Joins two integers.
     */
	long join( __In int i, __In int j );

    /*
     * void split(int *__result, int value, int slot)
     *
     * Split a value's digit characters into an integer array.
     * The third parameter controls at which position the digits are
     * inserted into the array. So you can insert the digits
     * to your array which already has data.
     */
	void split( __In __Out int *__result, __In int value, __In int slot);
	void split( __In __Out int *__result, __In int value);

    /*
     * int search( Text *container, const String *str )
     *
     * A __very__ slow, __unoptimized__ and basic search function
     * which looks for a specified string in a Text container.
     * Suitable for console command dictionary.
     */
	int search( __In Text *container, __In const String *str );
    int search ( __In Text *container, __In const String *str, __In int case_sensivity );

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
	bool ready();

    /*
     * bkpt(), bkptend().
     * Small functions to avoid lots of printf() when debugging/tracing for errors in the code.
     * bkptend() resets the counter.
     */
    int  __bkptcnt__ = 0;
	void bkpt();
	void bkptend();
}

__namespace(numeric) {
    /*
     * double log10( double d )
     *
     * Calculates log(10, d).
     */
	double log10( __In double d );

    /*
     * bool is_odd( int i )
     *
     * Returns true if the given number is an odd number, false otherwise.
     */
	bool is_odd( __In int i );

    /*
     * double deg2rad( double d )
     *
     * Converts degrees to radians.
     */
	double deg2rad( __In double d );

    /*
     * double rad2deg( double d )
     *
     * Converts radians to degrees.
     */
	double rad2deg( __In double d );

    /*
     * float clampf(float x, float a, float b)
     *
     * A "fall" variant of clamp() - that is, if x is greater than b,
     * returns a instead of "locking" x to b.
     */
	float clampf( __In float x, __In float a, __In float b);

    /*
     * int sgn( int i )
     *
     * Extracts the sign of a number.
     */
    int sgn( __In int i );

    /*
     * float fifelse( int cond, float a, float b )
     *
     * A float variant of ifelse().
     */
    float fifelse( __In int cond, __In float a, __In float b );

    /*
     * double difelse( int cond, double a, double b )
     *
     * A double variant of ifelse().
     */
    double difelse( __In int cond, __In double a, __In double b );

    /*
     * VECTOR *vifelse( int cond, VECTOR *a, VECTOR *b )
     *
     * A VECTOR variant of ifelse().
     */
    VECTOR *vifelse( __In int cond, __In VECTOR *a, __In VECTOR *b );

    /*
     * STRING *sifelse( int cond, STRING *a, STRING *b )
     *
     * A STRING variant of ifelse().
     */
    STRING *sifelse( __In int cond, __In STRING *a, __In STRING *b );
}

__namespace(window) {
	#define FULLSCREEN          1
	#define WINDOWED            2

    /*
     * void window_title_set( const char *title )
     *
     * Sets the window title.
     */
	void window_title_set( __In const char *title );

    /*
     * char *window_title_get()
     *
     * Retrieves the current title of the engine window.
     */
    char *window_title_get();

    /*
     * void window_pos_set( Pair *pos )
     * void window_pos_set( float posx, float posy )
     *
     * Sets the window's position.
     */
	void window_pos_set( __In Pair *pos );
	void window_pos_set( __In float posx, __In float posy );

    /*
     * Pair *window_pos_get()
     *
     * Returns the desktop position of the engine window.
     */
	Pair *window_pos_get();

    /*
     * void window_color_set( const Vector3 *color )
     *
     * Indirectly sets the window color.
     * Note that the window color can only be set as long as no raw level_load() calls have been executed.
     */
	void window_color_set( __In const Vector3 *color );
	void window_color_set( __In float r, __In float g, __In float b );

    /*
     * Vector3 *window_color_get()
     *
     * Returns the current background color of the window in the RGB form.
     */
	Vector3 *window_color_get();

    /*
     * void window_size_set( float width, float height )
     * void window_size_set( const Pair *__size )
     *
     * Sets the Acknex window size.
     */
	void window_size_set( __In const Pair *__size );
	void window_size_set( __In float width, __In float height );

    /*
     * Pair *window_size_get()
     *
     * Return the size of the window.
     */
	Pair *window_size_get();

    /*
     * void window_aspect_set( float ratio )
     *
     * Sets the Acknex screen aspect ratio.
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
	void window_aspect_set( __In float ratio );

    /*
     * float window_aspect_get()
     *
     * Retrieves the aspect ratio in use.
     */
	float window_aspect_get();

    /*
     * void window_mode_set( int mode )
     *
     * Sets the window mode. Available options are:
     * FULLSCREEN (1)
     * WINDOWED (2)
     */
	void window_mode_set( __In int mode );

    /*
     * int window_mode_get()
     * const char *window_mode_get()
     *
     * Retrieves the currently in use video mode.
     */
	int window_mode_get();
	const char *window_mode_get();
}

__namespace(video) {
    /*
     * Pair *shader_get_version()
     *
     * Returns the current shader version supported by your graphics card.
     * The first value in the returned pair indicates the pixel shader version supported,
     * while the second value indicates the vertex shader version.
     */
	Pair *shader_get_version();
}

__namespace(os) {
    /*
     * char *os_get_name()
     *
     * Retrieves the current OS in use.
     */
    char *os_get_name();

    /*
     * char *os_get_user_name()
     *
     * Gets the current user name in the system.
     */
    char *os_get_user_name();

    /*
     * char *os_get_computer_name()
     *
     * Gets the computer name.
     */
    char *os_get_computer_name();

    /*
     * char *os_get_system_directory()
     *
     * Retrieves the system directory, based on Windows's architecture (SysWOW64 if x64 Windows version, System32 otherwise).
     */
    char *os_get_system_directory();

    /*
     * BOOL os_is_privileged()
     *
     * Test if the executable was executed with administrator rights.
     */
    BOOL os_is_privileged();

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
     */
    Text *os_get_drives();
}

__namespace(io) {
    /*
     * char *file_get_ext ( const char *fn )
     *
     * Fetchs the extension of a given file name.
     */
	char *file_get_ext ( __In const char *fn );

    /*
     * A set of facilities used for writing to and reading from .ini files.
     * You should use the Windows registry whenever possible (it is available in the Gamestudio API
     * through the sys_get/set*() functions), as Microsoft may deprecate and/or remove the INI API in the future.
     * But INI is portable across platforms and it's easier to read.
     */
    #define    INI_BUFFER_LENGTH    3072
    __static char __ini_buffer__[INI_BUFFER_LENGTH];

    /*
     * void ini_write(const char *filename, char *section, char *entry, char *value)
     * void ini_write_int(const char *filename, char *section, char *entry, int i)
     * void ini_write_float(const char *filename, char *section, char *entry, float f)
     *
     * Writes a generic char array, integer, or float value to the specified INI file.
     * The path to the file must be an absolute path.
     */
    void ini_write(const char *filename, char *section, char *entry, char *value);
    void ini_write_int(const char *filename, char *section, char *entry, int i);
    void ini_write_float(const char *filename, char *section, char *entry, float f);

    /*
     * const char *ini_read(const char *filename, char *section, char *entry, char *defaultValue)
     * int ini_read_int(char *filename, char *section, char *entry, int defaultValue)
     * float ini_read_float(char *filename, char *section, char *entry, float defaultValue)
     *
     * Reads out a generic char array, an integer, or a float value out of an INI file.
     * The path to the file must be an absolute path.
     */
    const char *ini_read(const char *filename, char *section, char *entry, char *defaultValue);
    int ini_read_int(char *filename, char *section, char *entry, int defaultValue);
    float ini_read_float(char *filename, char *section, char *entry, float defaultValue);

    /*
     * TEXT *ini_read_sections(const char *filename)
     *
     * Reads out all sections of the specified INI file.
     * The path to the file must be an absolute path.
     *
     * text->strings contains the number of read sections.
     */
    TEXT *ini_read_sections(const char *filename);

    /*
     * A set of simple functions which wrap around sys_setstr() and sys_getstr()
     * for reading from and writing to the Windows registry.
     * Due to Lite-C's limitations, and I don't want to make a mess out of function overloading,
     * so currently these functions can only operate on string/string key/value pairs. For converting
     * to numeric values, str_to_int()/str_to_float() can be used instead:
     *      int i = str_to_int(item->value);
     * or by using reg_key_to_int()/reg_key_to_float():
     *      int i = reg_key_to_int(item);
     */
    #define    REGISTRY_KEY_MAX_KEY_LENGTH    128
    #define    REGISTRY_KEY_MAX_VALUE_LENGTH  128

    typedef struct {
        char *key;
        char *value;
    } RegistryItem;

    /*
     * RegistryItem *reg_key_new(const char *key, const char *value)
     *
     * Creates a registry item which can be used later for writing/reading to/from the Windows registry.
     */
    RegistryItem *reg_key_new(const char *key);
    RegistryItem *reg_key_new(const char *key, const char *value);

    /*
     * void reg_key_free(RegistryItem *item)
     *
     * Frees a previously allocated key/value registry item.
     */
    void reg_key_free(RegistryItem *item);

    /*
     * int reg_key_write(const char *location, RegistryItem *item)
     *
     * Writes a RegistryItem item to the Windows registry.
     * Returns !0 upon succeed, 0 otherwise.
     */
    int reg_key_write(const char *location, RegistryItem *item);

    /*
     * int reg_key_exists(const char *location, RegistryItem *item)
     *
     * Checks if a given key/value pair exists in the Windows registry.
     * Returns !0 upon succeed, 0 otherwise.
     */
    int reg_key_exists(const char *location, RegistryItem *item);

    /*
     * int reg_key_to_int(const RegistryItem *item)
     * float reg_key_to_float(const RegistryItem *item)
     *
     * Utility functions which perform numeric conversion from the value field of the RegistryItem instance.
     */
    int reg_key_to_int(const RegistryItem *item);
    float reg_key_to_float(const RegistryItem *item);
}

__namespace(GUI) {
    /*
     * void fade( PANEL *p, float a, float b, float s )
     *
     * Performs fading in/out.
     */
	void fade( __In __Out PANEL *p, __In float a, __In float b, __In float s );
}

__namespace(string) {
	__static bool __from_delimstr = false;

    /*
     * String *str_clip_ex( String *str, int n )
     *
     * An extension to str_clip() which returns the cut string.
     */
	String *str_clip_ex( __In String *str, __In unsigned int n );

    /*
     * String *str_trunc_ex( String *str, unsigned int n )
     *
     * An extension to str_trunc() which returns the cut string.
     */
	String *str_trunc_ex( String *str, unsigned int n );

    /*
     * String *str_create_ex( const int length )
     *
     * An extension to str_create() which is more flexible (but slower).
     * It allows one to explicitly provide the string length in actual integers
     * instead of fixed strings. With this, you can create lots of strings
     * with different lengths without tinkering with the dirty # symbol.
     */
	String *str_create_ex( __In const int length );

    /*
     * float str_width_ex(STRING *gstr, FONT *font)
     *
     * Returns the width of a string with ending spaces.
     */
    float str_width_ex(STRING *gstr, FONT *font);

    /*
     * void str_init( String *gstr )
     *
     * Initializes a given string with blank spaces.
     * This is safer (but slower) than str_cpy() as it doesn't
     * mess with the string length.
     */
	void str_init( __In String *gstr );
	void str_init( __In char *cstr );

    /*
    * String *str_parse_ex( String *to, char *from, int pos, char delimiter )
    *
    * An extension to <strio>/str_parse() which allows for custom delimiters.
    */
	String *str_parse_ex( __In String *to, __In char *from, __In int pos, __In char delimiter );

    /*
    * void str_parse_delim( Text *text, char *content, char delimiter )
    *
    * Breaks a string into substrings separated by the delimiter character given in
    * delimiter, and pushes the result substrings into the given text object.
    */
	void str_parse_delim( __In __Out Text *text, __In char *content, __In char delimiter );

    /*
     * ArrayContainer (struct)
     *
     * A very, very simple data structure that was made for use within
     * num_parse_delim(), which is another simple function used for parsing
     * out the numbers from a given string.
     */
    typedef struct {
        int count;
        int pointer;
        int *data;
    } ArrayContainer;

    /*
     * ArrayContainer *array_container_new( int elem )
     * ArrayContainer *array_container_new()
     *
     * Allocates and initializes a new array container.
     */
    ArrayContainer *array_container_new( __In int elem );
    ArrayContainer *array_container_new();

    /*
     * void array_container_free( ArrayContainer *container )
     *
     * Frees the specified array container.
     */
    void array_container_free( __In ArrayContainer *container );

    /*
     * int array_container_get_max_count( ArrayContainer *container )
     *
     * Returns the current maximum size of the array container.
     */
    int array_container_get_max_count( __In ArrayContainer *container );

    /*
     * int array_container_get( ArrayContainer *container, int pos )
     * int array_container_get_next( ArrayContainer *container )
     * int array_container_get_prev( ArrayContainer *container )
     *
     * Fetchs the array container's data at an arbitrary position, or in a sequential order.
     */
    int array_container_get_next( __In ArrayContainer *container );
    int array_container_get_prev( __In ArrayContainer *container );
    int array_container_get( __In ArrayContainer *container, __In int pos );

    /*
     * void num_parse_delim( ArrayContainer *container, char *content, char delimiter )
     *
     * Parses out an array of numbers seperated with delimiters into an array container.
     */
    void num_parse_delim( __In __Out ArrayContainer *container, __In char *content, __In char delimiter );
    void num_parse_delim( __In __Out ArrayContainer *container, __In char *content );
}

__namespace(text) {
    /*
     * void txt_clear( Text *object )
     *
     * Clears a text object's strings without also removing the text.
     */
	void txt_clear( __In __Out Text *object );

    /*
     * void txt_remove_ex( Text *object )
     *
     * An extension to txt_remove() which removes all strings associated with the text object.
     */
	void txt_remove_ex( __In __Out Text *object );

    /*
     * float txt_width( Text *object )
     *
     * Returns the width of the text object.
     */
	float txt_width( __In Text *object );

     /*
      * float txt_height( Text *object )
      *
      * Returns the total height of the text.
      */
	float txt_height( __In Text *object );
}

__namespace(object) {
    /*
     * bool object_in_region( Object *object, const String *region )
     *
     * Tests if a specified object lies within a region.
     * Regions must be drawn before in WED and have a name.
     * Returns true if the object lies completely or partially inside the region, false otherwise.
     * For more control over the return result, use the raw region_check() call. It has a separate return value
     * for object partially lying inside the region.
     */
	bool object_in_region( __In Object *object, __In const String *region );

    /*
     * void object_bound_srhink( Object *object )
     *
     * Shrink to fit the object's bounding box.
     */
	void object_bound_srhink( __In __Out Object *object );

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
    void object_bound_draw( __In __Out Object *object, __In const Vector3 *color );
	void object_bound_draw( __In __Out Object *object );

    /*
     * void object_bound_get( Object *object, Vector3 *min, Vector3 *max )
     *
     * Retrieves the upper and lower bounds of a object's bounding box and pass it to
     * the out parameters.
     * Retrieving individual bounds is also possible, simply pass NULL to which argument you want to ignore.
     */
	void object_bound_get( __In __Out Object *object, __In __Out Vector3 *min, __In __Out Vector3 *max );

    /*
     * void object_scale_set( Object *object, float scale )
     *
     * Uniformly scaling an object's proportion (pro-portion).
     * For doubling the object's scale: object_scale_set(object, object->scale_x * 2)
     */
	void object_scale_set( __In Object *object, __In float scale );

    /*
     * void object_place( Object *object, float modifier )
     *
     * Places the object on a ground surface (terrain entity or blocks), mainly
     * to avoid tedious object placement in WED, where you have to manually place
     * them.
     * The second parameter is for fine-tuning the result (play with this value!).
     */
	void object_place( __In __Out Object *object, __In float modifier );

    /*
     * Object *object_sky_create( const String *file, const int layer )
     *
     * Quick-creates a sky cube.
     */
	Object *object_sky_create( __In const String *file, __In const int layer );

    /*
     * void object_draw(void *ptr, float dtime)
     *
     * Draws an object for a fixed amount of seconds.
     * Because it uses draw_obj(), it can draw any kind of
     * engine objects (text, panel, view entity).
     */
     void object_draw( __In void *ptr, __In float dtime);
     void object_draw( __In void *ptr );
}

__namespace(color) {
    /*
     * Vector *bgr_to_rgb(Vector *rgb)
     * void    bgr_to_rgb(Vector *rgb)
     *
     * Returns a RGB color vector from a BGR one.
     */
	Vector3 *bgr_to_rgb( __In Vector3 *rgb );
	void     bgr_to_rgb( __In Vector3 *rgb );

    /*
     * VECTOR *hex_to_rgb( int color )
     *
     * Converts a hexadecimal color to the equivalent RGB representation.
     */
    VECTOR *hex_to_rgb( __In int color );
}

#include "utilities.c"
#endif /* utilities.h */
