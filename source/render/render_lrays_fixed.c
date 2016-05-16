/*
 * void render_light_rays_on(const ENTITY *what)
 *
 * Performs fake light ray rendering on a specified entity.
 * Doesn't make use of the vertex shader effects but
 * requires a special light ray model to be made in order to create beliveable effects.
 */
void render_light_rays_on(const ENTITY *what) {
	if(!(my->flags & PASSABLE))
		my->flags |= PASSABLE;

	if(!(my->flags & BRIGHT))
		my->flags |= BRIGHT;

	if(!(my->flags & TRANSLUCENT))
		my->flags |= (TRANSLUCENT);

	my->scale_z = 25.0;
	my->scale_y = random(2.0) + 0.5;
	my->skill1 = random(360.0);
	my->skill2 = random(4.0) + 10;

	while(!(my->skill3)) {
		vec_set(&my->pan, vector(camera->pan + 180.0, 0.0, 0.0));
		ang_add(&my->pan, vector(0.0, 35.0, 0.0));

		my->skill1 += my->skill2 * time_step;
		my->alpha = 12 * sin(my->skill1) + 2.0;

		wait(1.0);
	}
}
