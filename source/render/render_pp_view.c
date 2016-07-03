/* render_pp_view.c */
/**
 * Functions for postprocessing views v0.1 (2012-12-14)
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
 * Creates and returns a new postprocessing view for a given material, which contains a
 * postprocessing shader. The view is always creates on layer = 0 and has the flags
 * CHILD | PROCESS_TARGET activated.
 * 
 * m
 *    Material with postprocessing shader
 */
VIEW* ppViewCreate (MATERIAL* m)
{
	if (m == NULL)
		return NULL;

	VIEW* v = view_create(0);
	if (v != NULL)
	{
		v->material = m;
		v->flags |= (CHILD | PROCESS_TARGET);
	}

	return v;
}

/**
 * Creates and stages a new postprocessing view for a given material to a given view. The
 * newly created view will be returned.
 * 
 * pre
 *    View which leads with .stage to the newly created postprocessing view
 * 
 * m
 *    Material with postprocessing shader
 */
VIEW* ppViewCreateStaged (VIEW* pre, MATERIAL* m)
{
	if (pre == NULL || m == NULL)
		return;

	VIEW* v = ppViewCreate(m);
	pre->stage = v;

	return v;
}
