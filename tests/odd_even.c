/* odd_even.c */
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

void quit() { sys_exit( (void *) 0 ); }

int main()
{
	video_screen = 0;
	printf("\n");
	
	int i = 0;
	while(i < 17)
	{
		printf("%i is %s.\n", i, ifelse(i & 1, _chr("odd"), _chr("even") ) );
		
		i++;
		wait(1.0);
	}
	
	on_esc = quit;
}
