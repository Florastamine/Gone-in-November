/* strlen.c */
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

#include "__path_source_test.h"
#include "../source/utils/utilities.h"

int main( int argc, char **argl )
{
	String *sstr = str_create("This is a string!");
	PRINT_INT(str_len(sstr));
	
	str_cpy(sstr, " ");
	PRINT_INT(str_len(sstr));
	
	RETURN(0);
}
