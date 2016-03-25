/* render_surface.c */
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
 *
 * Shader definitions are preceded by smtl_* rather than mtl_* in order to
 * avoid conflicts with Gamestudio default materials.
 * __________________________________________________________________
 */

/*
 * void smtl_fur_add(ENTITY *object)
 *
 * Applies fur to an object.
 */
void smtl_fur_add(ENTITY *object)
{
	if( !__smtl_fur__noise )
	{
		__smtl_fur__noise = bmap_create(game_asset_get_2d_fx("noise.tga"));
		smtl_fur->skin1 = __smtl_fur__noise;
	}
	if( !__smtl_fur__color)
	{
		__smtl_fur__color = bmap_create(game_asset_get_2d_fx("noise.tga"));
		smtl_fur->skin2 = __smtl_fur__color;
	}

	if(object) object->material = smtl_fur;
}

/*
 * void smtl_grass_add(ENTITY *object)
 *
 * Applies waving effect to an object (suitable for grass entities.).
 */
void smtl_grass_add(ENTITY *object)
{
	if(object) object->material = smtl_grass;
}

/*
 * void smtl_water_add(ENTITY *object, BMAP *reflection)
 *
 * Applies water shader to an object. Note that, the reflection map
 * is stored only once. Pass the reflection map to the first object using this shader,
 * then for the remaining objects, pass NULL to the reflection map in order for these
 * objects to use the same map. (passing something other than NULL causes the old map to be
 * destroyed and assigned a new one).
 */
void smtl_water_add(ENTITY *object, BMAP *reflection)
{
	if( !reflection ) return;

	if( !__smtl_water__bump )
	{
		__smtl_water__bump = bmap_create(game_asset_get_2d_fx("bumpwaves.tga"));
		smtl_water->skin1 = __smtl_water__bump;
	}

	if( reflection && !smtl_water->skin2 )
	    smtl_water->skin2 = reflection;

	if(object)
	    object->material = smtl_water;
}

/*
 * void smtl_chrome_add(ENTITY *object, VECTOR *normalized_color)
 *
 * Applies chrome-looking to an object.
 * Normalized colors are required (that is - in the closed interval of [0.0 ; 1.0]).
 * Raw colors values are automatically normalized.
 */
void smtl_chrome_add(ENTITY *object, VECTOR *normalized_color)
{
	if( normalized_color->x > 1.0 ) normalized_color->x /= 255.0;
	if( normalized_color->y > 1.0 ) normalized_color->y /= 255.0;
	if( normalized_color->z > 1.0 ) normalized_color->z /= 255.0;

	if( !__smtl_chrome__lookup )
	{
		__smtl_chrome__lookup = bmap_create(game_asset_get_2d_fx("chrome_lookup.tga"));
		smtl_chrome->skin1 = __smtl_chrome__lookup;
	}

	smtl_chrome->skill1 = floatv(1.0);

	if(object)
	{
		object->material = smtl_chrome;
		object->skill41 = floatv(normalized_color->x);
		object->skill42 = floatv(normalized_color->y);
		object->skill43 = floatv(normalized_color->z);
	}
}

/*
 * void smtl_blur_add(ENTITY *object)
 *
 * Applies a fixed blur shader to the object.
 */
void smtl_blur_add(ENTITY *object)
{
	if(object) object->material = smtl_gblur;
}

/*
 * void smtl_ocean_add(ENTITY *object, BMAP *reflection)
 *
 * Applies the ocean shader to the object. Note that, the reflection map
 * is stored only once. Pass the reflection map to the first object using this shader,
 * then for the remaining objects, pass NULL to the reflection map in order for these
 * objects to use the same map. (passing something other than NULL causes the old map to be
 * destroyed and assigned a new one).
 */
void smtl_ocean_add(ENTITY *object, BMAP *reflection)
{
	if( !reflection ) return;

	if( !__smtl_ocean__wave )
	{
		__smtl_ocean__wave = bmap_create(game_asset_get_2d_fx("waves2.tga"));
		smtl_ocean->skin1 = __smtl_ocean__wave;
	}

	if(object)
	{
		smtl_ocean->skin2 = reflection;
		bmap_to_cubemap(smtl_ocean->skin2);

		object->material = smtl_ocean;
	}
}

/*
 * void smtl_velvety_add(ENTITY *object)
 *
 * Applies velvety to the object.
 */
void smtl_velvety_add(ENTITY *object)
{
	if(object) object->material = smtl_velvet;
}
