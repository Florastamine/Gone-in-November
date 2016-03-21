/* region.h */
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
 * <region>
 * A *very* basic header that was written to fill in the currently missing
 * built-in feature of Gamestudio: multilingual.
 * I could write a csv-parser instead but searching for the correct tag
 * requires a very fast string algorithm.
 *
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 */
#ifndef    _REGION_H_
#define    _REGION_H_

#include <strio.c>

#define PRAGMA_PRINT "\nMultilingual module initialization..."

#define __In
#define __Out
#define __static

#define __namespace(namespace)             {}

__namespace(multilingual) {
	#define    MAX_LANGUAGES          8

	#define    REGION_LANGUAGE_END_MARK   "--end--"

	#define    REGION_UNICODE_PADDING     "unicode.pad"
	#define    REGION_LANGUAGE_LIST       "__language.pad"
	#define    REGION_DEFAULT_DIRECTORY   "./translation/"
	#define    REGION_LANGUAGE_FLAG_FILE  "flag.jpg"

	typedef struct {
		String    *__search_path;          /* A string which stores the initial search path. */

		Text      *languages;              /* Contains all found languages during region_scan*(). */
		String    *language_active;        /* Contains the active language (in string form - two characters describing the language: vn/en/jp/... */
		                                   /* and an additional character '@' to mark the Unicode-ness of the language.                           */
		int        language_count;         /* Contains the number of found languages during region_scan*(). */
	} Region;

	Region *Region_singleton = NULL;

	Region *region_get_singleton();
	void region_new();
	void region_free();

	void region_scan();
	void region_scan_from( __In const STRING *directory );

	STRING *region_get_string( __In const STRING *tag, __In int length );
	STRING *region_get_string( __In const STRING *tag );
	STRING *region_get_languages();

	BMAP *region_get_language_flag( __In const STRING *language );
	BMAP *region_get_language_flag();

	BOOL region_set_language( __In const STRING *language );
	STRING *region_get_language();

	BOOL region_get_unicode( __In const STRING *language );
}

#include "region.c"
#endif /* region.h */
