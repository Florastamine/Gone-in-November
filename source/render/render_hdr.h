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
 * 
 * __________________________________________________________________
 * 
 * <render_hdr>
 * High dynamic range.
 * Default values for HDR (blur, bloom strength, exposure time...) can 
 * be re-defined through render_hdr_setup(). Invoke render_hdr_new() and
 * render_hdr() to perform HDR rendering.
 * Queue is also available with render_hdr_set_queued().
 * 
 * Authors: Wolfgang "BoH_Havoc" Reichardt (http://dotmos.org/)
 *          Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    A8_FREE

#ifndef    _HDR_H_
#define    _HDR_H_

#define PRAGMA_PRINT "[High dynamic range] "

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
	
	BOOL queued;
} HDRState;

HDRState *HDRState_singleton = NULL;

HDRState *render_hdr_get_singleton();
void render_hdr_new();
void render_hdr_free();
void render_hdr_set_queued( __In BOOL state );
BOOL render_hdr_get_queued();

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

void render_hdr();

__static void __render_hdr_initialize();

MATERIAL *mtl_hdrDownsample   = { effect = "hdrDownsample.fx"; }
MATERIAL *mtl_hdrHighpass     = { effect = "hdrHighpass.fx"; }
MATERIAL *mtl_hdrBlur         = { effect = "hdrBlur.fx"; }
MATERIAL *mtl_hdrHBlur        = { effect = "hdrHBlur.fx"; }
MATERIAL *mtl_hdrVBlur        = { effect = "hdrVBlur.fx";	}
MATERIAL *mtl_hdr             = { effect = "hdr.fx"; }
MATERIAL *mtl_hdrGamma        = { effect = "hdrGamma.fx"; }
MATERIAL *mtl_hdrGamma2       = { effect = "hdrGamma2.fx"; }
MATERIAL *mtl_hdrGamma3       = { effect = "hdrGamma3.fx"; }

VIEW *view_hdrDownsample  = { material = mtl_hdrDownsample; flags = PROCESS_TARGET; }
VIEW *view_hdrHighpass    = { material = mtl_hdrHighpass; flags = PROCESS_TARGET; }
VIEW *view_hdrBlur        = { material = mtl_hdrBlur; flags = PROCESS_TARGET; }
VIEW *view_hdrHBlur       = { material = mtl_hdrHBlur; flags = PROCESS_TARGET; }
VIEW *view_hdrVBlur       = { material = mtl_hdrVBlur; flags = PROCESS_TARGET; }
VIEW *view_hdr            = { material = mtl_hdr; flags = PROCESS_TARGET; }
VIEW *view_hdrGamma       = { material = mtl_hdrGamma; flags = PROCESS_TARGET; layer = -10; bmap="#64x64x32"; }
VIEW *view_hdrGamma2      = { material = mtl_hdrGamma2; flags = PROCESS_TARGET;size_x = 16; size_y = 16; bmap="#16x16x32"; }
VIEW *view_hdrGamma3      = { material = mtl_hdrGamma2; flags = PROCESS_TARGET;size_x = 4; size_y = 4; bmap="#4x4x32"; }
VIEW *view_hdrGamma4      = { material = mtl_hdrGamma3; flags = PROCESS_TARGET;size_x = 1; size_y = 1; }

#include "render_hdr.c"
#endif /* render_hdr.h */
#endif
