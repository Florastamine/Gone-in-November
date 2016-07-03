/* render_pp_grayscale.h */
/**
 * Grayscale postprocessing shader v0.3 (2012-01-06)
 *
 * Copyright (C) 2012, 2013 by Christian Behrenberg
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
#ifndef    A8_FREE

#ifndef    render_pp_grayscale_h
#define    render_pp_grayscale_h

#include "render_pp_mtl.h"
#include "render_pp_helper.h"

// modes

	#define PP_GRAYSCALE_MODE_NONE   0

	#define PP_GRAYSCALE_MODE_LUMA   1     // RGB dot product
	#define PP_GRAYSCALE_MODE_DESAT  2     // desaturation via HSV colorspace
	#define PP_GRAYSCALE_MODE_AVG    3     // average: (r+g+b)/3
	#define PP_GRAYSCALE_MODE_MIN    4     // lowest channel
	#define PP_GRAYSCALE_MODE_MAX    5     // highest channel
	#define PP_GRAYSCALE_MODE_R      6     // red channel
	#define PP_GRAYSCALE_MODE_G      7     // green channel
	#define PP_GRAYSCALE_MODE_B      8     // blue channel


	#define PP_GRAYSCALE_MODE_CNT    9


// functions

	// creates and returns a new grayscale postprocessing shader with the given mode
	MATERIAL* ppGrayscaleNewMtl (int mode);
	
	// resets the grayscale to the standard parameters
	void ppGrayscaleReset (MATERIAL*);

	// accessors

		// mode
		int ppGrayscaleGetMode (MATERIAL*);
		void ppGrayscaleSetMode (MATERIAL*, int mode);

		// alpha - if <0 colors are boosted, if >100 colors are inverted
		var ppGrayscaleGetAlpha (MATERIAL*);
		void ppGrayscaleSetAlpha (MATERIAL*, var);	

// private

	#define PP_GRAYSCALE_MTL_MODE skill1
	#define PP_GRAYSCALE_MTL_ALPHA skill2

	var def_ppGrayscaleMtlAlpha = 100;	

	#define PP_GRAYSCALE_MTL_FX "ppGrayscale.fx"

	char* ppGrayscaleTechniqueName (int mode);

	// names of techniques
	TEXT* txt_ppGrayscaleTechniques = {
		string ("tNone", "tLuma", "tDesat", "tAvg", "tMin", "tMax", "tR", "tG", "tB");
	}

#include "render_pp_grayscale.c"
#endif /* render_pp_grayscale.h */
#endif
