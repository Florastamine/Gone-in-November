/* render_pp_sepia.c */
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
 
/**
 * Creates and returns a new sepia postprocessing shader with the given mode. The
 * material must be applied to a staged postprocessing view.
 * 
 * mode
 *    Sepia mode, see modes-section in the file "ppFxSepia.h"
 */
MATERIAL* ppSepiaNewMtl (int mode)
{
	MATERIAL* m = ppMtlNew(PP_SEPIA_MTL_FX, NULL);

	m->PP_SEPIA_MTL_MODE = -1;
	ppSepiaSetMode(m, mode);

	ppSepiaReset(m);

	return m;
}

/**
 * Resets the sepia to the standard parameters
 */
void ppSepiaReset (MATERIAL* m)
{
	if (m != NULL)
	{
		ppSepiaSetAlpha(m, def_ppSepiaMtlAlpha);
		ppSepiaSetExpLumaThreshold(m, def_ppSepiaMtlExpLumaThreshold, def_ppSepiaMtlExpLumaThresholdWidth);
		ppSepiaSetExpStrength(m, def_ppSepiaMtlExpDimming, def_ppSepiaMtlExpBrightening);

		ppSepiaSetTint(m, def_ppSepiaMtlTint);
		ppSepiaSetYiq(m, def_ppSepiaMtlYiqI, def_ppSepiaMtlYiqQ);
		ppSepiaSetDuoColors(m, def_ppSepiaMtlDuoFrom, def_ppSepiaMtlDuoTo);
	}
}

/**
 * Returns the sepia mode
 */
int ppSepiaGetMode (MATERIAL* m)
{
	if (m == NULL)
		return PP_SEPIA_MODE_NONE;

	return (int)(m->PP_SEPIA_MTL_MODE);
}

/**
 * Changes the sepia mode
 * 
 * mode
 *    Sepia mode, see modes-section in the file "ppFxSepia.h"
 */
void ppSepiaSetMode (MATERIAL* m, int mode)
{
	if (m == NULL)
		return NULL;

	if (m->PP_SEPIA_MTL_MODE == mode)
		return m;

	char* strTechnique = _chr((txt_ppSepiaTechniques->pstring)[clamp(mode, 0, PP_SEPIA_MODE_CNT)]);

	MATERIAL* m = ppMtlLoad(m, PP_SEPIA_MTL_FX, strTechnique);
	m->PP_SEPIA_MTL_MODE = mode;

	return m;
}

/**
 * Returns the alpha between 0...100
 */
var ppSepiaGetAlpha (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_SEPIA_MTL_ALPHA) * 100;
}

/**
 * Sets the alpha
 * 
 * fAlpha
 *    Translucency of between 0...100
 */
void ppSepiaSetAlpha (MATERIAL* m, var fAlpha)
{
	if (m != NULL)
		m->PP_SEPIA_MTL_ALPHA = floatv(fAlpha / 100);
}

/**
 * Fetches the target threshold and the interpolation width of the decision, which pixel
 * is a highlight and which pixel is a dark one
 * 
 * refThreshold
 *    Pointer to a var, receiving the threshold
 *
 * refWidth
 *    Pointer to a var, receiving the width
 */
var ppSepiaGetExpLumaThreshold (MATERIAL* m, var* refThreshold, var* refWidth)
{
	if (m == NULL || (refThreshold == NULL && refWidth == NULL))
		return;

	if (refThreshold != NULL)
		*refThreshold = fixv(m->PP_SEPIA_MTL_EXP_LUMA_MIN) +
		                (fixv(m->PP_SEPIA_MTL_EXP_LUMA_MAX) - fixv(m->PP_SEPIA_MTL_EXP_LUMA_MIN)) / 2;

	if (refWidth != NULL)
		*refWidth = fixv(m->PP_SEPIA_MTL_EXP_LUMA_MAX) - fixv(m->PP_SEPIA_MTL_EXP_LUMA_MIN);
}

/**
 * Sets the threshold and its interpolation width for deciding, which pixel is a highlight
 * and which pixel is a dark one
 * 
 * fThreshold
 *    Value between 0...1, usually 0.5
 *
 * fWidth
 *    The broader, the more are colors near the threshold mixed because they are taken
 *    as both highlight and shadow; usually near zero, like 0.1 or 0.2
 */
void ppSepiaSetExpLumaThreshold (MATERIAL* m, var fThreshold, var fWidth)
{
	if (m == NULL || (fThreshold < 0 && fWidth < 0))
		return;

	var t = fThreshold;
	var w = fWidth;

	if (w < 0)
		w = fixv(m->PP_SEPIA_MTL_EXP_LUMA_MAX) - fixv(m->PP_SEPIA_MTL_EXP_LUMA_MIN);

	w /= 2;

	if (t < 0)
		t = fixv(m->PP_SEPIA_MTL_EXP_LUMA_MIN) +
		    (fixv(m->PP_SEPIA_MTL_EXP_LUMA_MAX) - fixv(m->PP_SEPIA_MTL_EXP_LUMA_MIN)) / 2;

	m->PP_SEPIA_MTL_EXP_LUMA_MIN = floatv(t - w);
	m->PP_SEPIA_MTL_EXP_LUMA_MAX = floatv(t + w);
}

/**
 * Fetches the target dimming and brightening factors of the exposure control
 * 
 * refDimming
 *    Pointer to a var, receiving the factor for dimming highlights
 *
 * refBrightening
 *    Pointer to a var, receiving the factor for brightening dark areas
 */
void ppSepiaGetExpStrength (MATERIAL* m, var* refDimming, var* refBrightening)
{
	if (m == NULL || (refDimming == NULL && refBrightening == NULL))
		return;

	if (refDimming != NULL)
		*refDimming = fixv(m->PP_SEPIA_MTL_EXP_DIMMING);

	if (refBrightening != NULL)
		*refBrightening = fixv(m->PP_SEPIA_MTL_EXP_BRIGHTENING);		
}

/**
 * Sets the dimming and brightening factors for exposure control
 * 
 * fDimming
 *    Factor for dimming highlights, should be near zero, like 0.1 ... 0.05
 *
 * fBrightening
 *    Factor for brightening dark areas, like 0.25
 */
void ppSepiaSetExpStrength (MATERIAL* m, var fDimming, var fBrightening)
{
	if (m == NULL || (fDimming < -1 && fBrightening < -1))
		return;

	if (fDimming > -1)
		m->PP_SEPIA_MTL_EXP_DIMMING = floatv(fDimming);

	if (fBrightening > -1)
		m->PP_SEPIA_MTL_EXP_BRIGHTENING = floatv(fBrightening);		
}

/**
 * Returns the tint color
 */
COLOR* ppSepiaGetTint (MATERIAL* m)
{
	if (m == NULL)
		return NULL;

	return vec_set(vector(0,0,0), m->PP_SEPIA_MTL_TINT);
}

/**
 * Sets the tint
 * 
 * cTint
 *    Tint added to the pixel's color. You can use positive and negative values.
 */
void ppSepiaSetTint (MATERIAL* m, COLOR* cTint)
{
	if (m != NULL && cTint != NULL)
		vec_set(m->PP_SEPIA_MTL_TINT, cTint);
}

/**
 * Fetches the target in-phase (I) and quadrature (Q) values of a pixel's color in
 * YIQ-color space
 * 
 * refI
 *    Pointer to a var, receiving the orange-blue range
 *
 * refQ
 *    Pointer to a var, receiving the purple-green range
 */
void ppSepiaGetYiq (MATERIAL* m, var* refI, var* refQ)
{
	if (m == NULL || (refI == NULL && refQ == NULL))
		return;

	if (refI != NULL)
		*refI = fixv(m->PP_SEPIA_MTL_YIQ_I);

	if (refQ != NULL)
		*refQ = fixv(m->PP_SEPIA_MTL_YIQ_Q);	
}

/**
 * Sets the in-phase (I) and quadrature (Q) values of a pixel's color in YIQ-color space
 * 
 * fI
 *    In-phase represents orange-blue range, for sepia use ~0.2 (I between +/-0.5957)
 *
 * fQ
 *    Quadrature represents purple-green range, for sepia use 0 (Q between +/-0.5226)
 */
void ppSepiaSetYiq (MATERIAL* m, var fI, var fQ)
{
	if (m == NULL || (fI < -1 && fQ < -1))
		return;

	if (fI > -1)
		m->PP_SEPIA_MTL_YIQ_I = floatv(fI);

	if (fQ > -1)
		m->PP_SEPIA_MTL_YIQ_Q = floatv(fQ);	
}

/**
 * Returns the from- and to-color of the duotone mode
 * 
 * cFrom
 *    Receives the color of low lumaninance colors
 *
 * cTo
 *    Receives the color of high lumaninance colors
 */
void ppSepiaGetDuoColors (MATERIAL* m, COLOR* cFrom, COLOR* cTo)
{
	if (m == NULL || (cFrom == NULL && cTo == NULL))
		return;

	if (cFrom != NULL)
		vec_set(cFrom, m->PP_SEPIA_MTL_DUO_COLOR1);

	if (cTo != NULL)
		vec_set(cTo, m->PP_SEPIA_MTL_DUO_COLOR2);
}

/**
 * Sets the from- and to-color of the duotone mode
 * 
 * cFrom
 *    Color of low lumaninance colors
 *
 * cTo
 *    Color of high lumaninance colors
 */
void ppSepiaSetDuoColors (MATERIAL* m, COLOR* cFrom, COLOR* cTo)
{
	if (m == NULL || (cFrom == NULL && cTo == NULL))
		return;

	if (cFrom != NULL)
		vec_set(m->PP_SEPIA_MTL_DUO_COLOR1, cFrom);

	if (cTo != NULL)
		vec_set(m->PP_SEPIA_MTL_DUO_COLOR2, cTo);
}
