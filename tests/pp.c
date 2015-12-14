/* pp.c */
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
 * __________________________________________________________________
 * 
 * <pp>
 * 
 * This source file was made specifically for testing the <render_pp*> family 
 * of PP effects.
 * 
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#define   PRAGMA_PATH "../"
#define   PRAGMA_PATH "../object/"
#define   PRAGMA_PATH "../fx/"

// #define   PERFORM_PATH_LOOKUP

#include <acknex.h>

#include "__path_source_test.h"

#include "../source/game/common/path.h"
#include "../source/utils/utilities.h"

#include "../source/render/render_rain.h"
#include "../source/render/render_pp.h"

#define    CRT         0
#define    SEPIA       1
#define    GRAIN       2
#define    GRAYSCALE   3
#define    VIGNETTE    4

int mode              = 0;
int current_shader    = CRT;

VECTOR draw_pos;

const STRING *sstr = "[1] Cathode ray tube (CRT)
[2] Grain
[3] Sepia
[4] Vignette
[5] Grayscale
[6]/[7] Rain on/off";

const STRING *sstr_crt = "[T]/[Y]: Adjust CRT intensity\n[U]/[I]: Adjust gamma\n[J]/[K]: Adjust alpha\n[G]/[H]: Adjust scale\n[B]/[N]: Switch to a different CRT mode";
const STRING *sstr_sepia = "[J]/[K]: Adjust alpha\n[B]/[N]: Switch to a different sepia mode";
const STRING *sstr_gscl  = "[J]/[K]: Adjust alpha\n[B]/[N]: Switch to a different grayscale mode";
const STRING *sstr_vign  = "[C]/[V]: Adjust alpha\n[B]/[N]: Adjust starting coverage";
const STRING *sstr_grain = "[T]/[Y]: Adjust alpha\n[G]/[H]: Adjust scale\n[B]/[N]: Adjust filmgrain speed\n[J]/[K]: Adjust brightness";

STRING *sstr_mode = "";

MATERIAL *uniform = NULL;

double param1 = .0;
double param2 = .0;
double param3 = .0;
double param4 = .0;

void create_crt();
void create_grain();
void create_sepia();
void create_vignette();
void create_grayscale();

void rain_on();
void rain_off();

SOUND *rain = "td1\\rain.ogg";
fixed rhandle = 0;

int main(int argc, char **argl)
{
	while( !ready() ) wait(1.0);
	draw_pos.x = 25.0;
	draw_pos.y = 150.0;
	
	window_size_set(1280, 720);
	
	level_load( "td1\\arena.wmb" );
	vec_set( &camera->x, vector(876, 516, -79) );
	vec_set( &camera->pan, vector(112, 5, 0) );
	object_sky_create( "td1\\envy+6.tga", 1 );
	
	render_rain_new();

	on_1 = create_crt;
	on_2 = create_grain;
	on_3 = create_sepia;
	on_4 = create_vignette;
	on_5 = create_grayscale;
	on_6 = rain_on;
	on_7 = rain_off;
	
	while(!key_esc)
	{	
		draw_text(sstr, 10, 10, COLOR_PEACH);
		draw_text(sstr_mode, draw_pos.x, draw_pos.y, COLOR_BLEU_DE_FRANCE);
		
		camera->pan += 0.25 * time_step;
		
		wait(1.0);
	}
	
	sys_exit(0);
}

__static void __create_stage()
{
	ppViewCreateStaged(camera, uniform);
}

void create_crt()
{
	str_cpy( sstr_mode, sstr_crt );
	mode = PP_CRT_MODE_SCANLINE;
	current_shader = CRT;
	
	uniform = ppCrtNewMtl(mode);
	__create_stage();
}

void create_grain()
{
	str_cpy( sstr_mode, sstr_grain );
	mode = PP_GRAIN_MODE_RANDOM;
	current_shader = GRAIN;
	
	uniform = ppGrainNewMtl(mode, "tStream", vector(1280, 720, 0));
	__create_stage();	
}

void create_sepia()
{
	str_cpy( sstr_mode, sstr_sepia );
	mode = PP_SEPIA_MODE_TINT;
	current_shader = SEPIA;
	
	uniform = ppSepiaNewMtl(mode);
	__create_stage();
}

void create_vignette()
{
	str_cpy( sstr_mode, sstr_vign );
	current_shader = VIGNETTE;
	
	uniform = ppVignetteNewMtl();
	__create_stage();
}

void create_grayscale()
{
	str_cpy( sstr_mode, sstr_gscl );
	mode = PP_GRAYSCALE_MODE_LUMA;
	current_shader = GRAYSCALE;
	
	uniform = ppGrayscaleNewMtl(mode);
	__create_stage();
}

void adjust_parameter_startup()
{
	COLOR *tint = NULL;
	COLOR *vig = NULL;
	
	while(true)
	{
		if(uniform)
		{
			switch(current_shader)
			{
				case CRT:
				{
					draw_text(str_printf(NULL, "Intensity = %.2f\nScale = %.2f\nGamma = %.2f\nAlpha = %.2f", 
					(double) ppCrtGetIntensity(uniform),
					(double) ppCrtGetScale(uniform),
					(double) ppCrtGetGamma(uniform),
					(double) ppCrtGetAlpha(uniform)),
					screen_size.x - 150.0,
					15.0,
					COLOR_WHITE);
					
					param1 = ppCrtGetIntensity(uniform);
					ppCrtSetIntensity(uniform, param1 + (key_t - key_y) * 0.25 * time_step);
					
					param2 = ppCrtGetScale(uniform);
					ppCrtSetScale(uniform, param2 + (key_g - key_h) * 0.25 * time_step);
					
					param3 = ppCrtGetGamma(uniform);
					ppCrtSetGamma(uniform, param3 + (key_u - key_i) * 0.25 * time_step);
					
					param4 = ppCrtGetAlpha(uniform);
					ppCrtSetAlpha(uniform, param4 + (key_j - key_k) * 0.45 * time_step);
					
					if(key_b)
					{
						while(key_b) wait(1.0);
						mode++;
						
						mode = (mode + 1) % PP_CRT_MODE_CNT;
						ppCrtSetMode(uniform, mode);	
					}
					
					if(key_n)
					{
						while(key_n) wait(1.0);
						mode--;
						
					mode = ifelse(mode < 0, PP_CRT_MODE_CNT - 1, mode);
						ppCrtSetMode(uniform, mode);
					}
					
					break;
				}
				
				case SEPIA:
				{
					draw_text(str_printf(NULL, "Alpha = %.2f", 
					(double) ppSepiaGetAlpha(uniform)),
					screen_size.x - 150.0,
					15.0,
					COLOR_WHITE);
					
					param1 = ppSepiaGetAlpha(uniform);
					ppSepiaSetAlpha(uniform, param1 + (key_j - key_k) * 0.75 * time_step);
					
					if (mode == PP_SEPIA_MODE_TINT)
					{
						tint = ppSepiaGetTint(uniform);
						tint->red += (key_t - key_y) * 5 * time_step;
						tint->green += (key_g - key_h) * 5 * time_step;
						tint->blue += (key_u - key_i) * 5 * time_step;

						draw_text(str_printf(NULL, "Custom sepia color (PP_SEPIA_MODE_TINT):\nRed = %d [T]/[Y]\nGreen = %d [G]/[H]\nBlue = %d [U]/[I]",
						(int)tint->red, (int)tint->green, (int)tint->blue), 50, 300, COLOR_WHITE);
						
						ppSepiaSetTint(uniform, tint);
					}
					
					if(key_b)
					{
						while(key_b) wait(1.0);
						mode++;
						
						mode = (mode + 1) % PP_SEPIA_MODE_CNT;
						ppSepiaSetMode(uniform, mode);	
					}
					
					if(key_n)
					{
						while(key_n) wait(1.0);
						mode--;
						
					mode = ifelse(mode < 0, PP_SEPIA_MODE_CNT - 1, mode);
						ppSepiaSetMode(uniform, mode);
					}
					
					break;
				}
				
				case GRAYSCALE:
				{
					draw_text(str_printf(NULL, "Alpha = %.2f", 
					(double) ppGrayscaleGetAlpha(uniform)),
					screen_size.x - 150.0,
					15.0,
					COLOR_WHITE);
					
					param1 = ppGrayscaleGetAlpha(uniform);
					ppGrayscaleSetAlpha(uniform, param1 + (key_j - key_k) * 2.5 * time_step);
					
					if(key_b)
					{
						while(key_b) wait(1.0);
						mode++;
						
						mode = (mode + 1) % PP_GRAYSCALE_MODE_CNT;
						ppGrayscaleSetMode(uniform, mode);	
					}
					
					if(key_n)
					{
						while(key_n) wait(1.0);
						mode--;
						
					mode = ifelse(mode < 0, PP_GRAYSCALE_MODE_CNT - 1, mode);
						ppGrayscaleSetMode(uniform, mode);
					}
					
					break;
				}
				
				case VIGNETTE:
				{
					float coverage_start = ppVignetteGetStart(uniform);
					float coverage_end   = ppVignetteGetEnd(uniform);
					
					draw_text(str_printf(NULL, "Alpha = %.2f\nCoverage = %.2f", 
					(double) ppVignetteGetAlpha(uniform),
					(double) coverage_start),
					screen_size.x - 150.0,
					15.0,
					COLOR_WHITE);
					
					param1 = ppVignetteGetAlpha(uniform);
					param1 += (key_c - key_v) * 1.5 * time_step;
					ppVignetteSetAlpha(uniform, param1);
					
					vig = ppVignetteGetColor(uniform);
					vig->red += (key_t - key_y) * 5 * time_step;
					vig->green += (key_g - key_h) * 5 * time_step;
					vig->blue += (key_u - key_i) * 5 * time_step;
					
					ppVignetteSetColor(uniform, vig);
					
					draw_text(str_printf(NULL, "Custom vignette color:\nRed = %d [T]/[Y]\nGreen = %d [G]/[H]\nBlue = %d [U]/[I]",
					(int)vig->red, (int)vig->green, (int)vig->blue), 50, 300, COLOR_WHITE);
					
					coverage_start += 0.125 * (key_b - key_n) * time_step;
					
					ppVignetteSetStart(uniform, coverage_start);
					ppVignetteSetEnd(uniform, coverage_end);
					
					break;
				}
				
				case GRAIN:
				{
					draw_text(str_printf(NULL, "Alpha = %.2f\nScale = %.2f\nSpeed = %.2f\nBrightness = %.2f", 
					(double) ppGrainGetAlpha(uniform),
					(double) ppGrainGetScale(uniform),
					(double) ppGrainGetSpeed(uniform),
					(double) ppGrainGetBrightness(uniform)),
					screen_size.x - 150.0,
					15.0,
					COLOR_WHITE);
					
					param1 = ppGrainGetAlpha(uniform);
					param1 += (key_t - key_y) * time_step * 0.55;
					ppGrainSetAlpha(uniform, param1);
					
					param2 = ppGrainGetScale(uniform);
					param2 += (key_g - key_h) * time_step * 0.55;
					ppGrainSetScale(uniform, param2);
					
					param3 = ppGrainGetSpeed(uniform);
					param3 += (key_b - key_n) * time_step * 0.55;
					ppGrainSetSpeed(uniform, param3);
					
					param4 = ppGrainGetBrightness(uniform);
					param4 += (key_j - key_k) * time_step * 0.15;
					ppGrainSetBrightness(uniform, param4);
					
					break;
				}
			}
		}
		
		wait(1.0);
	}
}

void rain_on()
{
	render_rain_start();
	rhandle = snd_loop(rain, 100, 0);
}

void rain_off()
{
	render_rain_stop();
	snd_stop(rhandle);
}
