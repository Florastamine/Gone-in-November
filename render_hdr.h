/* render_hdr.h */
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
#ifndef    _HDR_H_
#define    _HDR_H_

#define __In 
#define __Out 
#define __static 

#define    __HDR

typedef struct {
	float rt_factor;
	float bit_depth;
	
	float light_scattering;
	float blur;
	float bloom_strength;
	float highpass_luminance;
	float highpass_middle_grey;
	float highpass_white_cutoff;
	float adaption_speed;
	
	BOOL active;
} HDRState;

HDRState *HDRState_singleton = NULL;

void render_hdr_new();
void render_hdr_free();
BOOL render_hdr_is_active();
HDRState *HDRState_get_singleton();

void render_hdr_setup(
                       __In float RT,
                       __In float bit_depth,
                       __In float light_scattering,
                       __In float blur,
                       __In float bloom_strength,
                       __In float highpass_luminance,
                       __In float highpass_middle_grey,
                       __In float highpass_white_cutoff,
                       __In float adaption_speed );

void render_hdr_initialize();
void render_hdr();

MATERIAL *sc_mtl_hdrDownsample   = { effect = "sc_hdrDownsample.fx"; }
MATERIAL *sc_mtl_hdrHighpass     = { effect = "sc_hdrHighpass.fx"; }
MATERIAL *sc_mtl_hdrBlur         = { effect = "sc_hdrBlur.fx"; }
MATERIAL *sc_mtl_hdrHBlur        = { effect = "sc_hdrHBlur.fx"; }
MATERIAL *sc_mtl_hdrVBlur        = { effect = "sc_hdrVBlur.fx";	}
MATERIAL *sc_mtl_hdr             = { effect = "sc_hdr.fx"; }
MATERIAL *sc_mtl_hdrGamma        = { effect = "sc_hdrGamma.fx"; }
MATERIAL *sc_mtl_hdrGamma2       = { effect = "sc_hdrGamma2.fx"; }
MATERIAL *sc_mtl_hdrGamma3       = { effect = "sc_hdrGamma3.fx"; }

VIEW *sc_view_hdrDownsample  = { material = sc_mtl_hdrDownsample; flags = PROCESS_TARGET; }
VIEW *sc_view_hdrHighpass    = { material = sc_mtl_hdrHighpass; flags = PROCESS_TARGET; }
VIEW *sc_view_hdrBlur        = { material = sc_mtl_hdrBlur; flags = PROCESS_TARGET; }
VIEW *sc_view_hdrHBlur       = { material = sc_mtl_hdrHBlur; flags = PROCESS_TARGET; }
VIEW *sc_view_hdrVBlur       = { material = sc_mtl_hdrVBlur; flags = PROCESS_TARGET; }
VIEW *sc_view_hdr            = { material = sc_mtl_hdr; flags = PROCESS_TARGET; }
VIEW *sc_view_hdrGamma       = { material = sc_mtl_hdrGamma; flags = PROCESS_TARGET; layer = -10; bmap="#64x64x32"; }
VIEW *sc_view_hdrGamma2      = { material = sc_mtl_hdrGamma2; flags = PROCESS_TARGET;size_x = 16; size_y = 16; bmap="#16x16x32"; }
VIEW *sc_view_hdrGamma3      = { material = sc_mtl_hdrGamma2; flags = PROCESS_TARGET;size_x = 4; size_y = 4; bmap="#4x4x32"; }
VIEW *sc_view_hdrGamma4      = { material = sc_mtl_hdrGamma3; flags = PROCESS_TARGET;size_x = 1; size_y = 1; }

#include "render_hdr.c"
#endif /* render_hdr.h */
