/* region.c */
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
__static STRING *__region_get_path( STRING *language )
{
	STRING *str = str_create((region_get_singleton())->__search_path);
	str_cat(str, str_trim(language));
	str_cat(str, "/");
	
	return str;
}

__static STRING *__region_get_tag_file( STRING *file )
{
	STRING *str = __region_get_path(region_get_language());
	str_cat(str, str_create(file));
	
	return str;
}

/*
 * Region *region_get_singleton()
 * 
 * Returns the multilingual state singleton.
 * It can then be used to query various parameters like number of languages found, 
 * list of languages and the currently in use language.
 */
Region *region_get_singleton()
{
	return Region_singleton;
}

/*
 * void region_new()
 * 
 * Allocates and initializes a new region state.
 * To actually initializes the region state, you must call region_scan()
 * to find all available languages, and another call to region_set_language()
 * to select which language you want to use.
 */
void region_new()
{
	if(Region_singleton) region_free();
	
	Region_singleton                       = (Region *) sys_malloc(sizeof(Region));
	Region_singleton->languages            = txt_create(0, 42);
	Region_singleton->__search_path        = str_create("");
	Region_singleton->language_active      = str_create("#3"); // Only 3 characters.
	                                                           // The currently active language is stored as 2 characters ("vi", "en", "jp",... and such).
	                                                           // An additional '@' character is used to mark the existence of Unicode for that language (through the dummy unicode.pad).
	Region_singleton->language_count       = 0;
}

/*
 * void region_scan_from( const STRING *directory )
 * 
 * Explicitly scans available languages from a specified directory.
 * The directory itself *MUST* contain the following:
 * - Language directories, data, and the Unicode padding.
 * - generate_language_list.exe for generating __language.pad which contains a list of available languages.
 * - __language.pad (which is generated from generate_language_list.exe) to be parsed later.
 */
void region_scan_from( const STRING *directory )
{
	STRING *__directory   = str_create(directory);
	char separator        = str_getchr( __directory, str_len(__directory) );
	
	if( separator != '/' && separator != '\\' ) // In case someone forgot to put an additional '/' to distinguish directories from files.
	{
		str_cat( __directory, "/" );
	}
	
	str_cpy( (region_get_singleton())->__search_path, directory );
	
	STRING *language_list_file = ""; 
	str_cat(language_list_file, __directory);
	str_cat(language_list_file, REGION_LANGUAGE_LIST);
	
	// The problem with this code is it seems to doesn't invoke the batch script
	// with administrator rights (?) - it seems so, because the batch successfully executes 
	// without printing anything to stdout nor writing any files (in this case, __language.pad).
	// So this is apparently not feasible - I decided to switch to another option - compiles  
	// the script to a Windows executable and run it before launching the game.
	/*
	if( !file_exists(language_list_file) )
	{
		STRING *s = "";
		str_cat(s, __directory);
		str_cat(s, str_create("generate_language_list.bat"));
		str_replaceall( s, "/", "\\");
		
		exec_wait( s, NULL );
	}
	*/
	
	fixed f = file_open_read(language_list_file);
	
	STRING *cursor = "";
	while( true )
	{
		file_str_read(f, cursor);
		cursor = str_trim(cursor);
		
		if( !str_cmpi(cursor, REGION_LANGUAGE_END_MARK ) )
		{
			txt_addstring( (region_get_singleton())->languages, cursor); // Push a new language to the shelf.
			
			// Additionally check for Unicode presence
			if( region_get_unicode(cursor) )
			{
				str_cat( ((region_get_singleton())->languages->pstring)[(region_get_singleton())->language_count], "@"); // Append a '@' at the end
				                                                                                                         // Let others know this is a Unicode language.
			}
			
			(region_get_singleton())->language_count += 1;
		}
		else
		    break;
	}
	file_close(f);
}

void region_scan()
{
	region_scan_from(REGION_DEFAULT_DIRECTORY);
}

/*
 * void region_free()
 * 
 * Frees the region state singleton.
 */
void region_free()
{
	if(region_get_singleton())
	{
		// Remove the language list
		int i = 0;
		for(; i < (region_get_singleton())->language_count; i++)
		    str_remove( ((region_get_singleton())->languages->pstring)[i] );
		
		txt_remove( (region_get_singleton())->languages );
		
		// Remove the current active language and frees the singleton.
		str_remove( (region_get_singleton())->language_active );
		str_remove( (region_get_singleton())->__search_path );
		sys_free( region_get_singleton() );
	}
}

/*
 * STRING *region_get_languages()
 * 
 * Retrieves a list of found languages (through region_scan()), each connected by an underscore. For example:
 * "en_vn_jp" (without the double quotes).
 */
STRING *region_get_languages()
{
	STRING *s = NULL;
	
	if( region_get_singleton() )
	{
		s = str_create("");
		int i = 0;
		
		for(; i < (region_get_singleton())->language_count; i++)
		{
			str_cat( s, ((region_get_singleton())->languages->pstring)[i] );
			if(i < (region_get_singleton())->language_count - 1 ) str_cat( s, str_create("_") ); // Do not mark the underscore if we're doing for the last language.
		}
	}
	
	return s;
}

/*
 * BMAP *region_get_language_flag( const STRING *language )
 * 
 * Returns the flag bitmap object for the specified language, if it exists.
 * The file is named in <region>/REGION_LANGUAGE_FLAG_FILE (by default flag.jpg).
 */
BMAP *region_get_language_flag( const STRING *language )
{
	BMAP *b      = NULL;
	STRING *s    = __region_get_path(str_create(language));
	
	str_cat(s, REGION_LANGUAGE_FLAG_FILE);
	
	if(file_exists(s))    return bmap_create(s);
	                      return b;
}

BMAP *region_get_language_flag()
{
	return region_get_language_flag(region_get_language());
}

/*
 * STRING *region_get_string( const STRING *tag, int length )
 * 
 * Fetchs the content of the given tag (file name). The tag must exists.
 * The Unicode-ness of the return string is per-language, not per-file. This mean for any 
 * Unicode language, the format of the file name (the tag) must all be the same (Unicode encoding),
 * otherwise the strings won't be read correctly.
 */
STRING *region_get_string( const STRING *tag, int length )
{
	STRING *s         = __region_get_tag_file(str_create(tag));
	STRING *content   = NULL;
	if( length < 0 ) length = 1000;
	
	if( file_exists(s) ) {
		fixed f = file_open_read(s);
		content = str_create("");
		
		if( region_get_unicode(region_get_language()) )
		{
			content = str_createw("");
			file_str_readtow(f, content, "", length);
		}
		else
		{
			content = str_create("");
			file_str_readto(f, content, "", length);
		}
		
		file_close(f);
	}
	
	return content;
}

STRING *region_get_string( const STRING *tag )
{
	return region_get_string(tag, - 42);
}

/*
 * STRING *region_get_language()
 * 
 * Returns the currently active language.
 */
STRING *region_get_language()
{
	return (region_get_singleton())->language_active;
}

/*
 * BOOL region_set_language( const STRING *language )
 * 
 * Sets active a language. The language itself must exists.
 * Returns true if the language was successfully set, false otherwise.
 */
BOOL region_set_language( const STRING *language )
{
	if( !language ) return false;
	
	BOOL b = false;
	STRING *s       = str_create(language);
	STRING *list    = region_get_languages();
	
	if( region_get_singleton() )
	{
		if( str_stri(list, s) > 0 )
		{
			str_cpy((region_get_singleton())->language_active, s);
			b = true;
		} 
	}
	
	return b;
}

/*
 * BOOL region_get_unicode( const STRING *language )
 * 
 * Returns true if the specified language is Unicode, false otherwise.
 * This can done by either checking the last character of the language string,
 * or test if the unicode padding file exists.
 */
BOOL region_get_unicode( const STRING *language )
{
	BOOL ret = false;	
	STRING *unicode_pad_file = "";
	
	str_cpy(unicode_pad_file, __region_get_path(str_create(language)));
	str_cat(unicode_pad_file, REGION_UNICODE_PADDING);
	
	if( file_exists(unicode_pad_file) ) ret = true;
	
	return ret;
}
