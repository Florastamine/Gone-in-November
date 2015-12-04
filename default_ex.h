/* default_ex.h */
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
#ifndef    __DEFAULT_EX_H__
#define    __DEFAULT_EX_H__

#ifdef     DEBUG

#define PRAGMA_PRINT "\n(Warning!) Debug enabled. Debugging module initialization..."

var def_dfps = 0, def_dtlv = 0, def_dtcs = 0, def_dtac = 0, def_dtrf = 0, def_oldmouse = 0, def_camera = 0;
ANGLE def_cang;

TEXT *def_ctxt = { string("Enter command: ","#80"); layer = 42; font = "Times#18b"; }

VIEW *viewMap = 
{
	flags = WIREFRAME | NOCULL; 
	layer = 1; 
	roll = 270; 
	tilt = -90; 
	arc = 10; 
}

PANEL *def_debug_pan = 
{
   pos_x = 5; 
	pos_y = 5; 
	layer = 42; 
	
	// Current screen resolution
	digits(0,2,"%4.0fx","Arial#15b",1,screen_size.x);
	digits(40,2,"%.0f","Arial#15b",1,screen_size.y);
	
	// Frame rate
   digits(0,17,"FPS %5.0f","Arial#15b",16,def_dfps);
	
	// Camera parameters
	digits(0,32,"camera->x%7.0f","Arial#15b",1,camera.x);
	digits(0,47,"camera->y%7.0f","Arial#15b",1,camera.y);
	digits(0,62,"camera->z%7.0f","Arial#15b",1,camera.z);
	digits(0,77,"camera->pan%5.0f","Arial#15b",1,def_cang.pan);
	digits(0,92,"camera->tilt%5.0f","Arial#15b",1,def_cang.tilt);
	digits(0,107,"camera->roll%5.0f","Arial#15b",1,def_cang.roll);
	digits(0,122,"FOV: %2.0f","Arial#15b",1,camera.arc);
 
	digits(120,2,"Time","Arial#15b",0,0);
	digits(120,17,"Scene + sky rendering: %5.1f ms/frame","Arial#15b",1,def_dtlv);
	digits(120,32,"Entity rendering: %5.1f ms/frame","Arial#15b",1,time_entities);
	digits(120,47,"Particles rendering: %5.1f ms/frame","Arial#15b",1,time_effects);
	digits(120,62,"2D drawing: %5.1f ms/frame","Arial#15b",1,time_draw);
	digits(120,77,"Panels/texts/views rendering: %5.1f ms/frame","Arial#15b",1,time_panels);
	digits(120,92,"Screen refresh + idle time: %5.1f ms/frame","Arial#15b",1,def_dtrf);
	digits(120,107,"Client + server time: %5.1f ms/frame","Arial#15b",1,def_dtcs);
	digits(120,122,"Functions: %5.1f ms/frame","Arial#15b",1,def_dtac);
	digits(120,139,"Physics: %5.1f ms/frame","Arial#15b",1,time_physics);
	
	digits(400,2,"Count","Arial#15b",0,0);
	digits(400,17,"Total scene entities%5.0f","Arial#15b",1,num_entities);
	digits(400,32,"Visible entities%5.0f","Arial#15b",1,num_visents);
	digits(400,47,"Visible triangles%5.0fk","Arial#15b",0.001,num_vistriangles);
	digits(400,62,"Number of particles%5.0f","Arial#15b",1,num_particles);
	digits(400,77,"Number of dynamic lights%5.0f","Arial#15b",1,num_lights);
	digits(400,92,"Number of running actions%5.0f","Arial#15b",1,num_actions);

	digits(600,2,"Memory usage (MB)","Arial#15b",0,0);
	digits(600,17,"Nexus %5.0f","Arial#15b",1,nexus);
	digits(600,32,"Engine allocated %5.0f","Arial#15b",1,sys_memory);
}

void def_debug();
void def_exit();
void def_save();
void def_load();
void def_exit();
void def_move();
void def_moveset();
void def_console();

#include "default_ex.c"
#endif

#endif /* default_ex.h */ 
