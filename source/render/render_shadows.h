/* render_shadows.h */
////////////////////////////////////////////////////////////////
// shadows.c - parallel-split shadow mapping
// (c) oP group 2010
////////////////////////////////////////////////////////////////
#ifndef    A8_FREE

#ifndef    __RENDER_SHADOWS_H__
#define    __RENDER_SHADOWS_H__

#ifndef    d3d9_h
#include   <d3d9.h>
#endif

#define PRAGMA_PRINT "[Shadow mapping] "

#define fx_shadow   "vp_pssm.fx"	// single-pass shadow shader
#define fx_depth    "vp_depth.fx"	// depth rendering shader

fixed  pssm_res = 1024;			// shadow map resolution
fixed  pssm_numsplits = 3;		// number of splits: 3 for small, 4 for large levels
fixed  pssm_splitweight = 0.75; 	// logarithmic / uniform ratio
fixed  pssm_splitdist[5];				// split distances
fixed  pssm_transparency = 0.6;	// shadow transparency
float  pssm_fbias = 2.5;		// shadow depth bias -> must be adjusted

float pssm_time = 60.0;

//The MIT License (MIT)
//
//Copyright (c) 2015 Nils Daumann
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
void   view_to_split_custom(VIEW *frustview, fixed nearplane, fixed farplane, VIEW *split, float *matSplit);
void   pssm_split(VIEW* view, fixed num, fixed weight);
float *pssm_texscale(float fSize);
void   pssm_viewcpy(VIEW* from, VIEW* to);
void   pssm_run(fixed numsplits);

#include "render_shadows.c"
#endif /* render_shadows.h */
#endif
