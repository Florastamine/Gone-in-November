/* inventory.h */
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
 * TODO: - Custom function pointers for managing items in the inventory.
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    __INVENTORY_H__
#define    __INVENTORY_H__

#define    __static

#define    INV_SIZE_X            254
#define    INV_SIZE_Y            290
#define    INV_ITEM_SIZE         32
#define    INV_ITEMS_OFFSET_X    20
#define    INV_ITEMS_OFFSET_Y    30

#define    BAG_SIZE_X                  254
#define    BAG_SIZE_Y                  290
#define    BAG_ITEM_GAP_LENGTH         4
#define    ITEM_MAX_PAD_VARS           8
#define    BAG_ITEM_MAX_NAME_LENGTH    128
#define    BAG_ITEM_MAX_DESC_LENGTH    256

#define    ITEM_PAD(item, num)    *(item->pad + (num - 1))

/*
 * BagItem (struct)
 * Contains everything about an inventory item.
 */
typedef struct BagItem
{
	struct BagItem *next;
	struct BagItem *prev;

	char *name;              /* Name of the item.                                                          */
	char *description;       /* Description.                                                               */

	int ID;                  /* Item ID. When iterating through the inventory, you should compare its
	                            ID (which is way faster).                                                  */
	int type;
	float value;             /* Value of the item.                                                         */
	float *pad;              /* Additional padding variables which can be used to store additional info.   */

	PANEL *container;        /* Image container for the bag item.                                          */

	void *bag;
} BagItem;

typedef struct Bag
{
	BagItem *head;
	BagItem *tail;
	BagItem *iterator;

	PANEL *container;

	int count;
	int max_items;

	TEXT *text;
} Bag;

Bag *bag_new(const char *background_file, const char *name, int layer, const int max_items, const float w, const float h);
Bag *bag_new(const char *background_file, const char *name, int layer, const int max_items);
Bag *bag_new(const char *background_file, const char *name, int layer);
void bag_free(Bag *bag);

void bag_add_item(Bag *bag, BagItem *item);
void bag_remove_item(Bag *bag, BagItem *item);

BagItem *item_new(Bag *bag, const char *name, const char *description, const char *image, int ID, int type, float value);
void item_free(BagItem *item);

void bag_render(Bag *bag);
void bag_render_items(Bag *bag);
void bag_hide(Bag *bag);
void bag_hide_items(Bag *bag);
int bag_is_shown(Bag *bag);

void bag_center(Bag *bag);

void bag_set_pos(Bag *bag, float x, float y);
VECTOR *bag_get_pos(Bag *bag);

int bag_get_count(Bag *bag);

void bag_set_color(Bag *bag, COLOR *color);
void bag_set_color(Bag *bag);

void bag_set_alpha(Bag *bag, float alpha);
float bag_get_alpha(Bag *bag);

#include "inventory.c"
#endif /* inventory.h */
