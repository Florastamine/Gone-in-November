/* render_pp_image.c */
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

/**
 * Creates a new uniform background image postprocessing shader for a given bitmap. If
 * no image is passed, it can be later set with ppImageSetMtlImg. Returns the material
 * containing the shader; must be applied to a staged postprocessing view.
 * 
 * bmapImg
 *    The image
 */
MATERIAL* ppImageNewMtl (BMAP* bmapImg)
{
	MATERIAL* m = ppMtlNewInstance(ppImageGetMtlFx());
	ppImageSetMtlImg(m, bmapImg);

	ppImageSetMtlImgOffset(m, def_ppImageMtlOffset);

	return m;
}


/**
 * Changes the image of the material's postprocessing shader
 * 
 * m
 *    Material, containing the uniform background image postprocessing shader created with
 *    the function ppImageNewMtl
 * 
 * bmapImg
 *    The new image
 */
void ppImageSetMtlImg (MATERIAL* m, BMAP* bmapImg)
{
	if (m != NULL)
		m->PP_IMAGE_MTL_IMG = bmapImg;
}


/**
 * Gets the offset of an image
 *
 * offset
 *    Receives the offset in x and y direction, between (-1..0..+1)
 */
VECTOR* ppImageGetMtlImgOffset (MATERIAL* m, VECTOR* offset)
{
	if (m != NULL)
	{
		VECTOR* v = offset;

		if (v == NULL)
			v = vector(0,0,0);

		v->x = fixv(m->PP_IMAGE_MTL_OFFSET_X);
		v->y = fixv(m->PP_IMAGE_MTL_OFFSET_Y);

		return v;
	}
	else
		return NULL;
}


/**
 * Adds an offset to the image
 *
 * offset
 *    Offset in x and y direction, between (-1..0..+1)
 */
void ppImageSetMtlImgOffset (MATERIAL* m, VECTOR* offset)
{
	if (m != NULL && offset != NULL)
	{
		m->PP_IMAGE_MTL_OFFSET_X = floatv(offset->x);
		m->PP_IMAGE_MTL_OFFSET_Y = floatv(offset->y);
	}
}



/**
 * Returns a singleton-material containing the shader code
 */
MATERIAL* ppImageGetMtlFx ()
{
	ppMtlAssert(&g_ppImageMtlFx, "ppImage.fx", ENABLE_VIEW, mtlPpImage_ev);
}




/**
 * Updates the normalization and scaling parameters of the material's image
 */
void mtlPpImage_ev ()
{
	BMAP* img = mtl->PP_IMAGE_MTL_IMG;

	double normX, normY, scaleX, scaleY;
	ppImageCalcScaling(img, &normX, &normY, &scaleX, &scaleY);

	mtl->PP_IMAGE_MTL_NORM_X = floatv(normX);
	mtl->PP_IMAGE_MTL_NORM_Y = floatv(normY);
	mtl->PP_IMAGE_MTL_SCALE_X = floatv(scaleX);
	mtl->PP_IMAGE_MTL_SCALE_Y = floatv(scaleY);
}


/**
 * Calculates the normalization and scaling parameters of a uniformly scaled image
 *
 * bmapImg
 *    The image
 *
 * refNormX & -Y
 *    Double pointers, which receive the normalization factors for non-power-of-2 images
 *
 * refScaleX & -Y
 *    Double pointers, which receive the scaling factors for making the image fit the
 *    screen while being uniformly scaled
 */
void ppImageCalcScaling (BMAP* bmapImg, double* refNormX, double* refNormY, double* refScaleX, double* refScaleY)
{
	if (bmapImg == NULL)
		return;

	double w = bmap_width(bmapImg);
	double h = bmap_height(bmapImg);

	// normalization factor for non-power-of-2 images
	*refNormX = w / ppTrinary(bmapImg->finalwidth < 1, 1, bmapImg->finalwidth+2);
	*refNormY = h / ppTrinary(bmapImg->finalheight < 1, 1, bmapImg->finalheight+2);

	// bitmap- and screen aspect ratio
	double bmapAspect = w / h;
	double screenAspect = (double)screen_size.x / (double)screen_size.y;

	// the aspect ratios are compared to determine if the image is relatively wider or
	// taller than the screen. If the image is taller, it will be fitted on the x-axis and
	// will be centered on the y-axis; and vice versa.

	if (bmapAspect < screenAspect) // taller
	{
		*refScaleX = 1;
		*refScaleY = bmapAspect / screenAspect;
	}
	else // wider
	{
		*refScaleX = screenAspect / bmapAspect;
		*refScaleY = 1;
	}
}
