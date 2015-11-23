/* default_ex.c */
/////////////////////////////////////////////////////////////////////
// Default key and debug functions
// (c) jcl / Conitec  2008
// Modified for Downpour/Gone in November by Huy Nguyen
// 
// This is solely a modification to the good ol' <default> - not an 
// upgraded version or whatsoever. My point is I want to maintain compatibility 
// with the old <default>.
// This header *only* modifies the original <default> by separating the file 
// into .h/.c (good practice), cut some (useless) features and make 
// the debug panel clearer and more readable.
/////////////////////////////////////////////////////////////////////
void def_console()
{
	def_ctxt->pos_x = 5;
	def_ctxt->pos_y = screen_size.y / 2;
	toggle(def_ctxt,SHOW);
	
	while is(def_ctxt,SHOW)
	{
		inkey((def_ctxt->pstring)[1]);
		if (13 == result)
		{
			void* found = var_for_name((def_ctxt->pstring)[1]);
			if (!found) execute((def_ctxt->pstring)[1]);
		} 
		else reset(def_ctxt,SHOW);
	}
}

// call this function from a level to enable the free camera movement
void def_move()
{
	VECTOR force,speed,dist;
	ANGLE aforce,aspeed; 

	// initialize speed and distance
	vec_zero(speed);
	vec_zero(aspeed);
	vec_zero(dist);

	if (1 > def_camera)
	def_camera = 1;
	if (1 < run_mode && run_mode < 5) 
	def_camera = 2;	// prevent player movement in entity viewer mode

	while (def_camera) 
	{
		aforce.tilt = 5*(key_pgup - key_pgdn + mouse_right*mouse_force.y);
		if (key_alt==0) {
			aforce.pan = -5*(key_force.x + mouse_right*mouse_force.x + joy_force.x);
			aforce.roll = 0;
			} else {
			aforce.pan = 0;
			aforce.roll = 5*(key_force.x + mouse_right*mouse_force.x + joy_force.x);
		}
		vec_add(&camera->pan,vec_accelerate(&dist,&aspeed,&aforce,0.8));

		force.x = 7*(key_force.y + key_w - key_s + joy_force.y);
		force.y = 3*(key_comma - key_period + key_a - key_d);
		force.z = 3*(key_home - key_end);
		vec_accelerate(&dist,&speed,&force,0.5);
		
		if (NULL != player && 1 == def_camera) {
			c_move(player,&dist,nullvector,IGNORE_PASSABLE|IGNORE_PASSENTS|GLIDE);
			camera->genius = player;
			vec_set(&player->pan,&camera->pan);
			vec_set(&camera->x,nullvector);
			vec_rotate(&camera->x,&camera->pan);
			vec_add(&camera->x,&player->x);
			} else {
			camera->genius = NULL;
			vec_add(&camera->x,vec_rotate(&dist,&camera->pan));
		}
		wait(1);
	}
}

void def_moveset() 
{
	def_camera += 1;
	
	if (NULL != player)
	    def_camera = cycle(def_camera,0,3);	// 0-1-2
	else
	    def_camera = cycle(def_camera,0,2);	// 0-1
	    
	if (!key_shift && def_camera > 0) def_move();
}

void def_debug() 
{
	if(key_alt) 
	{ 
		diag_status(); 
		return; 
	}
	if(key_shift) 
	{
		diag_status();
		freeze_mode = !freeze_mode;
		if (freeze_mode)
		{
			def_oldmouse = mouse_mode;
			mouse_mode = 4;
			mouse_pointer = 2;
		}
		else
			mouse_mode = def_oldmouse;
			
		while (freeze_mode)
		{
			if(mouse_left)
			watched = mouse_ent;
			wait(1);
		}
		return;
	}

	if (key_ctrl) {
		toggle(viewMap,SHOW);
		while is(viewMap,SHOW) {
			vec_set(&viewMap->x,&camera->x);
			viewMap->z += 10000;
			wait(1);
		}
		return;
	}

	if(d3d_lines) {
		reset(def_debug_pan,SHOW);
		d3d_lines = 0;
		diag_mode &= ~1;
		} else {
		if is(def_debug_pan,SHOW) {
			d3d_lines = 3;
			} else {
			set(def_debug_pan,SHOW);
			diag_mode |= 1;
			while is(def_debug_pan,SHOW) {
				def_dfps = 0.9*def_dfps+0.1/time_frame;
				def_dtlv = time_level+time_sky;
				def_dtcs = time_client+time_server;
				def_dtrf = time_update+time_idle;
				def_dtac = time_actions+time_pfuncs;
				def_cang.pan = cycle(camera->pan,0,360); 
				def_cang.tilt = cycle(camera->tilt,-180,180); 
				def_cang.roll = cycle(camera->roll,-180,180); 
				
				pan_setcolor(def_debug_pan, 1, 3, vector(0, 255, 0));
				
				wait(1);
			}
		}
	}
}

void def_exit() 
{
	if (!key_f4 || key_alt) sys_exit(NULL); 
}

void def_save() { game_save(app_name,1,SV_ALL - SV_INFO - SV_STRUCT); }
void def_load() { game_load(app_name,1); }

void def_startup() 
{ 
	if (!on_f2) on_f2 = def_save;
	if (!on_f3) on_f3 = def_load;
	if (!on_f4) on_f4 = def_exit;
	if (!on_f11) on_f11 = def_debug;

	if (!on_0) on_0 = def_moveset;
	if (!on_grave) on_grave = def_console;
	if (!on_esc) on_esc = def_exit;
}
