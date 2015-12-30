/* file.c */
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

__static const char *__combine(const char *cstr)
{
	STRING *name = str_create(cstr);
	STRING *number = str_create("#4");
	str_for_int(number, (int) rand() % (1000 + 1)); // number = [1; 1000]
	str_cat(name, number);

	return name->chars;
}

/*
 * void file_bind( File *file )
 *
 * Binds a specified file object as the active file object.
 */
void file_bind( File *file )
{
	if(file)
	    File_active = file;
}

/*
 * fixed file_handle_get( File *file )
 *
 * Retrieves the raw file handle for a specific file object.
 * You can then make bare-metal Gamestudio's file_*() calls on this handle.
 * Omit the parameter to retrieve the handle of the currently bound file object.
 */
fixed file_handle_get( File *file )
{
	return ifelse(file_is_ready(file), file->__handle, 0);
}

fixed file_handle_get()
{
	return file_handle_get(File_active);
}

/*
 * int file_size_get( File *file )
 *
 * Returns the actual file size of the file object.
 * (not sizeof(), but the size of the file in bytes as stored on the physical disk).
 * Omit the parameter to reveal a free PAYDAY 2 activation code.
 */
int file_size_get( File *file )
{
	return ifelse( file_is_ready(file) , (int) file_length(file->__handle), 0);
}

int file_size_get()
{
	return file_size_get(File_active);
}

/*
 * const char *file_name_get( File *file )
 *
 * Retrieves the file name of the (currently bound file object, if the parameter is omitted) file object.
 */
const char *file_name_get( File *file )
{
	if( file_is_ready(file) )    return file->name;
	                             return NULL;
}

const char *file_name_get()
{
	return file_name_get(File_active);
}

/*
 * char *file_extension_get ( File *file )
 *
 * Returns the file extension of a specified file object.
 * Returns NULL if the file doesn't have extensions.
 */
char *file_extension_get ( File *f )
{
	if(f)
	    #ifdef    _UTILITIES_H_ // If <utilities> was included
	        return file_get_ext(f->name); // Use the facility provided in <utilities> instead
	    #else
	        return file_extension_get (f->name); // Roll our own version
	    #endif
}

char *file_extension_get ( const char *fn )
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

char *file_extension_get()
{
	return file_extension_get(File_active);
}

/*
 * File *file_new( const char *name, int process_mode, BOOL unicode )
 *
 * Allocates a new file object.
 * File objects allocated this way must be freed later with the file_free() instruction.
 *
 * Passing empty file name cause __combine() to be invoked, which will create a temporary
 * file name for the file object. It's way slower than just passing a straight const char *.
 * You should only be using temporary names for working with temporary files that gets
 * deleted later, not for persistent storage.
 *
 * Additional variants are provided below.
 */
File *file_new( const char *name, int process_mode, BOOL unicode )
{
	STRING *__name = str_create(name);
	
	File *f = (File *) sys_malloc(sizeof(File));
	f->name = (char *) sys_malloc(FILE_NAME_MAX_LENGTH * sizeof(char));

	/* strcpy(f->name, ifelse(__name, __name, __combine("out")) ); */
	if(__name)
		strcpy(f->name, __name->chars);
	else 
		strcpy(f->name, __combine("out"));

	f->unicode = ifelse(unicode, true, false);
	f->mode = process_mode;

	if(process_mode == READ)
	    f->__handle = file_open_read(f->name);
	else
	    f->__handle = file_open_write(f->name);

	if( !f->__handle ) f->ready = false;
	else               f->ready = true;

	return f;
}

File *file_new( const char *name, int process_mode )
{
	return file_new(name, process_mode, false);
}

File *file_new( const char *name )
{
	return file_new(name, WRITE, false);
}

/*
 * BOOL file_is_ready( File *file )
 *
 * Returns true if the file is ready for further operations. (read/write), false otherwise.
 *
 * (2.12.2015): Fixed a bug where the compiler would evaluate both statements, thus if
 * "file" isn't valid, you'll get a crash (because the second statement in the clause would get evaluated too).
 */
BOOL file_is_ready( File *file )
{
	BOOL ret = false;

	if(file)
	    if(file->ready)
	        ret = true;

	return ret;
}

BOOL file_is_ready()
{
	return file_is_ready(File_active);
}

/*
 * BOOL file_is_unicode( File *file )
 *
 * Returns true if the file object supports Unicode processing (read/write).
 *
 * This is merely a flag that is used to mark the object as unicode-compliant.
 * Users are responsible for indicating and manually marking this flag.
 * Otherwise bunnies would jump out of the cage and come hunting for you at night.
 */
BOOL file_is_unicode( File *file )
{
	BOOL ret = false;

	if(file)
	    if(file->unicode)
	        ret = true;

	return ret;
}

BOOL file_is_unicode()
{
	return file_is_unicode(File_active);
}

/*
 * void file_free( File *file )
 *
 * Frees a specified file object.
 */
void file_free( File *file )
{
	if(file)
	{
		file_close(file->__handle);
		sys_free(file->name);
		sys_free(file);

		file = NULL;
	}
}

void file_free()
{
	file_free(File_active);
}

/*
 * void file_write_new_line( File *file )
 *
 * Writes a new line. This is for conventional uses only and -
 * you should use delimiters for separating data chunks instead.
 */
void file_write_new_line( File *file )
{
	if(file_is_ready(file))
	{
		// Solution 1: Traditional way
		file_asc_write(file->__handle, 13); // CR
		file_asc_write(file->__handle, 10); // LF

		// Solution 2: I-can't-believe-it-works way
		// file_str_write(file->__handle,"\r\n");

		// Solution 3: The Welcome to Facebook way - behold!:
		/*
		file_str_write(file->__handle,"
");
		*/
	}
}

void file_write_new_line()
{
	file_write_new_line(File_active);
}

/*
 * void file_read_new_line( File *file )
 *
 * Reads a new line.
 * Currently only supports Windows line ending style (which means CR+LF = 13+10).
 * This function is __bad__ and deprecated.
 * The reason is, Gamestudio doesn't have a unified way to detect the new line character pair
 * properly. Sometimes file_asc_read() returns the 13/10 pair but most time it doesn't, although
 * I've tried lots of newline-ing methods laid out above (file_write_new_line()), but still failed.
 * So I just ditched it, and future functions like getting number of lines, streaming files and
 * writing data streams on specific lines won't be implemented until Gamestudio developers
 * taken care of this issue. (maybe forever).
 *
 * Thankfully, it can detect delimiters and skip newlines correctly, so you better off using
 * delimiters instead.
 */
void file_read_new_line( File *file )
{
	if(file_is_ready(file))
	{
		int marker[2];
		int pos = file_seek(file->__handle, NULL, 4);

		marker[0] = file_asc_read(file->__handle); // CR
		marker[1] = file_asc_read(file->__handle); // LF

		if(marker[0] == 13 && marker[1] == 10) return; // A new line found.
		else // Reset.
		{
			file_seek(file->__handle, pos, 0);
		}
	}
}

void file_read_new_line()
{
	file_read_new_line(File_active);
}

__static void __file_write(File *f, STRING *sstr) // This exists to avoid the double-check of file_is_ready() in the file_write*() functions.
{
	if( (file_mode_get(f) == WRITE) ) file_str_write(f->__handle, sstr);
}

/*
 * void file_write( ..., const char *cstr )
 * void file_write_unicode( ..., STRING *ustr )
 * void file_write( ..., float f )
 *
 * Functions to write various data types to file objects,
 * with an additional unicode variant for writing unicode strings.
 */
void file_write( File *file, STRING *cstr )
{
	if( file_is_ready(file) )
	    __file_write(file, str_create(cstr));
}

void file_write_unicode( File *file, STRING *ustr )
{
	if( file_is_ready(file) )
	{
		if( file_is_unicode(file) )
		    __file_write(file, str_createw(ustr));
		else
		    __file_write(file, str_create(ustr));
	}
}

void file_write( File *file, float f )
{
	if( file_is_ready(file) && (file_mode_get(file) == WRITE) )
	    file_var_write(file->__handle, f);
}

void file_write_unicode( STRING *ustr )
{
	file_write_unicode(File_active, ustr);
}

void file_write( STRING *cstr )
{
	file_write(File_active, cstr);
}

void file_write( float f )
{
	file_write(File_active, f);
}

__static STRING *__file_read_string(File *file, BOOL unicode) // Read a string from a file, taking Unicode into account.
{
	STRING *buffer = NULL;

	if( file_is_ready(file) && (file_mode_get(file) == READ) )
	{
		if( unicode )
		{
			buffer = str_createw("#384");
			file_str_readtow( file->__handle, buffer, _chr(delimiter_get()), 384 );
		}
		else
		{
			buffer = str_create("#384");
			file_str_readto( file->__handle, buffer, _chr(delimiter_get()), 384 );
		}
	}
	return buffer;
}

__static float __file_read_float(File *file)
{
	if( file_is_ready(file) && (file_mode_get(file) == READ) ) {
	    // return (float) file_var_read(file->__handle);
	    // Tricky way to get around the "sensitive" delimiters, e.g. '-' - cause the numbers to become negative
	    // (or positive) floats due to the delimiter.
	    String *sstr = str_create("#24");
	    file_str_readto(file->__handle, sstr, delimiter_get(), 24);

	    float __result = str_to_float(sstr);
	    str_remove(sstr);
	    return __result;
	}
}

/*
 * <...> file_read_string( File *file )
 *
 * Read a string from a file object.
 * Other variants provided below.
 */
STRING *file_read_string( File *file )
{
	return __file_read_string(file, file_is_unicode(file) );
}

float file_read_float( File *file )
{
	return __file_read_float(file);
}

STRING *file_read_string()
{
	return __file_read_string(File_active, file_is_unicode(File_active));
}

float file_read_float()
{
	return __file_read_float(File_active);
}

/*
 * void delimiter_set(const char *cstr )
 *
 * Sets the delimiter. Delimiters are used to break a large string into several small strings,
 * or if you want to read data field-by-field (useful for CSV and such things).
 */
void delimiter_set(const char *cstr )
{
	str_cpy(delimit_str, cstr);
}

/*
 * void delimiter_reset()
 *
 * Resets the delimiter to the default character.
 * According to the manual, it is the "," character.
 */
void delimiter_reset()
{
	str_cpy(delimit_str, ",");
}

/*
 * void delimiter_ignore()
 *
 * Ignore the delimiter.
 * Particularly useful if you want to read data without delimiters blocking the way.
 * Remember to set back the delimiter with delimiter_set() afterwards.
 */
void delimiter_ignore()
{
	str_cpy(delimit_str, "");
}

/*
 * const STRING *delimiter_get()
 *
 * Returns the currently in-use delimiter character.
 */
const STRING *delimiter_get()
{
	return delimit_str;
}

/*
 * void delimiter_write( File *file )
 *
 * Explicitly writes the delimiter to a file object.
 */
void delimiter_write( File *file )
{
	if( file_is_ready(file) && (file_mode_get(file) == WRITE) )
	{
		file_str_write(file->__handle, delimit_str);
	}
}

void delimiter_write()
{
	delimiter_write(File_active);
}

/*
 * int file_mode_get( File *file )
 *
 * Returns the current operation mode of the file object (READ/WRITE).
 * Can be used to prevent certain situations, e.g. performing file_write*() functions on read-only file objects.
 * (this is done already in the implementation).
 */
int file_mode_get( File *file )
{
	if(file)
	    return file->mode;
}

int file_mode_get()
{
	return file_mode_get(File_active);
}
