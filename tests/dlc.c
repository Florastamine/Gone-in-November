/* dlc.c */
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
 * __________________________________________________________________
 *
 * <dlc>
 * A small experiment which tests the add_buffer() API. Unfortunately this doesn't work
 * (the buffer doesn't get freed somehow).
 * Issue posted here: http://www.opserver.de/ubb7/ubbthreads.php?ubb=showflat&Number=457189#Post457189
 *
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#include <acknex.h>
#include <default.c>

const STRING *description = "[Q]: Free the buffer.\n[W]: Load the buffer\n[E]: Load the model.";
ENTITY *guard = NULL;

void *model_buffer = NULL;
long model_size = 0;

void buffer_free();
void buffer_load();
void model_load();

void spin();

int main(void)
{
	on_w = buffer_load;
	on_q = buffer_free;
	on_e = model_load;

	level_load(0);
	vec_set(&camera->x, vector(-256, 0, 0));

	while( !key_esc )
	{
		draw_text(description, 10.0, 10.0, COLOR_WHITE);

		wait(1.0);
	}

	add_new(); // Remove the buffer before exit.
	sys_exit(0);
}

void buffer_free()
{
	add_buffer( "guard.mdl", NULL, model_size );   // Remove the buffer.
}

void buffer_load()
{
	model_buffer = file_load(".\\dlc\\guard.mdl", NULL, &model_size);

	if(model_buffer)
	    printf("model loaded with a total size of %f Kbytes.", (double) model_size / 1024);

	add_buffer( "guard.mdl", model_buffer, model_size );
}

void model_load()
{
	if(guard)
	    safe_remove(guard);

	guard = ent_create("guard.mdl", nullvector, spin);
}

void spin()
{
	while(my)
	{
		my->pan += 5.5 * time_step;
		wait(1.0);
	}
}
