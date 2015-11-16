/* serializer_vector.c */
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
#include "..\utilities.h"
#include "..\serializer.h"

void quit() { sys_exit( (void *) 0 ); }

int main(void )
{
	video_screen = 0;
	random_seed(0);
	BIND_KEY(esc, quit);
	
	Archive *arch = archive_new(1);
	
	int i = 0;
	for(i = 0; i < 10; ++i)
	{
		archive_store_float(arch, i + 1 + random(42.42));
		archive_store_int(arch, i + 42);
		archive_store_string(arch, tmpnam(NULL));	
	}
	
	printf("\n");
	for(i = 1; i <= archive_get_float_size(arch); ++i)
	printf("archive_get_float(arch, %i) = %f\n", i, (double) archive_get_float(arch, i));
	
	printf("\n");
	for(i = 1; i <= archive_get_int_size(arch); ++i)
	printf("archive_get_int(arch, %i) = %i\n", i, (int) archive_get_int(arch, i));
	
	printf("\n");
	for(i = 1; i <= archive_get_string_size(arch); ++i)
	printf("archive_get_string(arch, %i) = %s\n", i, (char *) _chr(archive_get_string(arch, i)) );
}
