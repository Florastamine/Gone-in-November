/* render_pp_grain.h */
/**
 * Film grain postprocessing shader v0.1 (2013-01-06)
 *
 * Copyright (C) 2013 by Christian Behrenberg
 * http://www.christian-behrenberg.com
 *
 * Modified and packaged by Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to per-
 * mit persons to whom the Software is furnished to do so, subject to the following con-
 * ditions:
 *  
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, IN-
 * CLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LI-
 * ABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef    render_pp_grain_h
#define    render_pp_grain_h

#include "render_pp_mtl.h"
#include "render_pp_image.h"
#include "render_pp_helper.h"

// modes

	#define PP_GRAIN_MODE_NONE      0

	// resolution dependent
	#define PP_GRAIN_MODE_RANDOM    1     // random grain per pixel
	#define PP_GRAIN_MODE_TILE      2     // tiled RGB-grain image
	#define PP_GRAIN_MODE_SEQUENCE  3     // tiled grain sequence (channelwise RGBA)

	// resolution independent
	#define PP_GRAIN_MODE_FULL      4     // uniformly stretched RGB-grain image
	#define PP_GRAIN_MODE_STREAM    5     // uniformly stretched RGB-video stream


	#define PP_GRAIN_MODE_CNT       6


// functions

	// creates and returns a new grain postprocessing shader with the given mode and a
	// filename for the loaded grain texture or videostream

	MATERIAL* ppGrainNewMtl (int mode, char* filename, VECTOR* videoSize);
	
	// resets the grain to the standard parameters
	void ppGrainReset (MATERIAL*);	

	// accessors

		// mode
		int ppGrainGetMode (MATERIAL*);
		void ppGrainSetMode (MATERIAL*, int mode, char* filename, VECTOR* videoSize);

		void ppGrainUpdateMode (MATERIAL*);

		// alpha
		var ppGrainGetAlpha (MATERIAL*);
		void ppGrainSetAlpha (MATERIAL*, var);

		// scale
		var ppGrainGetScale (MATERIAL*);
		void ppGrainSetScale (MATERIAL*, var);

		// speed
		var ppGrainGetSpeed (MATERIAL*);
		void ppGrainSetSpeed (MATERIAL*, var);

		// brightness
		var ppGrainGetBrightness (MATERIAL*);
		void ppGrainSetBrightness (MATERIAL*, var);

// private

	#define PP_GRAIN_MTL_MODE skill1
	#define PP_GRAIN_MTL_ALPHA skill2
	#define PP_GRAIN_MTL_STREAM skill3
	#define PP_GRAIN_MTL_SCALE skill4

	#define PP_GRAIN_MTL_NORM_X skill5
	#define PP_GRAIN_MTL_NORM_Y skill6
	#define PP_GRAIN_MTL_SCALE_X skill7
	#define PP_GRAIN_MTL_SCALE_Y skill8

	#define PP_GRAIN_MTL_RAND_X skill9
	#define PP_GRAIN_MTL_RAND_Y skill10
	#define PP_GRAIN_MTL_SIZE_X skill11
	#define PP_GRAIN_MTL_SIZE_Y skill12	

	#define PP_GRAIN_MTL_SPEED skill13
	#define PP_GRAIN_MTL_TIMER skill14
	#define PP_GRAIN_MTL_BRIGHTNESS skill15
	#define PP_GRAIN_MTL_SEQ_INDEX skill16

	#define PP_GRAIN_MTL_SEQ_MASK 16 // skill 17, 18, 19, 20

	#define PP_GRAIN_MTL_BMAP skin1

	var def_ppGrainMtlAlpha = 100;
	var def_ppGrainMtlScale = 1;
	var def_ppGrainMtlSpeed = 1.0;
	var def_ppGrainMtlBrightness = 0;

	#define PP_GRAIN_MTL_FX "ppGrain.fx"

	// names of techniques
	TEXT* txt_ppGrainTechniques = {
		string ("tNone", "tRandom", "tTile", "tSeq", "tFull", "tStream");
	}

#include "render_pp_grain.c"
#endif /* render_pp_grain.h */
