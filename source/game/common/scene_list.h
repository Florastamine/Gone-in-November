/* scene_list.h */
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
 * <scene_list>
 * A low-level interface which acts as the base code for <scene>.
 * You should use the high-level interface instead, which allows you
 * to easily create and manage the level list yourself without extra code for
 * managing/navigating/destroying the doubly linked list.
 * It's not commented much.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    __SCENE_LIST_H__
#define    __SCENE_LIST_H__

/*
 * Maximum length allowed for the chapter name.
 */
#define    CHAPTER_NAME_MAX_LENGTH         256

/*
 * Maximum length allowed for the chapter's internal name
 * (which is actually the relative path to the scene file).
 */
#define    CHAPTER_INAME_MAX_LENGTH        128

/*
 * Maximum amount of additional data to be appended along with
 * the level. You can then use this data (qualified with *(elem->pad + )) to carry
 * additional associated information (PSSM information, for example.)
 */
#define    CHAPTER_PAD_VARS_MAX_LENGTH     16

#define    CHAPTER_PAD(ChapterDataBlock, ID)    *(ChapterDataBlock->pad + (ID - 1))

/*
 * ChapterData (struct)
 * This struct encapsulates itself with the scene associated data.
 */
typedef struct ChapterData
{
	char      *chapter_name;         /* Name of the chapter.                          */
	char      *chapter_iname;        /* Internal name of the chapter (mandatory).     */

	BMAP      *thumbnail;            /* Chapter's thumbnail image.                    */
	COLOR     *sun_color;
	COLOR     *fog_color;
	VECTOR    *fog_range;

	float      sun_light;
	float     *pad;
} ChapterData;

/*
 * ChapterNode (struct)
 * Contains a ChapterData struct and acts as a node in the doubly linked list.
 */
typedef struct ChapterNode
{
	ChapterData              *data;
	struct ChapterNode       *previous;
	struct ChapterNode       *next;
} ChapterNode;

/*
 * ChapterList (struct)
 * Contains the structure of a doubly linked list.
 *
 * When using facilities provided in <scene>, the linked list is transparent
 * and thus you'll only have to create and destroy the scene data, the rest
 * is automagically taken care of.
 */
typedef struct ChapterList
{
	ChapterNode *firstnode;
	ChapterNode *lastnode;
	ChapterNode *iterator;
	ChapterNode *next_iterator;

	int length;
} ChapterList;

ChapterData *ChapterDataCreate(const char *name, const char *internal_name,
                               const STRING *thumbnail_name, const COLOR *sun_color,
						       const COLOR *fog_color, const VECTOR *fog_range,
						       const float sun_light);

void ChapterDataFree(ChapterData *data);

ChapterList *ChapterListCreate();
void ChapterListFree(ChapterList *list);

void ChapterListRemoveFront(ChapterList *list);
void ChapterListRemoveBack(ChapterList *list);
void ChapterListRemoveNode(ChapterList *list, ChapterNode *node);

ChapterNode *ChapterListBegin(ChapterList *list);
ChapterNode *ChapterListNext(ChapterList *list);

void ChapterListPush(ChapterList *list, ChapterData *data);

int ChapterListFindIndex(ChapterList *list, ChapterNode *node);
ChapterNode *ChapterListFindNode(ChapterList *list, ChapterNode *node);

int ChapterListLength(ChapterList *list);
void ChapterListEnumerate(ChapterList *list, const void *funcptr);

#include "scene_list.c"
#endif /* scene_list.h */
