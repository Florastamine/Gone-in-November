/* render_surface.h */
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
 * 
 * __________________________________________________________________
 * 
 * <render_surface>
 * A collection of ready-to-use object-based shaders.
 * Fur (SM1.x) - (weird - Intel HD 4400 couldn't correctly render this?)
 * Grass (SM1.x)
 * Generic water shader (SM1.x)
 * Chrome (SM2.0)
 * Fixed light rays
 * Generic blur (SM2.0)
 * Ocean (SM2.0) - weird rendering artifacts with HD4400?
 * Velvety (SM3.0)
 * 
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 *          Slin
 *          mocosgames
 *          ello
 *          PHeMoX
 *          Steempipe
 * Shader definitions are preceded by smtl_* rather than mtl_* in order to 
 * avoid conflicts with Gamestudio default materials.
 * __________________________________________________________________
 */
#ifndef    __RENDER_SURFACE_H__
#define    __RENDER_SURFACE_H__

/* -- Fur -- */
BMAP *__smtl_fur__noise;
BMAP *__smtl_fur__color;

MATERIAL *smtl_fur = { effect = "mtlFur.fx"; }

void smtl_fur_add(ENTITY *object);

/* -- Grass -- */
MATERIAL *smtl_grass = { effect = "mtlGrass.fx"; }

void smtl_grass_add(ENTITY *object);

/* -- Water shader -- */
BMAP *__smtl_water__bump;

MATERIAL *smtl_water = { flags = TANGENT; effect = "mtlWater.fx"; }

void smtl_water_add(ENTITY *object, BMAP *reflection);

/* -- Chrome -- */
BMAP *__smtl_chrome__lookup;

MATERIAL *smtl_chrome = { effect = "mtlChrome.fx"; }

void smtl_chrome_add(ENTITY *object, VECTOR *normalized_color);

/* -- Simple light rays -- */
void smtl_lrays_add(ENTITY *object);

/* -- Generic blur -- */
MATERIAL *smtl_gblur = { effect = "mtlBlur.fx"; }

void smtl_blur_add(ENTITY *object);

/* -- Ocean -- */
MATERIAL *smtl_ocean = { effect = "mtlOcean.fx"; }

BMAP *__smtl_ocean__wave;

void smtl_ocean_add(ENTITY *object, BMAP *reflection);

/* -- Velvety -- */
MATERIAL *smtl_velvet = { flags = TANGENT; effect = "mtlVelvety.fx"; }

void smtl_velvety_add(ENTITY *object);

#include "render_surface.c"
#endif /* render_surface.h */
