/* path.c */
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
 * <path>
 * Contains common paths for which game assets and code are searched.
 * In the actual game build (with PERFORM_PATH_LOOKUP disabled),
 * they are expected to be in the root folder (or in resource packages)
 * instead.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 *
 * I'm not going to bash macros all over the place like I did with <serializer>.
 */
STRING *game_asset_get_object(const STRING *asset)
{
 	STRING *ret = "";

 	#ifndef    PERFORM_PATH_LOOKUP
 	    return str_cpy(ret, asset);
 	#else
 	    str_cpy(ret, __PATH_LOOKUP_OBJECT);
 	    str_cat(ret, asset);
 	    return ret;
 	#endif
}

STRING *game_asset_get_scene(const STRING *asset)
{
	STRING *ret = "";

	#ifndef    PERFORM_PATH_LOOKUP
	    return str_cpy(ret, asset);
	#else
	    str_cpy(ret, __PATH_LOOKUP_SCENE);
	    str_cat(ret, asset);
	    return ret;
	#endif
}

STRING *game_asset_get_stream(const STRING *asset) // Streams can't be packed into resources,
                                                   // so an absolute path must always be specified.
{
	return str_create(asset);
}

STRING *game_asset_get_sound(const STRING *asset)
{
	STRING *ret = "";

	#ifndef    PERFORM_PATH_LOOKUP
	    return str_cpy(ret, asset);
	#else
	    str_cpy(ret, __PATH_LOOKUP_SOUND);
	    str_cat(ret, asset);
	    return ret;
	#endif
}

STRING *game_asset_get_gui(const STRING *asset)
{
	STRING *ret = "";

	#ifndef    PERFORM_PATH_LOOKUP
	    return str_cpy(ret, asset);
	#else
	    str_cpy(ret, __PATH_LOOKUP_GUI_2D);
	    str_cat(ret, asset);
	    return ret;
	#endif
}

STRING *game_asset_get_2d_sprite(const STRING *asset)
{
	STRING *ret = "";

	#ifndef    PERFORM_PATH_LOOKUP
	    return str_cpy(ret, asset);
	#else
	    str_cpy(ret, __PATH_LOOKUP_SPRITE_2D);
	    str_cat(ret, asset);
	    return ret;
	#endif
}

STRING *game_asset_get_2d_generic(const STRING *asset)
{
	STRING *ret = "";

	#ifndef    PERFORM_PATH_LOOKUP
	    return str_cpy(ret, asset);
	#else
	    str_cpy(ret, __PATH_LOOKUP_GENERIC_2D);
	    str_cat(ret, asset);
	    return ret;
	#endif
}

STRING *game_asset_get_2d_fx(const STRING *asset)
{
	STRING *ret = "";

	#ifndef    PERFORM_PATH_LOOKUP
	    return str_cpy(ret, asset);
	#else
	    str_cpy(ret, __PATH_LOOKUP_FX_2D);
	    str_cat(ret, asset);
	    return ret;
	#endif
}

STRING *game_asset_get_save(const STRING *asset)
{
    STRING *ret = "";

    #ifndef    PERFORM_PATH_LOOKUP
        return str_cpy(ret, asset);
    #else
        str_cpy(ret, __PATH_LOOKUP_SAVE);
        str_cat(ret, asset);
        return ret;
    #endif
}
