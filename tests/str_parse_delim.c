/* str_parse_delim.c */
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
#include <strio.c>

#include "__path_source_test.h"
#include "../source/utils/utilities.h"
#include "../source/utils/gui_utilities.h"

void quit() { sys_exit(0); }

int main()
{
	while( !ready() ) wait(1.0);
	BIND_KEY(esc, quit);
	
	String *str             = "This ; is ; a ; simple ; test ; which ; demonstrates ; str_parse_delim() ; more ; to ; come ; later.";
	char delimiter          = ';';
	
	Text *object            = txt_create(0, 1);
	object->font            = Arial_18;
	gui_text_set_pos(object, pair_new(10.0, 15.0));
	
	str_parse_delim(object, str, delimiter);
	
	object->flags |= (SHOW);
}
