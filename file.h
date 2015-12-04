/* file.h */
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
 * <file>
 * Merely a wrapper for Gamestudio's file_*() family of functions with 
 * the intention of making I/O easier to work with.
 * 
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    _FILE_H_
#define    _FILE_H_

#define PRAGMA_PRINT "\nFile I/O module initialization..."

#define __static 
#define __In 
#define __Out 

#define FILE_NAME_MAX_LENGTH              260 /* https://msdn.microsoft.com/en-us/library/system.io.PathTooLongException.aspx */
#define READ                              (1 << 1)
#define WRITE                             (1 << 2)

typedef struct {
	char *name;            /* Name of the file. */
	fixed __handle;        /* The actual file handle, which is returned through the file_open_*() instructions. */
	BOOL ready;            /* Ready, if the file was successfully opened for I/O, false otherwise. */
	BOOL unicode;          /* Unicode-ness. Normally the user is responsible for determining if the file contains Unicode data and
	                       /* then pass a flag to the struct, but in future (maybe..) I'll give it the ability to detect Unicode automatically. (TODO) */
	int mode;              /* Current operating mode. Either READ or WRITE. This flag has the sole reason to prevent crashes */
	                       /* for example, do file_str_write() on file_open_read()-file. */
} File;

File *File_active = NULL;

void delimiter_set(__In const char *cstr );
void delimiter_reset();
void delimiter_ignore();
void delimiter_write( __In File *file );
void delimiter_write();
const STRING *delimiter_get();

void file_bind( __In File *file );

File *file_new( __In const char *name, __In int process_mode, __In BOOL unicode );
File *file_new( __In const char *name, __In int process_mode );
File *file_new( __In const char *name );

BOOL file_is_ready( __In File *file );
BOOL file_is_ready();

BOOL file_is_unicode( __In File *file );
BOOL file_is_unicode();

void file_free( __In __Out File *file );
void file_free();

int file_mode_get( __In File *file );
int file_mode_get();

fixed file_handle_get( __In File *file );
fixed file_handle_get();

int file_size_get( __In File *file );
int file_size_get();

char *file_extension_get( __In File *file );
char *file_extension_get ( __In const char *fn );
char *file_extension_get();

const char *file_name_get( __In File *file );
const char *file_name_get();

void file_write_new_line( __In File *file );
void file_write_new_line();

/*
void file_read_new_line( __In File *file );
void file_read_new_line();
*/

void file_write_unicode( __In File *file, __In STRING *ustr );
void file_write( __In File *file, __In STRING *cstr );
void file_write( __In File *file, __In float f );

void file_write_unicode( __In STRING *ustr );
void file_write( __In STRING *cstr );
void file_write( __In float f );

STRING *       file_read_string( __In File *file );
float          file_read_float( __In File *file );

STRING *       file_read_string();
float          file_read_float();

#include "file.c"
#endif /* file.h */
