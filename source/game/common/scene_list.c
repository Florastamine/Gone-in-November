/* scene_list.c */
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
 *
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
ChapterList *ChapterListCreate()
{
	ChapterList *this = sys_malloc(sizeof(ChapterList));

	this->firstnode = NULL;
	this->lastnode = NULL;
	this->iterator = NULL;
	this->next_iterator = NULL;
	this->length = 0;

	return this;
}

ChapterNode *ChapterListBegin(ChapterList *list)
{
	if(list->firstnode != NULL)
	{
		list->iterator = list->firstnode;
		list->next_iterator = list->iterator->next;

		return list->iterator;
	}

	return NULL;
}

ChapterNode *ChapterListNext(ChapterList *list)
{
	if(list->next_iterator != NULL)
	{
		list->iterator = list->next_iterator;
		list->next_iterator = list->iterator->next;
		return list->iterator;
	}

	return NULL;
}

void ChapterListPush(ChapterList *list, ChapterData *data)
{
	ChapterNode *newnode = sys_malloc(sizeof(ChapterNode));
	newnode->data = data;

	if(list->firstnode != NULL)
	{
		newnode->previous = list->lastnode;
		newnode->next = NULL;

		list->lastnode->next = newnode;
		list->lastnode = newnode;
	}
	else
	{
		newnode->previous = NULL;
		newnode->next = NULL;

		list->firstnode = newnode;
		list->lastnode = newnode;
	}

	(list->length)++;
}

int ChapterListLength(ChapterList *list)
{
	return list->length;
}

int ChapterListFindIndex(ChapterList *list, ChapterNode *node)
{
	ChapterNode *link = NULL;
	int count;

	for(link = list->firstnode, count = 0; link != NULL; link = link->next, count++)
	{
		if( link->data == node->data )
			return count;
	}
	return -1;
}

ChapterNode *ChapterListFindNode(ChapterList *list, ChapterNode *node)
{
	ChapterNode *link = NULL;
	int count;

	for(link = list->firstnode, count = 0; link != NULL; link = link->next, count++)
	{
		if( link->data == node->data )
			return link;
	}

	return NULL;
}

void ChapterListRemoveFront(ChapterList *list)
{
	if( list->firstnode != NULL )
	{
		if( list->firstnode == list->lastnode )
		{
			sys_free(list->firstnode);
			list->firstnode = NULL;
			list->lastnode = NULL;
		}
		else
		{
			ChapterNode *tmpDel = list->firstnode;
			list->firstnode->next->previous = NULL;
			list->firstnode = list->firstnode->next;

			sys_free(tmpDel);
			tmpDel = NULL;
		}

		(list->length)--;
	}
}

void ChapterListRemoveBack(ChapterList *list)
{
	if( list->firstnode != NULL )
	{
		if( list->firstnode == list->lastnode )
		{
			sys_free(list->firstnode);
			list->firstnode = NULL;
			list->lastnode = NULL;
		}
		else
		{
			ChapterNode *tmpDel = list->lastnode;
			list->lastnode->previous->next = NULL;
			list->lastnode = list->lastnode->previous;

			sys_free(tmpDel);
			tmpDel = NULL;
		}

		(list->length)--;
	}
}

void ChapterListRemoveNode(ChapterList *list, ChapterNode *_node)
{
	if( list->firstnode != NULL )
	{
		if( _node == list->firstnode )
			ChapterListRemoveFront(list);
		else if( _node == list->lastnode )
			ChapterListRemoveBack(list);
		else
		{
			_node->previous->next = _node->next;
			_node->next->previous = _node->previous;

			sys_free(_node);
			(list->length)--;
		}
	}
}

void ChapterListEnumerate(ChapterList *list, const void *funcptr)
{
	if(!list || !funcptr)
		return;

	const void f(ChapterNode *node, int *flag);
	int flag = 0;
	ChapterNode *link = NULL;

	f = funcptr;

	for(link = list->firstnode; link != NULL; link = link->next)
	{
		f(link, &flag);

		if(flag)
			break;
	}
}

void ChapterListFree(ChapterList *list)
{
	ChapterNode *tmp = NULL;

	if(list->firstnode != NULL)
	{
		list->iterator = list->firstnode;
		list->firstnode = NULL;
		while( list->iterator != NULL )
		{
			tmp = list->iterator->next;

			ChapterDataFree(list->iterator->data);
			sys_free(list->iterator);
			list->iterator = tmp;
		}
	}

	sys_free(list);
}

ChapterData *ChapterDataCreate(const char *name, const char *internal_name,
                               const STRING *thumbnail_name, const COLOR *vsun_color,
						       const COLOR *vfog_color, const VECTOR *vfog_range,
						       const float vsun_light)
{
	ASSERT(internal_name, _chr("Fatal error in ChapterDataCreate(): A null char * pointer was given to the function, cannot create the data block."));
	ASSERT(strlen(internal_name) < CHAPTER_INAME_MAX_LENGTH, _chr("Fatal error in ChapterDataCreate(): Chapter name's length exceeds CHAPTER_INAME_MAX_LENGTH."));

	ChapterData *data     = (ChapterData *) sys_malloc(sizeof(ChapterData));
	data->chapter_name    = (char *)        sys_malloc(CHAPTER_NAME_MAX_LENGTH * sizeof(char));
	data->chapter_iname   = (char *)        sys_malloc(CHAPTER_INAME_MAX_LENGTH * sizeof(char));
	data->pad             = (float *)       sys_malloc(CHAPTER_PAD_VARS_MAX_LENGTH * sizeof(float));
	data->sun_light       = -1.0;

	// The chapter name can be left blank, whilist the chapter's internal name (its scene file)
	// must not be blank - each ChapterData struct has to be linked to a scene in some way.
	if(name)
		if(strlen(name) <= CHAPTER_NAME_MAX_LENGTH)
			strcpy(data->chapter_name, name);

	strcpy(data->chapter_iname, internal_name);

	// Continue to allocate and set up any additional parameters.
	if(thumbnail_name)
		if(file_exists(thumbnail_name))
			data->thumbnail = bmap_create(thumbnail_name);

	if(vsun_color)
	{
		data->sun_color     = (COLOR *) sys_malloc(sizeof(COLOR));
		vec_set(data->sun_color, vsun_color);
	}

	if(vfog_color)
	{
		data->fog_color     = (COLOR *) sys_malloc(sizeof(COLOR));
		vec_set(data->fog_color, vfog_color);

		if(vfog_range)
		{
			data->fog_range = (VECTOR *) sys_malloc(sizeof(VECTOR));
			vec_set(data->fog_range, vfog_range);
		}
	}

	if(vsun_light >= 0.0)
		data->sun_light = vsun_light;

	return data;
}

void ChapterDataFree(ChapterData *data)
{
	sys_free(data->chapter_name);
	sys_free(data->chapter_iname);
	sys_free(data->pad);

	if(data->thumbnail)
		bmap_remove(data->thumbnail);

	if(data->fog_color)
	{
		if(data->fog_range)
			sys_free(data->fog_range);

		sys_free(data->fog_color);
	}

	if(data->sun_color)
		sys_free(data->sun_color);

	sys_free(data);
}
