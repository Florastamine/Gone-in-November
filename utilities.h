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
#define __In
#define __Out

#define Pi (3.141592) /* After more than 1.5hrs of surfing around the Internet, watching Pink Panther. */

#define __namespace(namespace)             {}
#define ARRAY_LENGTH(array)                (sizeof(array)/sizeof(array[0]))
#define MALLOC(number, type)               ((type *) malloc(number * sizeof(type)))
#define ASSERT(condition, message)         do { if( !(condition) ) __assert(message); } while(false)
#define STATIC_ASSERT(condition)           { if( !(condition) ) { /*##*/ }
#define WAIT_PROCESS(process)              while( proc_status(process) ) wait(1.0)
#define KILL_PROCESS(process)              proc_kill(4) /* This macro exists because it helps eliminating magic numbers. */
#define DEFER(argument)                    wait(1.0)
#define WALK_THROUGH(object, function)     object = ptr_first(object); while(object) { function(object); o = o.link.next; wait(1.0); }
#define SWAP(a, b, type)                   { type X = a; a = b; b = X; } while(false)
#define UNLESS(condition)                  if( !(condition) )

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

typedef STRING                             String;
typedef PANEL                              Panel;

__namespace(pair) {
	typedef struct {
		float first;
		float second;
	} pair;
	
	void pair_set( __In __Out pair *p, float first, float second );
	void pair_swap( __In __Out pair *p1, __In __Out pair *p2 );
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
}

__namespace(string) {
	String *str_clip_ex( __In String *str, __In int n );
}

__namespace(entity) {
}

#include "utilities.c"
#endif /* utilities.h */
