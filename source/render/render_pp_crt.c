/* render_pp_crt.c */
/**
 * Cathode ray tube (CRT) postprocessing shader v0.1 (2013-01-24)
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
 
/**
 * Creates and returns a new CRT postprocessing shader with the given mode. The material
 * must be applied to a staged postprocessing view.
 * 
 * mode
 *    CRT mode, see modes-section in the file "ppFxCrt.h"
 */
MATERIAL* ppCrtNewMtl (int mode)
{
	MATERIAL* m = ppMtlNew(PP_CRT_MTL_FX, NULL);
	m->PP_CRT_MTL_MODE = -1;

	ppCrtSetMode(m, mode);
	ppCrtReset(m);

	return m;
}

/**
 * Resets the CRT to the standard parameters
 */
void ppCrtReset (MATERIAL* m)
{
	if (m != NULL)
	{
		ppCrtSetAlpha(m, def_ppCrtMtlAlpha);
		ppCrtSetGamma(m, def_ppCrtMtlGamma);
		ppCrtSetIntensity(m, def_ppCrtMtlIntensity);
		ppCrtSetScale(m, def_ppCrtMtlScale);
	}
}

/**
 * Returns the CRT mode
 */
int ppCrtGetMode (MATERIAL* m)
{
	if (m == NULL)
		return PP_CRT_MODE_NONE;

	return (int)(m->PP_CRT_MTL_MODE);
}

/**
 * Changes the CRT mode
 * 
 * mode
 *    CRT mode, see modes-section in the file "ppFxCrt.h"
 */
void ppCrtSetMode (MATERIAL* m, int mode)
{
	if (m == NULL)
		return NULL;

	if (m->PP_CRT_MTL_MODE == mode)
		return m;

	char* strTechnique = _chr((txt_ppCrtTechniques->pstring)[clamp(mode, 0, PP_CRT_MODE_CNT)]);

	MATERIAL* m = ppMtlLoad(m, PP_CRT_MTL_FX, strTechnique);
	m->PP_CRT_MTL_MODE = mode;

	return m;
}

/**
 * Returns the alpha between 0...100
 */
var ppCrtGetAlpha (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_CRT_MTL_ALPHA) * 100;
}

/**
 * Sets the alpha
 * 
 * fAlpha
 *    Translucency of between 0...100
 */
void ppCrtSetAlpha (MATERIAL* m, var fAlpha)
{
	if (m != NULL)
		m->PP_CRT_MTL_ALPHA = floatv(fAlpha / 100);
}

/**
 * Returns the gamma
 */
var ppCrtGetGamma (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_CRT_MTL_GAMMA);
}

/**
 * Sets the gamma
 * 
 * fGamma
 *    Gamma
 */
void ppCrtSetGamma (MATERIAL* m, var fGamma)
{
	if (m != NULL)
		m->PP_CRT_MTL_GAMMA = floatv(fGamma);
}

/**
 * Returns the intensity
 */
var ppCrtGetIntensity (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_CRT_MTL_INTENSITY);
}

/**
 * Sets the intensity
 * 
 * fIntensity
 *    Intensity
 */
void ppCrtSetIntensity (MATERIAL* m, var fIntensity)
{
	if (m != NULL)
		m->PP_CRT_MTL_INTENSITY = floatv(fIntensity);
}

/**
 * Returns the scale
 */
var ppCrtGetScale (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_CRT_MTL_SCALE);
}

/**
 * Sets the scale
 * 
 * fScale
 *    Scale
 */
void ppCrtSetScale (MATERIAL* m, var fScale)
{
	if (m != NULL)
		m->PP_CRT_MTL_SCALE = floatv(fScale);
}
