/* render_pp_crt.h */
/**
 * Cathode ray tube (CRT) postprocessing shader v0.1 (2013-01-24)
 *
 * Copyright (C) 2013 by Christian Behrenberg
 * http://www.christian-behrenberg.com
 *
 * Modified and packaged by Florastamine (florastamine@gmail.com)
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
#ifndef    render_pp_crt_h
#define    render_pp_crt_h

#include "render_pp_helper.h"
#include "render_pp_mtl.h"

// modes

	#define PP_CRT_MODE_NONE       0

	#define PP_CRT_MODE_SCANLINE   1     // alternated colored- and black lines
	#define PP_CRT_MODE_DOTNBLOOM  2     // brightness based dots +bloom
	#define PP_CRT_MODE_RGB_BARS   3     // RGB bars
	#define PP_CRT_MODE_MG_BARS    4     // Magenta/Green bars
	#define PP_CRT_MODE_RGB_TRIAD  5     // RGB triad

	#define PP_CRT_MODE_CNT        6

// functions

	// creates and returns a new CRT postprocessing shader with the given mode
	MATERIAL* ppCrtNewMtl (int mode);

	// resets the crt to the standard parameters
	void ppCrtReset (MATERIAL*);

	// accessors

		// mode
		int ppCrtGetMode (MATERIAL*);
		void ppCrtSetMode (MATERIAL*, int mode);

		// alpha
		var ppCrtGetAlpha (MATERIAL*);
		void ppCrtSetAlpha (MATERIAL*, var);

		// gamma
		var ppCrtGetGamma (MATERIAL*);
		void ppCrtSetGamma (MATERIAL*, var);
		
		// intensity
		var ppCrtGetIntensity (MATERIAL*);
		void ppCrtSetIntensity (MATERIAL*, var);

		// scale
		var ppCrtGetScale (MATERIAL*);
		void ppCrtSetScale (MATERIAL*, var);

// private

	#define PP_CRT_MTL_MODE skill1
	#define PP_CRT_MTL_ALPHA skill2

	#define PP_CRT_MTL_GAMMA skill6
	#define PP_CRT_MTL_INTENSITY skill7
	#define PP_CRT_MTL_SCALE skill8

	var def_ppCrtMtlAlpha = 100;
	var def_ppCrtMtlGamma = 0;
	var def_ppCrtMtlIntensity = 0.5;
	var def_ppCrtMtlScale = 1;

	#define PP_CRT_MTL_FX "ppCrt.fx"

	// names of techniques
	TEXT* txt_ppCrtTechniques = {
		string ( "tNone", "tScanline", "tDotNBloom", "tRgbMask", "tMagentaGreenMask", "tRgbtriad");
	}

#include "render_pp_crt.c"
#endif /* render_pp_crt.h */
