/* serializer.c */
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
 * It's a horrible mess filled with weird-ass macros and syntax. Do not attempt to read this as your eyes will bleed badly. 
 * To put it to words: Here be dragons!
 * Just joking, it's not all that bad. But because I couldn't take advantage of templates, and writing the same code with different data types 
 * everywhere is suck, I decided to rewrote the same parts (just with different data types) in macros, and call them in appropriate functions.
 * This style of writing code is horrible (macro abusing anyone?), but no one ever reads it anyway.
 * ...
 * ...
 * ...
 * Even 2 years or one year later when someone interviewed me for a (better) job and he looked through this piece of code. 
 * Even he.
 * Even he doesn't care.
 * What? I hear you said casting/inlining? Heck, forget about it.
 */

#define __HEADER(type)                                  type temp = 0; type *backup = MALLOC(copyn, type)
#define __REALLOC_MEMORY_AND_PUSH(type, value)          int new_size = arch->pos_##type + 1; arch->pointer_##type = __memory_realloc_##type( &(arch->pointer_##type), new_size, new_size ); if(arch->pointer_##type) *(arch->pointer_##type + new_size - 1) = value
#define __GET_SIZE_RETURN(type)                         int ret = 0; if(arch) ret = arch->pos_##type; return ret
#define __SET_VALUE(type, value)                        if( pos <= archive_get_##type##_size(arch) ) *(arch->pointer_##type + pos - 1) = value
#define __GET_VALUE(type, pos)                          if( pos <= archive_get_##type##_size(arch) ) return *(arch->pointer_##type + pos - 1)

__static int __archive_get_order( Archive *arch )
{
	return arch->__order_pointer;
}

__static int __archive_increase_order( Archive *arch )
{
	arch->__order_pointer += 1;
}

__static void __archive_push_order( Archive *arch, int what )
{
	int current_pos = __archive_get_order(arch);
	
	switch(what)
	{
		case    FLOAT : (arch->__order)[current_pos] = FLOAT;     break;
		case  INTEGER : (arch->__order)[current_pos] = INTEGER;   break;
		case __STRING : (arch->__order)[current_pos] = __STRING;  break;
	}
	__archive_increase_order(arch);
}

__static float *__memory_realloc_float( float **in, int n, int copyn )
{
	__HEADER(float);	
	
	*backup = **in;
	temp = **in;
	copy( &backup, in, copyn - 1 );
	
	float *w = REALLOC( *in, float, n);

	if(w != NULL)
	{
		*w = temp;
		copy( &w, &backup, copyn - 1 );
	}
	else
	w = *in;

	return w;
}

__static int *__memory_realloc_int( int **in, int n, int copyn )
{
	__HEADER(int);
	
	*backup = **in;
	temp = **in;
	copy( &backup, in, copyn - 1 );
	
	int *u = REALLOC( *in, int, n );
	if(u != NULL )
	{
		*u = temp;
		copy( &u, &backup, copyn - 1 );
	}
	else
	u = *in;
	
	return u;
}

/*
 * Archive *archive_new( int __size )
 * 
 * Allocates and initializes a new archive for serialization/deserialization.
 * The first parameter gives the initial size of the archive. Must be at least one element.
 * The archive automagically grows when needed.
 * 
 * Archives initialized this way must be freed later using archive_free().
 */
Archive *archive_new( int __size )
{
	__size = (int) ifelse( __size > 0, __size, 1 );
	
	Archive *arch              = MALLOC(1, Archive);
	
	arch->pointer_string       = txt_create(__size, 1);
	arch->pointer_float        = MALLOC(__size, float);
	arch->pointer_int          = MALLOC(__size, int);
	arch->pos_string           = 0;
	arch->pos_float            = 0;
	arch->pos_int              = 0;
	
	arch->__initial_size       = __size;
	arch->__order_pointer     = 0;
	/* memset( arch->__order, 0, MAX_PUSH * sizeof(float) ); */
	
	return arch;
}

/*
 * void archive_free( Archive *arch )
 * 
 * Frees an archive. All data are wiped away, and gone forever.
 */
void archive_free( Archive *arch )
{
	if(arch)
	{
		int i = 0;
		
		for(; i < arch->pointer_string->strings; i++)
		    str_remove((arch->pointer_string->pstring)[i]);
		
		txt_remove(arch->pointer_string);
		
		FREE(arch->pointer_float);
		FREE(arch->pointer_int);
		
		FREE(arch);
	}
}

/*
 * void archive_store_float(Archive *arch, float f)
 * 
 * Pushes a float value to the archive.
 */
void archive_store_float( Archive *arch, float data )
{
	if( !arch ) return;
	
	if(arch->pos_float <= arch->__initial_size - 1)
	    *(arch->pointer_float + arch->pos_float) = data;
	else 
	    { __REALLOC_MEMORY_AND_PUSH( float, data ); }

	(arch->pos_float) += 1;
	
	// The New Order
	__archive_push_order(arch, FLOAT);
}

/*
 * void archive_store_int( Archive *arch, int i )
 * 
 * Pushes an integer to the archive.
 */
void archive_store_int( Archive *arch, int i )
{
	if( !arch ) return;
	
	if(arch->pos_int <= arch->__initial_size - 1)
	    *(arch->pointer_int + arch->pos_int) = i;
	else
	    { __REALLOC_MEMORY_AND_PUSH( int, i ); }
	
	(arch->pos_int) += 1;
	
	__archive_push_order(arch, INTEGER);
}

/*
 * void archive_store_string( Archive *b, String *content )
 * 
 * Pushes a string to the archive.
 * Thanks to the built-in txt_addstring(), otherwise an additional significant work 
 * would have been put into making this work.
 */
void archive_store_string( Archive *b, String *content )
{
	if(b)
	{
		if( b->pos_string < b->__initial_size )
		(b->pointer_string->pstring)[b->pos_string] = str_create(content);
		else
		txt_addstring( b->pointer_string, str_create(content) );
		
		(b->pos_string)++;
		__archive_push_order(b, __STRING);
	}
}

/*
 * void archive_set_string( Archive *arch, STRING *data, int pos )
 * 
 * Modifies a previously pushed string from the archive at an arbitrary position.
 */
void archive_set_string( Archive *arch, STRING *data, int pos )
{
	if( !data || !arch ) return;
	
	if( pos <= archive_get_string_size(arch) )
	{
		(arch->pointer_string->pstring)[pos - 1] = str_create(data);
	}
}

/*
 * void archive_set_float( Archive *arch, float data, int pos )
 * 
 * Modifies a previously pushed float value from the archive at an arbitrary position.
 */
void archive_set_float( Archive *arch, float data, int pos )
{
	if( !arch ) return;
	
	__SET_VALUE(float, data);
}

/*
 * void archive_set_int( Archive *arch, int data, int pos )
 * 
 * Modifies a previously pushed integer value from the archive at an arbitrary position.
 */
void archive_set_int( Archive *arch, int data, int pos )
{
	if( !arch ) return;
	
	__SET_VALUE(int, data);
}

/*
 * int archive_get_initial_size( Archive *arch )
 * 
 * Retrieves the initial size of the archive (the argument which you passed to archive_new()).
 */
int archive_get_initial_size( Archive *arch )
{
	int ret = 0;
	if(arch) ret = arch->__initial_size;
	
	return ret;
}

/*
 * int archive_get_int( Archive *arch, int pos )
 * 
 * Retrieves a previously pushed integer value from the archive, at an arbitrary position.
 */
int archive_get_int( Archive *arch, int pos )
{
	if( !arch ) return;
	
	__GET_VALUE(int, pos);
}

/*
 * STRING *archive_get_string( Archive *arch, int pos )
 * 
 * Retrieves a new copy of the previously pushed string from the archive, at an arbitrary position.
 */
STRING *archive_get_string( Archive *arch, int pos )
{
	if( !arch ) return NULL;
	
	return str_create((arch->pointer_string->pstring)[pos - 1]);
}

/*
 * float archive_get_float( Archive *arch, int pos )
 * 
 * Retrieves a previously pushed float value from the archive, at an arbitrary position.
 */
float archive_get_float( Archive *arch, int pos )
{
	if( !arch ) return;
	
	__GET_VALUE(float, pos);
}

/*
 * int archive_get_int_size ( Archive *arch )
 * 
 * Retrieves the current size of the archive's integer container.
 */
int archive_get_int_size ( Archive *arch )
{
	__GET_SIZE_RETURN(int);
}

/*
 * int archive_get_float_size( Archive *arch )
 * 
 * Retrieves the current size of the archive's float container.
 */
int archive_get_float_size( Archive *arch )
{
	__GET_SIZE_RETURN(float);
}

/*
 * int archive_get_string_size ( Archive *arch )
 * 
 * Retrieves the current size of the archive's string container 
 * (okay, not actually, because a text object is in the way).
 */
int archive_get_string_size ( Archive *arch )
{
	__GET_SIZE_RETURN(string);
}

/*
 * void serialize( Archive *arch, const char *file )
 * 
 * Performs serialization.
 */

/*
__static void __write_header( fixed channel, int mode )
{
	char *header = "<F#=";
	const char *criterion = "#";
	char *HEAD;
	
	HEAD = strstr(header, criterion);
	ASSERT( HEAD != NULL, _chr("<serializer>/__write_header() (__static void) failed at strstr().") );
	
	switch(mode)
	{
		case __WRITE_HEADER   : *HEAD = 'H'; break;
		case __WRITE_FLOAT    : *HEAD = 'F'; break;
		case __WRITE_INTEGER  : *HEAD = 'I'; break;
		case __WRITE_STRING   : *HEAD = 'S'; break;
	}
	
	file_str_write(channel, _str(header));	
	file_str_write(channel, _str("["));
}
*/

#define    __WRITE_DATA(type) for(counter = 0; counter <= archive_get_##type##_size(archf); counter++) { file_str_write(channel, str_for_int(NULL, (archf->__order)[counter])); if(counter < archive_get_##type##_size(archf) - 1) file_str_write(channel, ","); }

#define    __WRITE_HEADER    0
#define    __WRITE_FLOAT     1
#define    __WRITE_INTEGER   2
#define    __WRITE_STRING    3

__static void __write_header( fixed channel, int mode )
{
	switch(mode)
	{
		case __WRITE_HEADER   : file_str_write(channel, _str("<FH=[") ); break;
		case __WRITE_FLOAT    : file_str_write(channel, _str("<FF=[") ); break;
		case __WRITE_INTEGER  : file_str_write(channel, _str("<FI=[") ); break;
		case __WRITE_STRING   : file_str_write(channel, _str("<FS=[") ); break;
	}
}

__static void __write_footer( fixed channel )
{
	file_str_write(channel, _str("]"));
}

void serialize( Archive *archf, const char *file )
{
	if( archf == NULL || (file_exists(file) && file_length(file_open_read(file)) ) ) return;
	
	fixed channel = file_open_write(str_create(file));
	int counter = 0;

	__write_header(channel, __WRITE_HEADER);
	
	/* Write the header */
	file_str_write(channel, str_for_int(NULL, archf->__initial_size));
	file_str_write(channel, ",");
	
	/* Write the serializing orders. */
	
	while(counter < archf->__order_pointer)
	{
		file_str_write(channel, str_for_int(NULL, (archf->__order)[counter]));
		if(counter < __archive_get_order(archf) - 1) file_str_write(channel, ",");
		
		counter++;
	}
	__write_footer(channel);
	
	/* Checks for the existence of each data type in the archfive and, if yes, serialize it. */
	if( (int) archive_get_float_size(archf) > 0 )
	{
		__write_header(channel, __WRITE_FLOAT);
		
		__WRITE_DATA(float)
		__write_footer(channel);
	}
	
	if( (int) archive_get_int_size(archf) > 0 )
	{
		__write_header(channel, __WRITE_INTEGER);
		
		__WRITE_DATA(int)
		__write_footer(channel);
	}
	
	if( (int) archive_get_string_size(archf) > 0 )
	{
		__write_header(channel, __WRITE_STRING);
		
		for(counter = 1; counter <= archive_get_string_size(archf); counter++)
		{
			file_str_write(channel, (archf->pointer_string->pstring)[counter - 1] );
			if(counter < archive_get_string_size(archf) ) file_str_write(channel, "|");
		}
		__write_footer(channel);
	}
	
	file_close(channel);
}


