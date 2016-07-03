/* render_pp_grain.c */
/**
 * Film grain postprocessing shader v0.1 (2013-01-06)
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

/**
 * Creates and returns a new film grain postprocessing shader with the given mode. The
 * material must be applied to a staged postprocessing view.
 * 
 * mode
 *    Film grain mode, see modes-section in the file "ppFxGrain.h"
 */
MATERIAL* ppGrainNewMtl (int mode, char* filename, VECTOR* videoSize)
{
	MATERIAL* m = ppMtlNew(PP_GRAIN_MTL_FX, NULL);
	m->PP_GRAIN_MTL_MODE = -1;

	ppGrainSetMode(m, mode, filename, videoSize);
	ppGrainReset(m);

	ppMtlEvent(m, ppGrainUpdateMode);

	return m;
}

/**
 * Resets the grain to the standard parameters
 */
void ppGrainReset (MATERIAL* m)
{
	if (m != NULL)
	{
		ppGrainSetAlpha(m, def_ppGrainMtlAlpha);
		ppGrainSetScale(m, def_ppGrainMtlScale);
		ppGrainSetSpeed(m, def_ppGrainMtlSpeed);
		ppGrainSetBrightness(m, def_ppGrainMtlBrightness);
	}
}

/**
 * Returns the film grain mode
 */
int ppGrainGetMode (MATERIAL* m)
{
	if (m == NULL)
		return PP_GRAIN_MODE_NONE;

	return (int)(m->PP_GRAIN_MTL_MODE);
}

/**
 * Changes the film grain mode
 * 
 * mode
 *    Film grain mode, see modes-section in the file "ppFxGrain.h"
 *
 * filename
 *    Filename of the to be used bitmap or media stream (mode dependent)
 */
void ppGrainSetMode (MATERIAL* m, int mode, char* filename, VECTOR* videoSize)
{
	if (m == NULL)
		return NULL;

	if (m->PP_GRAIN_MTL_MODE == mode)
		return m;

	char* strTechnique = _chr((txt_ppGrainTechniques->pstring)[clamp(mode, 0, PP_GRAIN_MODE_CNT)]);

	MATERIAL* m = ppMtlLoad(m, PP_GRAIN_MTL_FX, strTechnique);
	m->PP_GRAIN_MTL_MODE = mode;

	if (m->PP_GRAIN_MTL_STREAM != 0)
		media_stop(m->PP_GRAIN_MTL_STREAM);

	BMAP* b = (BMAP*)m->PP_GRAIN_MTL_BMAP;
	if (b != NULL)
	{
		ptr_remove(b);
		m->PP_GRAIN_MTL_BMAP = NULL;
	}

	if (mode > PP_GRAIN_MODE_NONE)
	{
		if (filename != NULL && str_len(filename) > 1)
		{
			if (mode != PP_GRAIN_MODE_STREAM)
				m->PP_GRAIN_MTL_BMAP = bmap_create(filename);
			else
			{
				int w = ppTrinary(videoSize != NULL, videoSize.x, screen_size.x);
				int h = ppTrinary(videoSize != NULL, videoSize.y, screen_size.y);

				m->PP_GRAIN_MTL_BMAP = bmap_createblack(w, h, 24);

				var h = media_loop(filename, m->PP_GRAIN_MTL_BMAP, 0);
				m->PP_GRAIN_MTL_STREAM = h;
			}
		}
	}

	ppGrainUpdateMode(m);

	return m;
}

/**
 * Updates the film grain
 */
void ppGrainUpdateMode (MATERIAL* m)
{
	if (m == NULL)
		return;

	int mode = ppGrainGetMode(m);
	BMAP* b = (BMAP*)m->PP_GRAIN_MTL_BMAP;

	if (mode == PP_GRAIN_MODE_NONE)
		return;

	var w = screen_size.x;
	var h = screen_size.y;

	if (b != NULL)
	{
		w = b->width;
		h = b->height;
	}

	// store grain bitmap / video size
	m->PP_GRAIN_MTL_SIZE_X = floatv(w);
	m->PP_GRAIN_MTL_SIZE_Y = floatv(h);	

	// tiled grain bitmap
	if (mode == PP_GRAIN_MODE_TILE || mode == PP_GRAIN_MODE_SEQUENCE)
	{
		var s = ppGrainGetScale(m);
		m->PP_GRAIN_MTL_SCALE_X = floatd(1, s);
		m->PP_GRAIN_MTL_SCALE_Y = floatd(1, s);
	}

	// uniformly stretched bitmap
	if (mode == PP_GRAIN_MODE_FULL || mode == PP_GRAIN_MODE_STREAM)
	{
		double normX = 1, normY = 1, scaleX = 1, scaleY = 1;
		ppImageCalcScaling(b, &normX, &normY, &scaleX, &scaleY);

		m->PP_GRAIN_MTL_NORM_X = floatv(normX);
		m->PP_GRAIN_MTL_NORM_Y = floatv(normY);

		m->PP_GRAIN_MTL_SCALE_X = floatv(scaleX);
		m->PP_GRAIN_MTL_SCALE_Y = floatv(scaleY);
	}

	// randomization
	if (mode != PP_GRAIN_MODE_STREAM)
	{
		// timer exceed, generate new random offsets
		if (m->PP_GRAIN_MTL_TIMER < 0)
		{
			// reset timer
			m->PP_GRAIN_MTL_TIMER += m->PP_GRAIN_MTL_SPEED;

			BOOL bRand = true;

			if (mode == PP_GRAIN_MODE_SEQUENCE)
			{
				// switch index
				m->PP_GRAIN_MTL_SEQ_INDEX = (m->PP_GRAIN_MTL_SEQ_INDEX + 1) % 4;

				// update mask

				int i, active;
				for (i = 0; i < 4; i++)
				{
					active = (int)(m->PP_GRAIN_MTL_SEQ_INDEX == i);
					(m->skill)[PP_GRAIN_MTL_SEQ_MASK + i] = floatv(active);
				}

				// randomize only if on first frame of sequence
				bRand = (m->PP_GRAIN_MTL_SEQ_INDEX == 0);
			}

			if (bRand)
			{
				// randomize
				m->PP_GRAIN_MTL_RAND_X = floatv((int)random(w));
				m->PP_GRAIN_MTL_RAND_Y = floatv((int)random(h));
			}
		}
		else
		{
			// keep calm and carry on
			m->PP_GRAIN_MTL_TIMER -= time_step;
		}
	}
}

/**
 * Returns the alpha between 0...100
 */
var ppGrainGetAlpha (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_GRAIN_MTL_ALPHA) * 100;
}

/**
 * Sets the alpha
 * 
 * fAlpha
 *    Translucency of between 0...100
 */
void ppGrainSetAlpha (MATERIAL* m, var fAlpha)
{
	if (m != NULL)
		m->PP_GRAIN_MTL_ALPHA = floatv(fAlpha / 100);
}

/**
 * Returns the scale
 */
var ppGrainGetScale (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_GRAIN_MTL_SCALE);
}

/**
 * Sets the scale
 * 
 * fScale
 *    Scale
 */
void ppGrainSetScale (MATERIAL* m, var fScale)
{
	if (m != NULL)
	{
		m->PP_GRAIN_MTL_SCALE = floatv(fScale);
		ppGrainUpdateMode(m);
	}
}

/**
 * Returns the speed
 */
var ppGrainGetSpeed (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return m->PP_GRAIN_MTL_SPEED;
}

/**
 * Sets the speed
 * 
 * fSpeed
 *    Speed
 */
void ppGrainSetSpeed (MATERIAL* m, var fSpeed)
{
	if (m != NULL)
		m->PP_GRAIN_MTL_SPEED = fSpeed;
}

/**
 * Returns the brightness
 */
var ppGrainGetBrightness (MATERIAL* m)
{
	if (m == NULL)
		return 0;

	return fixv(m->PP_GRAIN_MTL_BRIGHTNESS);
}

/**
 * Sets the brightness
 * 
 * fBrightness
 *    Brightness
 */
void ppGrainSetBrightness (MATERIAL* m, var fBrightness)
{
	if (m != NULL)
		m->PP_GRAIN_MTL_BRIGHTNESS = floatv(fBrightness);
}
