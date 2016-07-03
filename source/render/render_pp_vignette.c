/* render_pp_vignette.c */
/**
 * Vignette postprocessing shader v0.1 (2012-12-15)
 *
 * Copyright (C) 2012 by Christian Behrenberg
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

/**
 * Creates a new default vignette postprocessing shader. Returns the material containing
 * the shader with default values, these can be changed through the given accessor
 * functions. The material must be applied to a staged postprocessing view.
 */
MATERIAL* ppVignetteNewMtl ()
{
	MATERIAL* m = ppMtlNewInstance(ppVignetteGetMtlFx());

	ppVignetteSetCenter(m, def_ppVignetteMtlCenter);
	ppVignetteSetColor(m, def_ppVignetteMtlColor);
	ppVignetteSetStart(m, def_ppVignetteMtlStart);
	ppVignetteSetEnd(m, def_ppVignetteMtlEnd);
	ppVignetteSetAlpha(m, def_ppVignetteMtlAlpha);

	return m;
}

/**
 * Resets the vignette to the standard parameters
 */
void ppVignetteReset (MATERIAL* m)
{
	if (m != NULL)
	{
		ppVignetteSetCenter(m, def_ppVignetteMtlCenter);
		ppVignetteSetColor(m, def_ppVignetteMtlColor);
		ppVignetteSetStart(m, def_ppVignetteMtlStart);
		ppVignetteSetEnd(m, def_ppVignetteMtlEnd);
		ppVignetteSetAlpha(m, def_ppVignetteMtlAlpha);	
	}
}

/**
 * Sets the center of the vignette. x = y = 0 is the center of the screen and +/- 0.5 are
 * the corresponding borders, whereas xy=(-0.5, -0.5) is the left upper corner and vice
 * versa.
 * 
 * v
 *    The relative screen coordinates of the vignette center; .z will be ignored.
 */
void ppVignetteSetCenter (MATERIAL* m, VECTOR* v)
{
	if (m != NULL && v != NULL)
	{
		m->PP_VIGNETTE_MTL_CENTER_X = floatv(v->x + 0.5);
		m->PP_VIGNETTE_MTL_CENTER_Y = floatv(-v->y + 0.5);
	}
}


/**
 * Returns the center of the vignette in relative screen coordinates
 */
VECTOR* ppVignetteGetCenter (MATERIAL* m)
{
	if (m == NULL)
		return nullvector;

	var cx = fixv(m->PP_VIGNETTE_MTL_CENTER_X) - 0.5;
	var cy = -(fixv(m->PP_VIGNETTE_MTL_CENTER_Y) - 0.5);

	return vector(cx, cy, 0);
}


/**
 * Sets the color of the vignette
 * 
 * c
 *    Color vector in BGR-format with values between 0...255
 */
void ppVignetteSetColor (MATERIAL* m, COLOR* c)
{
	if (m != NULL && c != NULL)
		vec_set(m->PP_VIGNETTE_MTL_COLOR, c);
}


/**
 * Returns the color of the vignette in BGR-format between 0...255
 */
COLOR* ppVignetteGetColor (MATERIAL* m)
{
	if (m == NULL)
		return NULL;

	return vec_set(vector(0,0,0), m->PP_VIGNETTE_MTL_COLOR);
}


/**
 * Sets the start distance of the vignette
 * 
 * fStart
 *    Start distance of the vignette as percentage, e.g. 0.3 f�E 30%. Should be inside the
 *    screen (0 < fStart < 1).
 */
void ppVignetteSetStart (MATERIAL* m, var fStart)
{
	if (m != NULL)
	{
		m->PP_VIGNETTE_MTL_START = floatv(fStart);

		if (m->PP_VIGNETTE_MTL_START > m->PP_VIGNETTE_MTL_END)
			m->PP_VIGNETTE_MTL_START = m->PP_VIGNETTE_MTL_END;	
	}
}


/**
 * Returns the start distance of the vignette
 */
var ppVignetteGetStart (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_VIGNETTE_MTL_START);
}


/**
 * Sets the end distance of the vignette
 * 
 * fEnd
 *    End distance of the vignette as percentage, e.g. 0.75 f�E 75%. Should be near the
 *    border of the screen or slightly outside, e.g. 0.75 < fEnd < 1.5
 */
void ppVignetteSetEnd (MATERIAL* m, var fEnd)
{
	if (m != NULL)
	{
		m->PP_VIGNETTE_MTL_END = floatv(fEnd);

		if (m->PP_VIGNETTE_MTL_END < m->PP_VIGNETTE_MTL_START)
			m->PP_VIGNETTE_MTL_START = m->PP_VIGNETTE_MTL_END;		
	}
}


/**
 * Returns the end distance of the vignette
 */
var ppVignetteGetEnd (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_VIGNETTE_MTL_END);
}


/**
 * Sets the alpha of the vignette. Different translucency settings are normally achieved
 * with broader/narrow start/end-values, but you can achieve this as well with the
 * provided alpha-parameter to be more flexible.
 * 
 * fAlpha
 *    Translucency of the vignette between 0...100
 */
void ppVignetteSetAlpha (MATERIAL* m, var fAlpha)
{
	if (m != NULL)
		m->PP_VIGNETTE_MTL_ALPHA = floatv(fAlpha / 100);
}


/**
 * Returns the alpha of the vignette between 0...100
 */
var ppVignetteGetAlpha (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_VIGNETTE_MTL_ALPHA) * 100;
}


/**
 * Returns a singleton-material containing the shader code
 */
MATERIAL* ppVignetteGetMtlFx ()
{
	ppMtlAssert(&g_ppVignetteMtlFx, PP_VIGNETTE_MTL_FX, 0, NULL);
}
