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
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 * 
 * I'm not going to bash macros all over the place like I did with <serializer>.
 */
STRING *game_get_asset_object(const STRING *asset)
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

STRING *game_get_asset_scene(const STRING *asset)
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

STRING *game_get_asset_stream(const STRING *asset) // Streams can't be packed into resources,
                                                   // so an absolute path must always be specified.
{
	return str_create(asset);
}

STRING *game_get_asset_sound(const STRING *asset)
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

STRING *game_get_asset_translation_text(const STRING *asset) // I have a derp feeling that this function is going to be deprecated very soon.
{
	return region_get_string(asset);
}

STRING *game_get_asset_gui(const STRING *asset)
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

STRING *game_get_asset_sprite(const STRING *asset)
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

STRING *game_get_asset_2d_generic(const STRING *asset)
{
	STRING *ret = "";
	
	#ifndef    PERFORM_PATH_LOOKIP
	    return str_cpy(ret, asset);
	#else
	    str_cpy(ret, __PATH_LOOKUP_GENERIC_2D);
	    str_cat(ret, asset);
	    return ret;
	#endif
}
