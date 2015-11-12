/* dtypes.c */
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

void print(const char *content, int i) { printf("%s%i\n", (char *) content, i ); }
void quit() { sys_exit( (void *) 0 ); }

int main(void )
{
	video_screen = 0;
	BIND_KEY(esc, quit);
	
	print("\n", NULL);
	print("sizeof(char) = ", sizeof(char));
	print("sizeof(char *) = ", sizeof(char *));
	
	print("sizeof(int) = ", sizeof(int));
	print("sizeof(int *) = ", sizeof(int *));
	
	print("sizeof(float) = ", sizeof(float));
	print("sizeof(float *) = ", sizeof(float *));
	
	print("sizeof(double) = ", sizeof(double));
	print("sizeof(double *) = ", sizeof(double *));
	
	print("sizeof(short) = ", sizeof(short));
	print("sizeof(short *) = ", sizeof(short *));
	
	print("sizeof(long) = ", sizeof(long));
	print("sizeof(long *) = ", sizeof(long *));
	
	print("sizeof(size_t) = ", sizeof(size_t));
	
	print("sizeof(STRING) = ", sizeof(STRING));
	print("sizeof(STRING * ) = ", sizeof(STRING *));
}
