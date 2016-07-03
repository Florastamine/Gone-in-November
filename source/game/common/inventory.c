/* inventory.c */
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
 * <inventory>
 *
 * A plain simple and rewritten inventory system from the TUST project
 * (https://github.com/MasterQ32/TUST), which fixed, modified and added features.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
Bag *bag_new(const char *background_file, const char *name, int layer, const int max_items, const float w, const float h)
{
	w = abs(w);
	h = abs(h);

	Bag* bag = (Bag *) sys_malloc(sizeof(Bag));
	bag->count = 0;
	bag->max_items = (int) ifelse(max_items > 0, max_items, 36);

	int nInvItemsX = 0, nInvItemsY = 0;
	int items_line = (int) sqrt(bag->max_items);
	layer = (int) ifelse(layer > 0, layer, 1);

	// Create background panel
	bag->container = pan_create(NULL, layer);

	if(background_file)
		bag->container->bmap = bmap_createpart(background_file, 0, 0, w, h);
	else
	{
		bag->container->bmap = bmap_createblack(w, h, 16);

		// Draw item boxes
		int vFormat = bmap_lock(bag->container->bmap, 0);
		int vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
		int i, j, k;

		// For each box...
		for(i=0; i < items_line; i++)
		{
			for(j=0; j < items_line; j++)
			{
				// Draw the borders
				for(k=0; k<INV_ITEM_SIZE+2; k++)
				{
					// Top and left border
					pixel_to_bmap(bag->container->bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+BAG_ITEM_GAP_LENGTH))+k, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+BAG_ITEM_GAP_LENGTH)), vPixel);
					pixel_to_bmap(bag->container->bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+BAG_ITEM_GAP_LENGTH)), INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+BAG_ITEM_GAP_LENGTH))+k, vPixel);

					// Right and bottom border
					pixel_to_bmap(bag->container->bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+BAG_ITEM_GAP_LENGTH))+INV_ITEM_SIZE+1, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+BAG_ITEM_GAP_LENGTH))+k, vPixel);
					pixel_to_bmap(bag->container->bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+BAG_ITEM_GAP_LENGTH))+k, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+BAG_ITEM_GAP_LENGTH))+INV_ITEM_SIZE+1, vPixel);
				}
			}
		}

	}

	// Create the title bar.
	bag->text = txt_create(1, layer + 1);
	bag->text->flags |= (OUTLINE | CENTER_X | WWRAP);
	bag->text->size_x = bmap_width(bag->container->bmap);
	bag->text->font = font_create("Arial#15b");

	if(name)
		str_cpy((bag->text->pstring)[0], _str(name));
	else
		str_cpy((bag->text->pstring)[0], "Inventory");

	return bag;
}

Bag *bag_new(const char *background_file, const char *name, int layer, const int max_items)
{
	return bag_new(background_file, name, layer, max_items, BAG_SIZE_X, BAG_SIZE_Y);
}

Bag *bag_new(const char *background_file, const char *name, int layer)
{
	return bag_new(background_file, name, layer, 0, BAG_SIZE_X, BAG_SIZE_Y);
}

void bag_free(Bag *bag)
{
	if(bag)
	{
		safe_remove(bag->container);
		safe_remove(bag->text);

		sys_free(bag);
	}
}

int bag_is_shown(Bag *bag)
{
	if(!bag)
		return false;

	return (int) ifelse(bag->container->flags & SHOW, true, false);
}

#define    __SHOW    0
#define    __HIDE    1
__static void __bag_process(Bag *bag, const int mode)
{
	if(bag)
	{
		if(mode == __SHOW)
		{
			bag->container->flags |= (SHOW);
			bag->text->flags      |= (SHOW);
		}
		else
		{
			bag->container->flags &= ~(SHOW);
			bag->text->flags      &= ~(SHOW);
		}

		bag->iterator = bag->head;
		int i = 0;
		while(bag->iterator != NULL)
		{
			if(bag->iterator->container != NULL)
			{
				if(mode == __SHOW)
				{
					int j = (int) sqrt(bag->max_items);
					bag->iterator->container->flags |= (SHOW);

					bag->iterator->container->pos_x = bag->container->pos_x + 1 + INV_ITEMS_OFFSET_X + (BAG_ITEM_GAP_LENGTH * (i % j)) + (INV_ITEM_SIZE*(i % j));
					bag->iterator->container->pos_y = bag->container->pos_y + 1 + INV_ITEMS_OFFSET_Y + (BAG_ITEM_GAP_LENGTH * (integer(i/j))) + (INV_ITEM_SIZE*(integer(i/j)));
				}
				else
					bag->iterator->container->flags &= ~(SHOW);
			}

			i++;
			bag->iterator = bag->iterator->next;
		}
	}
}

void bag_render(Bag *bag)
{
	__bag_process(bag, __SHOW);
}

void bag_hide(Bag *bag)
{
	__bag_process(bag, __HIDE);
}

void bag_center(Bag *bag)
{
	if(bag)
	{
		bag_set_pos(bag,
		            (screen_size.x - bmap_width(bag->container->bmap)) / 2,
				    (screen_size.y - bmap_height(bag->container->bmap)) / 2);
	}
}

void bag_set_pos(Bag *bag, float x, float y)
{
	if(bag)
	{
		bag->container->pos_x = x;
		bag->container->pos_y = y;
		bag->text->pos_x      = x + bmap_width(bag->container->bmap) / 2 + 5.0;
		bag->text->pos_y      = y + 5.0;
	}
}

VECTOR *bag_get_pos(Bag *bag)
{
	VECTOR *v = NULL;

	if(bag)
		v = vector(bag->container->pos_x, bag->container->pos_y, 0.0);

	return v;
}

void bag_add_item(Bag *bag, BagItem *item)
{
	if(bag && item)
	{
		if(bag->count < bag->max_items)
		{
			if(!(bag->tail))
			{
				bag->head = bag->tail = item;
				bag->head->next = NULL;
				bag->head->prev = NULL;

				layer_sort(item->container, bag->container->layer + 1);
			}
			else
			{
				item->prev = bag->tail;
				item->next = NULL;
				bag->tail->next = item;
				bag->tail = item;

				layer_sort(item->container, bag->container->layer + 1);
			}
		}

		item->bag = bag;
		bag->count += 1;
	}
}

void item_free(BagItem *item)
{
	if(item)
	{
		sys_free(item->name);
		sys_free(item->description);
		safe_remove(item->container);

		sys_free(item);
	}
}

void bag_remove_item(Bag *bag, BagItem *item)
{
	if(bag && item)
	{
		BagItem *titem = NULL;
		bag->iterator = bag->head;

		while(bag->iterator != NULL)
		{
			if(bag->iterator == item)
			{
				titem = bag->iterator;
				if(titem == bag->head)
					bag->head = bag->head->next;
				else if(titem == bag->tail)
					bag->tail = bag->tail->prev;

				if(!(titem->prev))
					titem->prev->next = titem->next;

				if(!(titem->next))
					titem->next->prev = titem->prev;

				if(!(bag->head))
					bag->tail = NULL;

				bag->count -= 1;
				break;
			}
			bag->iterator = bag->iterator->next;
		}
	}
}

__static void __bag_item_click__(fixed num, PANEL *p)
{
	if(p)
	{
		if(p->skill_x)
		{
			BagItem *item = (BagItem *) p->skill_x;
			if(item)
				printf("%s", item->name);
		}
	}
}

__static void __bag_item_enter__(fixed num, PANEL *p)
{
	if(p)
	{
		if(p->skill_x)
		{
			BagItem *item = (BagItem *) p->skill_x;
			if(item)
				wait(1.0);
		}
	}
}

__static void __bag_item_leave__(fixed num, PANEL *p)
{
	if(p)
	{
		if(p->skill_x)
		{
			BagItem *item = (BagItem *) p->skill_x;
			if(item)
				wait(1.0);
		}
	}
}

BagItem *item_new(Bag *bag, const char *name, const char *description, const char *image, int ID, int type, float value)
{
	int layer = (int) ifelse(bag, bag->container->layer, 1);

	BagItem *item        = (BagItem *) sys_malloc(sizeof(BagItem));
	item->name           = (char *) sys_malloc(BAG_ITEM_MAX_NAME_LENGTH * sizeof(char));
	item->description    = (char *) sys_malloc(BAG_ITEM_MAX_DESC_LENGTH * sizeof(char));
	item->pad            = (float *) sys_malloc(ITEM_MAX_PAD_VARS * sizeof(float));

	if(name)
		if(strlen(name) <= BAG_ITEM_MAX_NAME_LENGTH)
			strcpy(item->name, name);

	if(description)
		if(strlen(description) <= BAG_ITEM_MAX_DESC_LENGTH)
			strcpy(item->description, description);

	item->ID = ID;
	item->value = value;
	item->type = type;
	item->container = pan_create(NULL, layer + 1);
	item->container->size_x = INV_ITEM_SIZE;
	item->container->size_y = INV_ITEM_SIZE;

	if(!image)
	{
		item->container->bmap = bmap_createblack(INV_ITEM_SIZE, INV_ITEM_SIZE, 16);
		bmap_fill( item->container->bmap, vector(random(255.0), random(255.0), random(255.0)), 100.0 );
	}
	else
		item->container->bmap = bmap_create(image);

	item->container->skill_x = item;

	pan_setbutton(item->container, 0, 0, 0, 0, item->container->bmap, item->container->bmap, item->container->bmap, item->container->bmap, __bag_item_click__, __bag_item_leave__, __bag_item_enter__);

	return item;
}

__static void __bag_process_items__(Bag *bag, const int flag)
{
	if(bag)
	{
		bag->iterator = bag->head;
		while(bag->iterator != NULL)
		{
			if(bag->iterator->container)
			{
				if(flag == __HIDE)
					if(bag->iterator->container->flags & SHOW)
						bag->iterator->container->flags &= ~(SHOW);
				else
					if((bag->iterator->container->flags & SHOW))
						bag->iterator->container->flags |= (SHOW);
			}

			bag->iterator = bag->iterator->next;
		}
	}
}

void bag_render_items(Bag *bag)
{
	__bag_process_items__(bag, __SHOW);
}

void bag_hide_items(Bag *bag)
{
	__bag_process_items__(bag, __HIDE);
}

int bag_get_count(Bag *bag)
{
	return (int) ifelse(bag != NULL, bag->count, -1);
}

void bag_set_color(Bag *bag, COLOR *color)
{
	if(bag)
		if(color)
			bmap_fill(bag->container->bmap, color, 100.0);
		else
			bmap_fill(bag->container->bmap, nullvector, 100.0);
}

void bag_set_color(Bag *bag)
{
	bag_set_color(bag, NULL);
}

void bag_set_alpha(Bag *bag, float alpha)
{
	if(bag)
	{
		if(alpha < 100.0)
		{
			if(!(bag->container->flags & TRANSLUCENT))
				bag->container->flags |= (TRANSLUCENT);

			bag->container->alpha = alpha;
		}
		else // Totally opaque, then do not render its translucency.
			if(bag->container->flags & TRANSLUCENT)
				bag->container->flags &= ~(TRANSLUCENT);
	}
}

float bag_get_alpha(Bag *bag)
{
	return (float) ifelse(bag != NULL, bag->container->alpha, -1.0);
}
