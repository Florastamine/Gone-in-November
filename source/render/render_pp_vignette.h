/* render_pp_vignette.h */
/**
 * Vignette postprocessing shader v0.1 (2012-12-15)
 *
 * Copyright (C) 2012 by Christian Behrenberg
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
#ifndef    render_pp_vignette_h
#define    render_pp_vignette_h

#include "render_pp_mtl.h"

// functions

	// creates a new default vignette postprocessing shader
	MATERIAL* ppVignetteNewMtl ();
	
	// resets the vignette to the standard parameters
	void ppVignetteReset (MATERIAL*);

	// accessors

		// center of the vignette in relative screen coordinates
		VECTOR* ppVignetteGetCenter (MATERIAL*);
		void ppVignetteSetCenter (MATERIAL*, VECTOR*);

		// color of the vignette in BGR-format between 0...255
		COLOR* ppVignetteGetColor (MATERIAL*);
		void ppVignetteSetColor (MATERIAL*, COLOR*);

		// start distance
		var ppVignetteGetStart (MATERIAL*);
		void ppVignetteSetStart (MATERIAL*, var);

		// end distance
		var ppVignetteGetEnd (MATERIAL*);
		void ppVignetteSetEnd (MATERIAL*, var);

		// alpha
		var ppVignetteGetAlpha (MATERIAL*);
		void ppVignetteSetAlpha (MATERIAL*, var);


// defaults

	VECTOR* def_ppVignetteMtlCenter = nullvector; // center
	COLOR* def_ppVignetteMtlColor = nullvector; // black
	var def_ppVignetteMtlStart = 0.3;
	var def_ppVignetteMtlEnd = 0.75;
	var def_ppVignetteMtlAlpha = 100;


// fields

	#define PP_VIGNETTE_MTL_CENTER_X skill1
	#define PP_VIGNETTE_MTL_CENTER_Y skill2

	#define PP_VIGNETTE_MTL_START skill3
	#define PP_VIGNETTE_MTL_END skill4

	#define PP_VIGNETTE_MTL_ALPHA skill5

	#define PP_VIGNETTE_MTL_COLOR ambient_blue	

// private

	MATERIAL* ppVignetteGetMtlFx ();

	#define PP_VIGNETTE_MTL_FX "ppVignette.fx"
	MATERIAL* g_ppVignetteMtlFx = NULL;
	
#include "render_pp_vignette.c"
#endif /* render_pp_vignette.h */
