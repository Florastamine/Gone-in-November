/* scene.c */
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
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
 ChapterData *scene_data_new(const char *name, const char *internal_name,
                             const STRING *thumbnail_name, const COLOR *sun_color,
 						    const COLOR *fog_color, const VECTOR *fog_range,
 						    const float sun_light)
{
    return ChapterDataCreate(name, internal_name, thumbnail_name, sun_color, fog_color, fog_range, sun_light);
}

void scene_data_free(ChapterData *data)
{
    if(data)
    {
        ChapterDataFree(data);

        __scene_counter__ -= 1;
        game_log_write(str_printf(NULL, "Removed a scene from the list. Total scenes: %i", __scene_counter__));
    }
}

void scene_new()
{
    game_log_write("Request to create a new scene list...");

    if(!ChapterList_singleton)
    {
        ChapterList_singleton = ChapterListCreate();
        game_log_write("Allocated and created a brand new scene list.");
    }
}

void scene_free()
{
    game_log_write("Request to free the scene list...");

    if(ChapterList_singleton)
    {
        ChapterListFree(ChapterList_singleton);
        game_log_write("Scene list freed. Subsequent calls to scene_add() will be invalid.");
    }
}

void scene_add(ChapterData *data)
{
    if(ChapterList_singleton && data)
    {
        ChapterListPush(ChapterList_singleton, data);

        __scene_counter__ += 1;
        game_log_write(str_printf(NULL, "Pusheen a scene into the list. Total scenes: %i", __scene_counter__));
    }
}

void scene_remove(ChapterData *data)
{
    if(ChapterList_singleton && data)
        ChapterListRemoveNode(ChapterList_singleton, data);
}

void scene_load(ChapterData *data)
{
    if(ChapterList_singleton && data)
    {
        game_log_write(str_printf(NULL, "Request to load a game scene <name = %s; internal_name = %s>", data->chapter_name, data->chapter_iname));

        ChapterNode *link = NULL;

        for(link = ChapterList_singleton->firstnode; link != NULL; link = link->next)
            if(link->data == data)
                break;

        if(link == NULL)
        {
            game_log_write(str_printf(NULL, "Request to load the scene data block (internal_name = %s) failed. Reason: End of linked list. (tried pushing to the list through scene_add()?)", link->data->chapter_iname));
            return;
        }

        // Begin loading the level.
        if(file_exists(link->data->chapter_iname))
            game_scene_load(link->data->chapter_iname);
        else
        {
            game_log_write(str_printf(NULL, "Request to load the scene data block (internal_name = %s) failed. Reason: The scene file doesn't exist.", link->data->chapter_iname));
            return;
        }

        // And load associated data.
        if(link->data->sun_color)
            vec_set(sun_color, link->data->sun_color);

        if(link->data->sun_light != -1.0)
            sun_light = link->data->sun_light;

        if(link->data->fog_color)
            vec_set(&d3d_fogcolor1, link->data->fog_color);

        if(link->data->fog_range)
        {
            camera->fog_start = link->data->fog_range->x;
            camera->fog_end = link->data->fog_range->y;
        }
    }

    // Done.
}
