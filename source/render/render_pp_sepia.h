/* render_pp_sepia.h */
/**
 * Sepia postprocessing shader v0.2 (2012-01-06)
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
#ifndef    A8_FREE

#ifndef    render_pp_sepia_h
#define    render_pp_sepia_h

#include "render_pp_mtl.h"

// modes
	#define PP_SEPIA_MODE_NONE      0

	#define PP_SEPIA_MODE_TINT      1     // adds to RGB a color offset
	#define PP_SEPIA_MODE_MATRIX    2     // multipies RGB with color matrix
	#define PP_SEPIA_MODE_YIQ       3     // does toning in YIQ colorspace
	#define PP_SEPIA_MODE_DUO       4     // interpolate between 2 colors by luma


	#define PP_SEPIA_MODE_CNT       5


// functions

	// creates and returns a new sepia postprocessing shader with the given mode
	MATERIAL* ppSepiaNewMtl (int mode);
	
	// resets the sepia to the standard parameters
	void ppSepiaReset (MATERIAL*);

	// global accessors

		// mode
		int ppSepiaGetMode (MATERIAL*);
		void ppSepiaSetMode (MATERIAL*, int mode);

		// alpha
		var ppSepiaGetAlpha (MATERIAL*);
		void ppSepiaSetAlpha (MATERIAL*, var);

		// exposure threshold
		void ppSepiaGetExpLumaThreshold (MATERIAL*, var* refThreshold, var* refWidth);
		void ppSepiaSetExpLumaThreshold (MATERIAL*, var fThreshold, var fWidth);

		// exposure dimming/brightening strength
		void ppSepiaGetExpStrength (MATERIAL*, var* refDimming, var* refBrightening);
		void ppSepiaSetExpStrength (MATERIAL*, var fDimming, var fBrightening);

	// mode accessors

		COLOR* ppSepiaGetTint (MATERIAL*);
		void ppSepiaSetTint (MATERIAL*, COLOR* cTint);

		void ppSepiaGetYiq (MATERIAL*, var* refI, var* refQ);
		void ppSepiaSetYiq (MATERIAL*, var fI, var fQ);

		void ppSepiaGetDuoColors (MATERIAL*, COLOR* cFrom, COLOR* cTo);
		void ppSepiaSetDuoColors (MATERIAL*, COLOR* cFrom, COLOR* cTo);

// private

	#define PP_SEPIA_MTL_MODE skill1
	#define PP_SEPIA_MTL_ALPHA skill2

	#define PP_SEPIA_MTL_EXP_LUMA_MIN skill3
	#define PP_SEPIA_MTL_EXP_LUMA_MAX skill4
	#define PP_SEPIA_MTL_EXP_DIMMING skill5
	#define PP_SEPIA_MTL_EXP_BRIGHTENING skill6

	#define PP_SEPIA_MTL_YIQ_I skill7
	#define PP_SEPIA_MTL_YIQ_Q skill8

	#define PP_SEPIA_MTL_TINT ambient_blue
	#define PP_SEPIA_MTL_DUO_COLOR1 diffuse_blue
	#define PP_SEPIA_MTL_DUO_COLOR2 specular_blue

	var def_ppSepiaMtlAlpha = 100;
	var def_ppSepiaMtlExpLumaThreshold = 0.5;
	var def_ppSepiaMtlExpLumaThresholdWidth = 0.2;
	var def_ppSepiaMtlExpDimming = 0.02;
	var def_ppSepiaMtlExpBrightening = 0.2;

	var def_ppSepiaMtlYiqI = 0.2;
	var def_ppSepiaMtlYiqQ = 0;

	COLOR* def_ppSepiaMtlTint = { red = 80;  green = 43;  blue = -23; }

	COLOR* def_ppSepiaMtlDuoFrom = { red = 51;  green = 13;  blue = 0; }
	COLOR* def_ppSepiaMtlDuoTo   = { red = 255; green = 230; blue = 128; }	

	#define PP_SEPIA_MTL_FX "ppSepia.fx"

	// names of techniques
	TEXT* txt_ppSepiaTechniques = {
		string ("tNone", "tTint", "tMatrix", "tYiq", "tDuo");
	}

#include "render_pp_sepia.c"
#endif /* render_pp_sepia.h */
#endif
