/* render_hdr.c */
/*
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                     Version 2, December 2004
 * 
 *  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 * 
 *  Everyone is permitted to copy and distribute verbatim or modified
 *  copies of this license document, and changing it is allowed as long
 *  as the name is changed.
 *
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 * 
 *   0. You just DO WHAT THE FUCK YOU WANT TO.
 */

/*
 * HDRState *render_hdr_get_singleton()
 * 
 * Returns the singleton of the HDR state object.
 * Can be used to query HDR parameters.
 */
HDRState *render_hdr_get_singleton()
{
	return HDRState_singleton;
}

/*
 * void render_hdr_free()
 * 
 * Frees the HDR state.
 * This struct only contains HDR parameters, and thus it's safe 
 * to be freed after the call to render_hdr() without affecting the 
 * "real" HDR being rendered on the screen, but you won't be able 
 * to query parameters afterwards.
 */
void render_hdr_free()
{
	if(HDRState_singleton) sys_free(HDRState_singleton);
}

/*
 * void render_hdr_set_queued( __In BOOL state )
 * 
 * Queue HDR for rendering.
 * 
 */
void render_hdr_set_queued( __In BOOL state )
{
	if(HDRState_singleton) HDRState_singleton->queued = state;
}

/*
 * void render_hdr_new()
 * 
 * Allocates memory for the HDR state singleton, and set to default values.
 * Overriding values is done through direct query of the singleton (render_hdr_get_singleton()->field), 
 * or through render_hdr_setup(). Neither must be done before HDR is activated with render_hdr().
 */
void render_hdr_new()
{
	HDRState_singleton = (HDRState *) sys_malloc(sizeof(HDRState));
	
	HDRState_singleton->rt_factor = 4;
	HDRState_singleton->bit_depth = 32;
	HDRState_singleton->light_scattering = 0.001;
	HDRState_singleton->blur = 4;
	HDRState_singleton->bloom_strength = 80;
	HDRState_singleton->highpass_luminance = 0.09;
	HDRState_singleton->highpass_middle_grey = 0.18;
	HDRState_singleton->highpass_white_cutoff = 0.8;
	HDRState_singleton->adaption_speed = 2;
	
	HDRState_singleton->queued = false;
}

/*
 * BOOL render_hdr_get_queued()
 * 
 * Yields true if HDR is queued (queried for rendering), false otherwise.
 */
BOOL render_hdr_get_queued()
{
	if(HDRState_singleton) return HDRState_singleton->queued;
}

/*
 * void render_hdr_setup(...)
 * 
 * Overrides default HDR parameters with a new one.
 * Certain arguments can be ignored; simply pass (-1) to which parameters you want to ignore.
 */
void render_hdr_setup(
                       float RT,
                       float bit_depth,
                       float light_scattering,
                       float blur,
                       float bloom_strength,
                       float highpass_luminance,
                       float highpass_middle_grey,
                       float highpass_white_cutoff,
                       float adaption_speed )
{
	if(HDRState_singleton)
	{
		HDRState_singleton->rt_factor                   = ifelse(RT == -1, HDRState_singleton->rt_factor, RT);
		HDRState_singleton->bit_depth                   = ifelse(bit_depth == -1, HDRState_singleton->bit_depth, bit_depth);
		
		HDRState_singleton->light_scattering            = ifelse(light_scattering == -1, HDRState_singleton->light_scattering, light_scattering);
		HDRState_singleton->blur                        = ifelse(blur == -1, HDRState_singleton->blur, blur);
		HDRState_singleton->bloom_strength              = ifelse(bloom_strength == -1, HDRState_singleton->bloom_strength, bloom_strength);
		HDRState_singleton->highpass_luminance          = ifelse(highpass_luminance == -1, HDRState_singleton->highpass_luminance, highpass_luminance);
		HDRState_singleton->highpass_middle_grey        = ifelse(highpass_middle_grey == -1, HDRState_singleton->highpass_middle_grey, highpass_middle_grey);
		HDRState_singleton->highpass_white_cutoff       = ifelse(highpass_white_cutoff == -1, HDRState_singleton->highpass_white_cutoff, highpass_white_cutoff);
		HDRState_singleton->adaption_speed              = ifelse(adaption_speed == -1, HDRState_singleton->adaption_speed, adaption_speed);
	}
}

/*
 * void render_hdr()
 * 
 * Performs HDR rendering.
 */
void render_hdr()
{
	__render_hdr_initialize();
	
	#ifdef __DOF
	{
		if(render_hdr_get_queued())	
		    view_dof->stage = view_hdrDownsample;
		else
		    camera->stage = view_hdrDownsample;
	}
	#else
		camera->stage = view_hdrDownsample;
	#endif
	
	view_hdrDownsample->stage = view_hdrHighpass;
	view_hdrHighpass->stage = view_hdrBlur;
	view_hdrBlur->stage = view_hdrHBlur;
	view_hdrHBlur->stage = view_hdrVBlur;
	view_hdrVBlur->stage = view_hdr;
}

__static void __render_hdr_initialize()
{
	view_hdrGamma->flags |= (SHOW);
	
	__static float rt_factor = (render_hdr_get_singleton())->rt_factor;
	__static float bits = (render_hdr_get_singleton())->bit_depth;
	
	mtl_hdr->skill4 = floatv(rt_factor);
	mtl_hdrDownsample->skill1 = floatv(rt_factor);
	mtl_hdrBlur->skill1 = floatv((render_hdr_get_singleton())->light_scattering);
	mtl_hdrHBlur->skill1 = floatv((render_hdr_get_singleton())->blur);
	mtl_hdrVBlur->skill1 = floatv((render_hdr_get_singleton())->blur);
	mtl_hdrHighpass->skill1 = floatv((render_hdr_get_singleton())->bloom_strength);
	mtl_hdrHighpass->skill2 = floatv((render_hdr_get_singleton())->highpass_luminance);
	mtl_hdrHighpass->skill3 = floatv((render_hdr_get_singleton())->highpass_middle_grey);
	mtl_hdrHighpass->skill4 = floatv((render_hdr_get_singleton())->highpass_white_cutoff);
	mtl_hdrGamma3->skill1 = floatv((render_hdr_get_singleton())->adaption_speed);
	mtl_hdr->skill1 = floatv((render_hdr_get_singleton())->highpass_luminance);
	
	view_hdrDownsample->arc = camera->arc;
	view_hdrDownsample->size_x = screen_size.x/rt_factor;
	view_hdrDownsample->size_y = screen_size.y/rt_factor;
	view_hdrDownsample->bmap = bmap_createblack(screen_size.x/rt_factor,screen_size.y/rt_factor,bits);
	view_hdrHighpass->arc = camera->arc;
	view_hdrHighpass->size_x = screen_size.x/rt_factor;
	view_hdrHighpass->size_y = screen_size.y/rt_factor;
	view_hdrHighpass->bmap = bmap_createblack(screen_size.x/rt_factor,screen_size.y/rt_factor,bits);
	view_hdrBlur->arc = camera->arc;
	view_hdrBlur->size_x = screen_size.x/rt_factor;
	view_hdrBlur->size_y = screen_size.y/rt_factor;
	view_hdrBlur->bmap = bmap_createblack(screen_size.x/rt_factor,screen_size.y/rt_factor,bits);
	view_hdrHBlur->arc = camera->arc;
	view_hdrHBlur->size_x = screen_size.x/rt_factor;
	view_hdrHBlur->size_y = screen_size.y/rt_factor;
	view_hdrHBlur->bmap = bmap_createblack(screen_size.x/rt_factor,screen_size.y/rt_factor,bits);
	view_hdrVBlur->arc = camera->arc;
	view_hdrVBlur->size_x = screen_size.x/rt_factor;
	view_hdrVBlur->size_y = screen_size.y/rt_factor;
	view_hdrVBlur->bmap = bmap_createblack(screen_size.x/rt_factor,screen_size.y/rt_factor,bits);
	view_hdr->arc = camera->arc;
	view_hdr->size_x = screen_size.x;
	view_hdr->size_y = screen_size.y;
	
	view_hdrGamma->stage = view_hdrGamma2;
	view_hdrGamma2->stage = view_hdrGamma3;
	view_hdrGamma3->stage = view_hdrGamma4;
	view_hdrGamma4->bmap = (RenderState_get_singleton())->bmap_gamma;
	
	mtl_hdr->skin1 = (RenderState_get_singleton())->map_scene;
	
	#ifdef __DOF
		if( (render_dof_get_singleton())->queued)
		{
			mtl_hdr->skin1 = RenderState_singleton->bmap_dof;
		}
	#endif
	
	mtl_hdr->skin2 = (RenderState_get_singleton())->bmap_gamma;
	mtl_hdrHighpass->skin1 = (RenderState_get_singleton())->bmap_gamma;
	mtl_hdrGamma->skin1 = (RenderState_get_singleton())->map_scene;
	mtl_hdrGamma->skin2 = (RenderState_get_singleton())->bmap_gamma;
}
