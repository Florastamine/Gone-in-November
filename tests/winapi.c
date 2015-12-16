/* winapi.c */
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

#define WINDOWS_API
#include "..\\source\\utils\\utilities.h"

int main(void)
{
	STRING *sprint = "";
	str_printf(sprint, "OS (Windows API independent): %s \nUser name: %s\nComputer name: %s\nLaunched with administrator rights: %i",
	os_get_name(),
	os_get_user_name(),
	os_get_computer_name(),
	(int) os_is_privileged());

	Text *drives = os_get_drives();
	drives->font = Arial_18;
	drives->pos_x = screen_size.x - 125.0;
	drives->pos_y = 10.0;
	drives->flags |= (SHOW);

	while( !key_esc )
	{
		draw_text(sprint, 10, 10, COLOR_BLEU_DE_FRANCE);

		wait(1.0);
	}

    RETURN(0);
}
