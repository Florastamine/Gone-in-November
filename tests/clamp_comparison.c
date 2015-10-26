/* clamp_comparison.c */
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
#include "..\utilities.h"

const int inbound = 0;
const int outbound = 256;

void print(const int inbound, const int outbound)
{
	PRINT_BOOL( (int) clamp(inbound, 0, outbound) == inbound );
}

int main( int argc, char **argl )
{
	while( !ready() ) wait(1.0);
	
	print(inbound, outbound); // true
	print(inbound - 1, outbound); // false
	print(outbound + 1, outbound); // false
	print(outbound, outbound); // true
	print(outbound - 1, outbound); // true
	
	return 0;
}
