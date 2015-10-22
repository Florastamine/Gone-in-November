/* DPFL.c */
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
#include "utilities.h"

int main( int argc, char **argl )
{
	//split(NULL, 234532578);
	//int a[8];
	//printf("%i", (int)sizeof(a));
	//int *p = malloc(4);
	//printf("%i", p);
	int *a = (int *) malloc(4 * 5);
	split(a, 12345);
	reverse(a, 5);
	
	int i = 0;
	while(i < 5)
	{
		printf("%i", (int) *(a + i));
		i++;
		
		wait(1.0);
	}
	
	return 0;
}
