/* render_pp_helper.c */
/**
 * General helper functions for postprocessing applications v0.1 (2012-12-14)
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

/**
 * Returns the next greater power-of-2 number for a given number n, if n is not a power-
 * of-2 number; otherwise n will be returned.
 * 
 * n
 *    Number
 */
int ppNumNextPow2 (int n)
{
	int x = n;

	x--;

	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;

	x++;

	return x;
}

/**
 * Returns fTrue, if b == true; fFalse otherwise (trinary operator)
 * 
 * b
 *    Boolean; if true, fTrue will be returned; if false, fFalse will be returned
 * 
 * fTrue
 *    Will be returned, if b == true
 *            
 * fFalse
 *    Will be returned, if b == false
 */
double ppTrinary (BOOL b, double fTrue, double fFalse)
{
	if (b)
		return fTrue;
	else
		return fFalse;
}
