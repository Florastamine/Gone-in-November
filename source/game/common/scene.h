/* scene.h */
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
 * <scene>
 * Contains functionalities for adding, removing and calling scenes.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    __SCENE_H__
#define    __SCENE_H__

#define    __static

#include "scene_list.h"

ChapterList *ChapterList_singleton = NULL;
int          __scene_counter__     = 0;

void scene_new();
void scene_free();

ChapterData *scene_data_new(const char *name, const char *internal_name,
                            const STRING *thumbnail_name, const COLOR *sun_color,
						    const COLOR *fog_color, const VECTOR *fog_range,
						    const float sun_light);

void scene_data_free(ChapterData *data);

void scene_add(ChapterData *data);
void scene_remove(ChapterData *data);
void scene_load(ChapterData *data, const void *loader);
void scene_load(ChapterData *data);

#include "scene.c"
#endif /* scene.h */
