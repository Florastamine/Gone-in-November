/* bmap_ex.c */
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
 * <bmap_ex>
 * Contains extra bmap_() functions which performs various transformations/filters
 * to images, that are currently missing from the Gamestudio API.
 * Based on CImg library 1.5.2.
 *
 * Taken and modified from the TUST project. (https://github.com/MasterQ32/TUST)
 * DLL source code can be found under ./source/cpp/bmap.
 * __________________________________________________________________
 */
void bmap_lerp ( BMAP *destiny, BMAP *source1, BMAP *source2, var factor )
{
	if ( !__bmapmatLerp__ )
	{
		__bmapmatLerp__ = mtl_create ();
		effect_load ( __bmapmatLerp__, "bmap_lerp.fx" );
	}
	__bmapmatLerp__->skill1 = floatv(factor);
	__bmapmatLerp__->skin1 = source2;
	bmap_process ( destiny, source1, __bmapmatLerp__ );
}

void bmap_rotate ( BMAP *destiny, BMAP *source, var source_offset_x, var source_offset_y, var destiny_offset_x, var destiny_offset_y, var angle )
{
	if ( !__bmapmatRotate__ )
	{
		__bmapmatRotate__ = mtl_create ();
		effect_load ( __bmapmatRotate__, "bmap_rotate.fx" );
	}
	__bmapmatRotate__->skill1 = floatv(cosv(angle));
	__bmapmatRotate__->skill2 = floatv(-sinv(angle));
	__bmapmatRotate__->skill3 = floatv(sinv(angle));
	__bmapmatRotate__->skill4 = floatv(cosv(angle));
	__bmapmatRotate__->skill5 = floatd(source_offset_x, bmap_width(source));
	__bmapmatRotate__->skill6 = floatd(source_offset_y, bmap_height(source));
	__bmapmatRotate__->skill7 = floatd(destiny_offset_x, bmap_width(destiny));
	__bmapmatRotate__->skill8 = floatd(destiny_offset_y, bmap_height(destiny));
	__bmapmatRotate__->skill9 = floatd(bmap_width(destiny), bmap_width(source));
	__bmapmatRotate__->skill10 = floatd(bmap_height(destiny), bmap_height(source));
	__bmapmatRotate__->skill11 = floatd(bmap_width(source), bmap_height(source));
	bmap_process ( destiny, source, __bmapmatRotate__ );
}
