/* render_pp_mtl.c */
/**
 * Functions for postprocessing materials v0.2 (2013-01-06)
 *
 * Copyright (C) 2012, 2013 by Christian Behrenberg
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
 * Creates a new material with a shader and a selected technique
 * 
 * fxFilename
 *    Shader file; if NULL, no shader will be loaded
 * 
 * strTechnique
 *    If != NULL, the technique with the given name will be selected
 */
MATERIAL* ppMtlNew (char* fxFilename, char* strTechnique)
{
	MATERIAL* m = mtl_create();
	
	if (m != NULL)
	{
		ppMtlLoad(m, fxFilename, strTechnique);
	}
	
	return m;
}

/**
 * Reloads a shader with a selected technique into a given material
 * 
 * fxFilename
 *    Shader file; if NULL, no shader will be loaded
 * 
 * strTechnique
 *    If != NULL, the technique with the given name will be selected
 */
MATERIAL* ppMtlLoad (MATERIAL* m, char* fxFilename, char* strTechnique)
{
	if (m == NULL)
		return NULL;
	
	if (strTechnique != NULL)
		m->technique = strTechnique;

	if (fxFilename != NULL)
		effect_load(m, fxFilename);
			
	return m;
}

/**
 * Makes sure that a material exists with a given shader, flags and event. This can be
 * used to delay the material creation (and shader compilation) until the first time it is
 * needed. Returns the asserted material.
 * 
 * refMtl
 *    Reference to the asserted material; must be != NULL
 * 
 * fxFilename
 *    Filename of a shader which shall be loaded into the material. Will only be loaded
 *    into the material, if != NULL and the material has no shader loaded
 *            
 * flags
 *    Material flags; will be replaced completely if != 0
 * 
 * ev
 *    Event function; will only be changed, if != NULL
 */
MATERIAL* ppMtlAssert (MATERIAL** refMtl, char* fxFilename, long flags, EVENT ev)
{
	if (refMtl == NULL)
		return NULL;
		
	MATERIAL* m = *refMtl;
	
	// if reference is empty, create material
	if (m == NULL)
		*refMtl = m = mtl_create();
	
	if (m == NULL)
		return NULL;
	
	// if an .fx is passed and no effect was loaded, load shader
	if (m->effect == NULL && fxFilename != NULL)
		effect_load(m, fxFilename);
	
	// flags are given, the old ones will be replaced with the current ones
	if (flags != 0)
		m->flags = flags;
	
	// if an event is given, it will be replaced	
	if (ev != NULL)
		m->event = ev;
	
	return m;
}

/**
 * Creates a new material as a copy of an existing material. Supported are shaders, flags
 * and the event. The copy will be returned.
 * 
 * srcMtl
 *    The existing material, which serves as template for the new material.
 */
MATERIAL* ppMtlNewInstance (MATERIAL* srcMtl)
{
	if (srcMtl == NULL)
		return NULL;
		
	MATERIAL* m = mtl_create();
	
	if (m == NULL)
		return NULL;
	
	if (srcMtl->effect != NULL)
		effect_cpy(m, srcMtl);
		
	m->flags = srcMtl->flags;
	m->event = srcMtl->event;
	
	return m;
}

/**
 * Returns the total number of existing materials
 */
int ppMtlCount ()
{
	MATERIAL* mtlInstance = mtl_create();
	MATERIAL* ptrMtl = ptr_first(mtlInstance);
	
	int num = 0;
	while (ptrMtl != NULL)
	{
		num++;
		ptrMtl = ptrMtl->link.next;
	}
	
	if (mtlInstance != NULL)
		ptr_remove(mtlInstance);
	
	return num - 1;
}

/**
 * Runs each frame an event for a given material, which is passed as argument
 *
 * m
 *    The material, for which the event is executed each frame
 *
 * ev
 *    Event function of the formt <void fnc (MATERIAL*)>, which will be called each
 *    frame for the given material m
 */
void ppMtlEvent (MATERIAL* m, EVENT ev)
{
	var h = handle(m);
	
	while (1)
	{
		MATERIAL* p = ptr_for_handle(h);
		if (p != NULL)
		{
			void f (MATERIAL*);
			f = ev;
			
			f(m);
		}
		else
			break;
		
		wait(1);
	}
}
