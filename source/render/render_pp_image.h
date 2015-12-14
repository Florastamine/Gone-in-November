/* render_pp_image.h */
/**
 * Uniform background image postprocessing shader v0.2 (2013-01-15)
 *
 * Copyright (C) 2012, 2013 by Christian Behrenberg
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
#ifndef    render_pp_image_h
#define    render_pp_image_h

   #include "render_pp_helper.h"
   #include "render_pp_mtl.h"
   #include "render_pp_image.h"

	// creates a new uniform background image postprocessing shader for a given bitmap
	MATERIAL* ppImageNewMtl (BMAP*);

	// accessors

		// changes the image of the material's postprocessing shader
		void ppImageSetMtlImg (MATERIAL* m, BMAP* bmapImg);

		// offset
		VECTOR* ppImageGetMtlImgOffset (MATERIAL* m, VECTOR* offset);
		void ppImageSetMtlImgOffset (MATERIAL* m, VECTOR* offset);

// private

	#define PP_IMAGE_MTL_IMG skin1

	#define PP_IMAGE_MTL_NORM_X skill1
	#define PP_IMAGE_MTL_NORM_Y skill2
	#define PP_IMAGE_MTL_SCALE_X skill3
	#define PP_IMAGE_MTL_SCALE_Y skill4

	#define PP_IMAGE_MTL_OFFSET_X skill5
	#define PP_IMAGE_MTL_OFFSET_Y skill6

	MATERIAL* ppImageGetMtlFx ();
	MATERIAL* g_ppImageMtlFx = NULL;

	VECTOR* def_ppImageMtlOffset = nullvector;

	void mtlPpImage_ev ();

	// calculates the normalization and scaling parameters of an uniformly scaled image
	void ppImageCalcScaling (BMAP*, double* refNormX, double* refNormY,
	                                double* refScaleX, double* refScaleY);
	                                
	#include "render_pp_image.c"
#endif /* render_pp_image.h */
