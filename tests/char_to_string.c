/* char_to_string.c */
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
#include <acknex.h>
#include <stdio.h>

#include "__path_source_test.h"
#include "../source/utils/utilities.h"

int main( int argc, char **argl )
{
	while( !ready() ) wait(1.0);
	
	const char *cstr = MALLOC(12, char);
	PRINT_INT(strlen(cstr));
	
	const STRING *sstr = str_create(cstr);
	PRINT_INT(str_len(sstr));
	
	FREE(cstr);
	
	return 0;
}
