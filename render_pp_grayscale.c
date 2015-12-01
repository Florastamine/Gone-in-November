/* render_pp_grayscale.c */
/**
 * Grayscale postprocessing shader v0.3 (2012-01-06)
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
 * Creates and returns a new grayscale postprocessing shader with the given mode. The
 * material must be applied to a staged postprocessing view.
 * 
 * mode
 *    Grayscale mode, see modes-section in the file "ppFxGrayscale.h"
 */
MATERIAL* ppGrayscaleNewMtl (int mode)
{
	MATERIAL* m = ppMtlNew(PP_GRAYSCALE_MTL_FX, NULL);
	m->PP_GRAYSCALE_MTL_MODE = -1;

	ppGrayscaleSetMode(m, mode);
	ppGrayscaleReset(m);

	return m;
}

/**
 * Resets the grayscale to the standard parameters
 */
void ppGrayscaleReset (MATERIAL* m)
{
	if (m != NULL)
		ppGrayscaleSetAlpha(m, def_ppGrayscaleMtlAlpha);
}

/**
 * Returns the grayscale mode
 */
int ppGrayscaleGetMode (MATERIAL* m)
{
	if (m == NULL)
		return PP_GRAYSCALE_MODE_NONE;

	return (int)(m->PP_GRAYSCALE_MTL_MODE);
}

/**
 * Changes the grayscale mode
 * 
 * mode
 *    Grayscale mode, see modes-section in the file "ppFxGrayscale.h"
 */
void ppGrayscaleSetMode (MATERIAL* m, int mode)
{
	if (m == NULL)
		return NULL;

	if (m->PP_GRAYSCALE_MTL_MODE == mode)
		return m;

	char* strTechnique = _chr((txt_ppGrayscaleTechniques->pstring)[clamp(mode, 0, PP_GRAYSCALE_MODE_CNT)]);

	MATERIAL* m = ppMtlLoad(m, PP_GRAYSCALE_MTL_FX, strTechnique);
	m->PP_GRAYSCALE_MTL_MODE = mode;

	return m;
}

/**
 * Returns the alpha between 0...100
 */
var ppGrayscaleGetAlpha (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_GRAYSCALE_MTL_ALPHA) * 100;
}

/**
 * Sets the alpha
 * 
 * fAlpha
 *    Translucency of between 0...100
 */
void ppGrayscaleSetAlpha (MATERIAL* m, var fAlpha)
{
	if (m != NULL)
		m->PP_GRAYSCALE_MTL_ALPHA = floatv(fAlpha / 100);
}
