/* encrypt_decrypt.c */
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
#define ENFORCE_STRICT

#include <acknex.h>
#include <d3d9.h>
#include <stdio.h>
#include <default.c>

#include "..\utilities.h"
#include "..\file.h"
#include "..\crypto.h"

int main( int argc, char **argl )
{
	while( !ready() ) wait(1.0);
	
	STRING *sstr = "This is my encrypted string.";
	STRING *estr = "";
	STRING *zstr = "";
	
	encrypt_string(sstr, estr);
	WAIT_PROCESS(encrypt_string);
	
	decrypt_string(estr, zstr);
	WAIT_PROCESS(decrypt_string);
	
	File *f = file_new("my_file.txt", WRITE);
	file_write(f, estr);
	file_write_new_line(f);
	file_write(f, zstr);
	file_free();
	
	while(true)
	{
		draw_text(estr, 5.0, 5.0, COLOR_TEAL);
		draw_text(zstr, 5.0, 20.0, COLOR_WHITE);
		
		wait(1.0);
	}
	
	return 0;
}
