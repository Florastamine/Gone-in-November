////////////////////////////////////////////////////////////////////
//View for post processing effects
//
//desc: for setting a pp effect, call pp_set(camera,material);
////////////////////////////////////////////////////////////////////

#ifndef mtlView
#define mtlView

#define PRAGMA_PATH "%EXE_DIR%\\code";
#define PRAGMA_BIND "d3dcompiler_42.dll"; // DirectX 9.c shader compiler

////////////////////////////////////////////////////////////////////
//Shader Name: HDR
//Material Name: mtl_hdr
//Variable1: Strength
//Variable2: Threshold
//Variable3: facExposure
//Variable4: timeExposure

void hdr_event();

MATERIAL* mtl_hdr =
{
	event = hdr_event;
}

////////////////////////////////////////////////////////////////////
//Shader Name: Blur
//Material Name: mtl_blur
//Variable1: facBlur
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_blur =
{
	effect = "pp_blur.fx";
	skill11 = 30; // skill11..14 are copied to skill1..4
}
////////////////////////////////////////////////////////////////////
//Shader Name: Gaussian Blur
//Material Name: mtl_gaussian
//Variable1: facBlur
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_gaussian =
{
	effect = "pp_gaussian.fx";
	skill11 = 30;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Dilate
//Material Name: mtl_dilate
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_dilate =
{
	effect = "pp_dilate.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Displace
//Material Name: mtl_displace
//Variable1: displacement
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_displace =
{
	effect = "pp_displace.fx";
	skill11 = 50;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Erode
//Material Name: mtl_erode
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_erode =
{
	effect = "pp_erode.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Kuwahara
//Material Name: mtl_kuwahara
//Variable1: nPixels
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_kuwahara =
{
	effect = "pp_kuwahara.fx";
	skill11 = 50;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Median
//Material Name: mtl_median
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used
MATERIAL* mtl_median =
{
	effect = "pp_median.fx";
}

////////////////////////////////////////////////////////////////////
//Shader Name: Sharpen
//Material Name: mtl_sharpen
//Variable1: facSharpen
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_sharpen =
{
	effect = "pp_sharpen.fx";
	skill11 = 50;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Sharpen More
//Material Name: mtl_sharpen2
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_sharpen2 =
{
	effect = "pp_sharpen2.fx";
}

////////////////////////////////////////////////////////////////////
//Shader Name: Bleach
//Material Name: mtl_bleach
//Variable1: opacity
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_bleach =
{
	effect = "pp_bleach.fx";
	skill11 = 50;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Desaturate
//Material Name: mtl_desaturate
//Variable1: desat
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_desaturate =
{
	effect = "pp_desaturate.fx";
	skill11 = 50;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Sepia
//Material Name: mtl_sepia
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_sepia =
{
	effect = "pp_sepia.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Monochrome
//Material Name: mtl_monochrome
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_monochrome =
{
	effect = "pp_monochrome.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Negative
//Material Name: mtl_negative
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_negative =
{
	effect = "pp_negative.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Colorshift
//Material Name: mtl_colorshift
//Variable1: xShift
//Variable2: yShift
//Variable3: zShift
//Variable4: Not used
MATERIAL* mtl_colorshift =
{
	effect = "pp_colorshift.fx";
	skill11 = 50;
	skill12 = 50;
	skill13 = 50;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Colorspin
//Material Name: mtl_colorspin
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_colorspin =
{
	effect = "pp_colorspin.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Emboss
//Material Name: mtl_emboss
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_emboss =
{
	effect = "pp_emboss.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Laplace
//Material Name: mtl_laplace
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_laplace =
{
	effect = "pp_laplace.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Sobel
//Material Name: mtl_sobel
//Variable1: Not used
//Variable2: Not used
//Variable3: Not used
//Variable4: Not used
MATERIAL* mtl_sobel =
{
	effect = "pp_sobel.fx";
}
////////////////////////////////////////////////////////////////////
//Shader Name: Posterize
//Material Name: mtl_posterize
//Variable1: nColors
//Variable2: gamma
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_posterize =
{
	effect = "pp_posterize.fx";
	skill11 = 50;
	skill12 = 50;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Lens
//Material Name: mtl_lens
//Variable1: xPos
//Variable2: yPos
//Variable3: Not used
//Variable4: Not used

MATERIAL* mtl_lens =
{
	effect = "pp_lens.fx";
	skill11 = 50;
	skill12 = 50;
}
////////////////////////////////////////////////////////////////////
//Shader Name: Bias
//Material Name: mtl_bias
//Variable1: bias
//Variable2: gradient
//Variable3: Not used
//Variable4: Not used
MATERIAL* mtl_bias =
{
	effect = "pp_bias.fx";
	skill11 = 50;
	skill12 = 50;
}

///////////////////////////////////////////////////////////////////
// Section : mtlView helper functions

VIEW* pp_laststage(VIEW* view)
{
	if(!view) view = camera;
	while(view->stage) view = view->stage;
	return view;
}

VIEW* pp_findstage(VIEW* view)
{
	VIEW *v;
	for(v = ptr_first(camera); v; v = v->link.next)
		if(view == v->stage) return v;
	return camera;
}

VIEW* pp_stage = NULL;
VIEW* pp_view = NULL;

function pp_event() { return; }

function pp_set(VIEW* view,MATERIAL* m)
{
	if(!pp_view) pp_view = camera;
	if(!view) view = camera;
	if(m) {
// store original view
	  pp_view = view;
// create new view stage for postprocessing
		if(!pp_stage) pp_stage = view_create(-1);
		pp_stage->material = m;
		pp_stage->flags |= PROCESS_TARGET;
		pp_stage->size_x = view->size_x;
		pp_stage->size_y = view->size_y;
		view->stage = pp_stage;
// set up material skill default values
		if(m->skill11) m->skill1 = floatv(m->skill11);
		if(m->skill12) m->skill2 = floatv(m->skill12);
		if(m->skill13) m->skill3 = floatv(m->skill13);
		if(m->skill14) m->skill4 = floatv(m->skill14);
// make sure material event is executed before the next stage is assigned
		if(m->event) { pp_event = m->event; pp_event(); }
	} else {
		view->stage = NULL;
		if(pp_stage) pp_stage->material = NULL;
	}
}

function pp_add(MATERIAL* m)
{
	if(!pp_view) pp_view = camera;
	if(!pp_stage) pp_stage = camera;
	if(m) {
		VIEW* view = pp_laststage(pp_stage);
		pp_stage = NULL;
		pp_set(view,m);
	} else {
		ptr_remove(pp_stage);
		pp_stage = pp_view;
		pp_stage->stage = NULL;
		pp_view = pp_findstage(pp_stage);
	}
}

function mtl_setup(MATERIAL* m,v1,v2,v3,v4)
{
// copy default variables to vecSkill1
	if (v1 && !m->skill1) m->skill1 = floatv(v1);
	if (v2 && !m->skill2) m->skill2 = floatv(v2);
	if (v3 && !m->skill3) m->skill3 = floatv(v3);
	if (v4 && !m->skill4) m->skill4 = floatv(v4);
}

MATERIAL* mtl_createshader(char* filename,char* technique)
{
	MATERIAL* m = mtl_create();
	m->technique = technique;
	effect_load(m,filename);
	return m;
}

// adapt a bitmap size, reallocate if necessary
BMAP* bmap_setsize(BMAP* bmap,dx,dy,format)
{
	if(!bmap)
	  bmap = bmap_createblack(dx,dy,format);
	else if(bmap_width(bmap) != dx || bmap_height(bmap) != dy)
	{
		if(!format) format = bmap_format(bmap);
		ptr_remove(bmap);
		bmap = bmap_createblack(dx,dy,format);
	}
	return bmap;
}

BMAP* view_settarget(VIEW* view,factor,format)
{
	if(!format || !factor) {
		if(view.bmap) {
			ptr_remove(view.bmap);
			view.bmap = NULL;
			view.size_x = 0;
			view.size_y = 0;
		}
	} else {
		view.size_x = screen_size.x/factor;
		view.size_y = screen_size.y/factor;
		view.bmap = bmap_setsize(view.bmap,view.size_x,view.size_y,format);
	}
	return view.bmap;
}

/////////////////////////////////////////////////
// section: HDR shader
#define fx_hdr "pp_hdr.fx"
#define fx_gamma "pp_gamma.fx"
#define EXPOSURE_COMPENSATION // comment this out for no exposure (faster)

void hdr_event()
{
// must only be started once, as it runs forever
	mtl_hdr->event = NULL;
	mtl_hdr->technique = "filterStart"; // highpass filter
	effect_load(mtl_hdr,fx_hdr);
	mtl_setup(mtl_hdr,50,50,0,50);

// create 3 filter stages for the bloom effect; last filter is output stage
	int i;
	VIEW *viewFilter[3];
	for(i=0; i<3; i++) {
		viewFilter[i] = view_create(-1);
		viewFilter[i]->flags |= PROCESS_TARGET;
		if(i>0) viewFilter[i-1]->stage = viewFilter[i];
	}

	viewFilter[0]->material = mtl_createshader(fx_hdr,"filterH");
	viewFilter[1]->material = mtl_createshader(fx_hdr,"filterV");
	viewFilter[2]->material = mtl_createshader(fx_hdr,"filterEnd");

#ifdef EXPOSURE_COMPENSATION
// create 4 downsample stages for sampling the overall scene brightness
	VIEW *viewGamma[4];
	for(i=0; i<4; i++) {
		viewGamma[i] = view_create(-1);
		viewGamma[i]->flags |= PROCESS_TARGET;
		int size = 64 >> (2*i);
		viewGamma[i]->size_x = viewGamma[i]->size_y = size;
		viewGamma[i]->bmap = bmap_createblack(size,size,32);
		if(i>0) viewGamma[i-1]->stage = viewGamma[i];
	}

	viewGamma[0]->material = mtl_createshader(fx_gamma,"GammaStart");
	viewGamma[1]->material = mtl_createshader(fx_gamma,"GammaSample");
	viewGamma[2]->material = mtl_createshader(fx_gamma,"GammaSample");
	viewGamma[3]->material = mtl_createshader(fx_gamma,"GammaEnd");
#endif

	BMAP *cameraTarget = NULL, *ppTarget = NULL;
	VIEW *cameraView = pp_view, *ppView = pp_stage;

	while(1)
	{
		if(ppView.material == mtl_hdr) // HDR active?
	  {
			cameraTarget = view_settarget(cameraView,1,32);
			ppTarget = view_settarget(ppView,4,32);
			ppView.stage = viewFilter[0];

			view_settarget(viewFilter[0],4,32);
			view_settarget(viewFilter[1],4,32);

			viewFilter[2]->material->skill1 = mtl_hdr->skill1; // Strength
			viewFilter[2]->material->skill2 = mtl_hdr->skill2; // Threshold
			viewFilter[2]->material->skill3 = mtl_hdr->skill3; // Exposure Compensation Factor
			viewFilter[2]->material->skin1 = cameraTarget;
#ifdef DEBUG_HDR
			DEBUG_BMAP(viewSecond->bmap,10,0.5);
			DEBUG_BMAP(viewFilter[0]->bmap,150,0.5);
			DEBUG_BMAP(viewFilter[1]->bmap,290,0.5);
#endif
#ifdef EXPOSURE_COMPENSATION
			if(mtl_hdr.skill3 > 0) { // enable exposure compensation
				viewFilter[2]->material->skin2 = viewGamma[3]->bmap;	// 1-pixel luminance
				viewGamma[0]->material->skin2 = viewGamma[3]->bmap;		// 1-pixel luminance
				viewGamma[0]->material->skin1 = viewFilter[1]->bmap;	// bloom
				viewGamma[0]->material->skill1 =
					floatv(bmap_width(viewGamma[0]->material->skin1)/bmap_width(viewGamma[0]->bmap));
				viewGamma[0]->material->skill2 =
					floatv(bmap_height(viewGamma[0]->material->skin1)/bmap_height(viewGamma[0]->bmap));
				viewGamma[3]->material->skill4 = mtl_hdr->skill4; // Exposure Compensation Delay
				set(viewGamma[0],SHOW);
#ifdef DEBUG_HDR
				DEBUG_BMAP(viewGamma[0]->bmap,450,1);
				DEBUG_BMAP(viewGamma[1]->bmap,520,2);
				DEBUG_BMAP(viewGamma[2]->bmap,570,3);
				DEBUG_BMAP(viewGamma[3]->bmap,590,4);
#endif
			}
#endif
		} else
		{
// temporarily disabled - cleanup if necessary
			if(cameraView.bmap == cameraTarget) view_settarget(cameraView,0,0);
			if(ppView.bmap == ppTarget) view_settarget(ppView,0,0);
			if(ppView.stage == viewFilter[0]) ppView.stage = NULL;
#ifdef EXPOSURE_COMPENSATION
			reset(viewGamma[0],SHOW);
#endif
		}
		wait(1);
	}
}

#endif
