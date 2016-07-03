/* render_pp.h */
/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to per-
 * mit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, IN
 * CLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LI-
 * ABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT-
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * __________________________________________________________________
 * 
 * <render_pp>
 * 
 * Contains postprocessing effects.
 * 
 * Authors: Christian Behrenberg (http://www.christian-behrenberg.com)
 *          Modified and packaged by Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    A8_FREE

#ifndef    __RENDER_PP_H__
#define    __RENDER_PP_H__

#define PRAGMA_PRINT "[Postprocessing] "

#define    __RENDER_PP_CRT
#define    __RENDER_PP_GRAIN
#define    __RENDER_PP_GRAYSCALE
#define    __RENDER_PP_IMAGE
#define    __RENDER_PP_SEPIA
#define    __RENDER_PP_VIGNETTE

#ifdef    __RENDER_PP_CRT
    #include "render_pp_view.h"
    #include "render_pp_image.h"

    #include "render_pp_crt.h"
#endif

#ifdef    __RENDER_PP_GRAIN
    #include "render_pp_view.h"
    #include "render_pp_image.h"

    #include "render_pp_grain.h"
#endif

#ifdef    __RENDER_PP_GRAYSCALE
    #include "render_pp_view.h"
    #include "render_pp_image.h"

    #include "render_pp_grayscale.h"

#endif

#ifdef    __RENDER_PP_IMAGE
    #include "render_pp_view.h"
    #include "render_pp_image.h"
#endif

#ifdef    __RENDER_PP_SEPIA
    #include "render_pp_view.h"
    #include "render_pp_image.h"

    #include "render_pp_sepia.h"
#endif

#ifdef    __RENDER_PP_VIGNETTE
    #include "render_pp_view.h"
    #include "render_pp_image.h"

    #include "render_pp_vignette.h"
#endif

#include "render_pp.c"
#endif /* render_pp.h */
#endif
