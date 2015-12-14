/* render_pp_mtl.h */
/**
 * Functions for postprocessing materials v0.2 (2013-01-06)
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
#ifndef    render_pp_mtl_h
#define    render_pp_mtl_h

	// creates a new material
	MATERIAL* ppMtlNew (char* fxFilename, char* strTechnique);
	
	// reloads a shader with a selected technique into a given material
	MATERIAL* ppMtlLoad (MATERIAL* m, char* fxFilename, char* strTechnique);

	// lazy initialization of materials
	MATERIAL* ppMtlAssert (MATERIAL**, char* fxFilename, long flags, EVENT);

	// creates a new material as a copy of an existing material
	MATERIAL* ppMtlNewInstance (MATERIAL*);

	// returns the total number of existing materials
	int ppMtlCount ();
	
	// runs each frame an event for a given material, which is passed as argument
	void ppMtlEvent (MATERIAL*, EVENT);
	
	#include "render_pp_mtl.c"
#endif /* render_pp_mtl.h */
