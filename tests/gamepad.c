/* gamepad.c */
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

int main(void)
{
	if(!num_joysticks)
	{
		while(!key_esc)
		{
			draw_text("Connect a gamepad/joystick device, then restart the demo.", 10.0, 10.0, COLOR_WHITE);
			wait(1.0);
		}
		sys_exit(0);
	}
	
	while( !key_esc )
	{
		draw_text(str_printf(NULL, "Detected gamepads: %i", (int) num_joysticks), 10.0, 10.0, COLOR_WHITE);
		draw_text(str_printf(NULL, "Binary indication for every button push: %.3f", (double) joy_buttons), 10.0, 40.0, COLOR_WHITE);
		
		draw_text(str_printf(NULL, "joy_force.x = %f\njoy_force.y = %f\njoy_force.z = %f", (double) joy_force.x, (double) joy_force.y, (double) joy_force.z), 10.0, 70.0, COLOR_WHITE);
		draw_text(str_printf(NULL, "joy_rot.x = %f\njoy_rot.y = %f\njoy_rot.z = %f", (double) joy_rot.x, (double) joy_rot.y, (double) joy_rot.z), 10.0, 150.0, COLOR_WHITE);
		draw_text(str_printf(NULL, "joy_raw.x = %f\njoy_raw.y = %f\njoy_raw.z = %f", (double) joy_raw.x, (double) joy_raw.y, (double) joy_raw.z), 10.0, 230.0, COLOR_WHITE);
		
		draw_text(str_printf(NULL, "joy_hat = %f", (double) joy_hat), 10.0, 300.0, COLOR_WHITE);
		
		wait(1.0);
	}
	
	sys_exit(0);
}
