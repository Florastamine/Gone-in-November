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
	BIND_KEY(esc, quit);
	
	Archive *arch = archive_new(1);
	
	int i = 0;
	for(; i < 10; ++i) archive_store_float(arch, i + 1);
	
	printf("\n");
	for(i = 1; i <= archive_get_float_size(arch); ++i)
	printf("archive_get_float(arch, %i) = %f\n", i, (double) archive_get_float(arch, i));
	
	printf("\n");
	
	for(i = 1; i <= archive_get_float_size(arch); ++i)
	archive_set_float(arch, (float) i * 2.0, i);
	printf("\n");
	
	for(i = 1; i <= archive_get_float_size(arch); ++i)
	printf("archive_get_float(arch, %i) = %f\n", i, (double) archive_get_float(arch, i));
}
