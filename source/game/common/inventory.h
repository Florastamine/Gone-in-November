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
 *       - Custom inventory size, background image, and coloring.
 *       - Padding variables for additional data (item type, belong to whom, etc. ect.)
 *       - Get count of bag.
 *
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    __INVENTORY_H__
#define    __INVENTORY_H__

#define    __static

#define    INV_SIZE_X            254
#define    INV_SIZE_Y            290
#define    INV_ITEM_SIZE         32
#define    INV_ITEMS_X           6
#define    INV_ITEMS_Y           6
#define    INV_ITEMS_OFFSET_X    20
#define    INV_ITEMS_OFFSET_Y    30
#define    INV_ITEM_GAP          4 // Gap between item slots

#define    BAG_ITEM_MAX_NAME_LENGTH    128
#define    BAG_ITEM_MAX_DESC_LENGTH    256

#define    BAG_PAD(bag, num)    *(bag->pad + (num - 1))

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

	TEXT *text;
} Bag;

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

#include "inventory.c"
#endif /* inventory.h */
