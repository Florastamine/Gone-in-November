/* gui_utilities.c */
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
 */

/*
 * Pair *gui_panel_get_size( Panel *panel )
 * 
 * Returns the size of a given panel.
 */
Pair *gui_panel_get_size( Panel *panel )
{
	Pair *p = pair_new();
	
	if(panel)
	{
		p->first = panel->size_x;
		p->second = panel->size_y;
	}
	
	return p;
}

/*
 * void gui_panel_set_size( Panel *panel, float sx, float sy )
 * 
 * Overrides the current panel size's components. 
 */
void gui_panel_set_size( Panel *panel, float sx, float sy )
{
	if( !panel ) return;
	
	panel->size_x = sx;
	panel->size_y = sy;
}

/*
 * Pair *gui_image_get_dimensions( const Bitmap *image )
 * 
 * Returns the size (dimensions) of a given bitmap image.
 * Arguments can either be passed as a bitmap struct or a string containing file name.
 */
Pair *gui_image_get_dimensions( const Bitmap *image )
{	
	Pair *p = pair_new();
	
	if(image)
	{
		p->first = bmap_width(image);
		p->second = bmap_height(image);
	}
	
	return p;
}

Pair *gui_image_get_dimensions( const String *image_file )
{
	return gui_image_get_dimensions(bmap_create(image_file));
}

/*
 * Pair *gui_panel_get_pos( Panel *panel )
 * 
 * Returns the current position of the panel.
 */
Pair *gui_panel_get_pos( Panel *panel )
{
	Pair *p = pair_new();
	
	if(panel)
	{
		p->first = panel->pos_x;
		p->second = panel->pos_y;
	}
	
	return p;
}

/*
 * void gui_panel_set_pos( Panel *panel, Pair *pair )
 * 
 * Sets the panel's position.
 */
void gui_panel_set_pos( Panel *panel, Pair *pair )
{
	if( !panel || !pair ) return;
	
	panel->pos_x = pair->first;
	panel->pos_y = pair->second;
}

void gui_panel_set_pos( Panel *panel, float px, float py )
{
	if(panel)
	{
		panel->pos_x = px;
		panel->pos_y = py;
	}
}

Pair *gui_text_get_pos( Text *text )
{
	Pair *p = pair_new();
		
	if(text)
	{
		p->first = text->pos_x;
		p->second = text->pos_y;
	}
	
	return p;
}

void gui_text_set_pos( Text *text, Pair *pair )
{
	if( !pair || !text ) return;
	
	text->pos_x = pair->first;
	text->pos_y = pair->second;
}

void gui_text_set_pos( Text *text, float px, float py )
{
	if(text)
	{
		text->pos_x = px;
		text->pos_y = py;
	}
}

/*
 * void gui_panel_set_align( Panel *panel, const int mode )
 * 
 * Align a panel. The panel must contain a valid bitmap.
 */
void gui_panel_set_align( Panel *panel, const int mode )
{
	if( !panel ) return;
	
	Vector3 pos;
	
	switch( mode ) {
		case    ALIGN_TOP_LEFT:
		{
			pos.x = 0;
			pos.y = 0;
			break;
		}
		
		case    ALIGN_TOP_RIGHT:
		{
			pos.x = screen_size.x - bmap_width(panel->bmap);
			pos.y = 0;
			break;
		}
		
		case    ALIGN_BOTTOM_LEFT:
		{
			pos.x = 0;
			pos.y = screen_size.y - bmap_height(panel->bmap);
			break;
		}
		
		case    ALIGN_BOTTOM_RIGHT:
		{
			pos.x = screen_size.x - bmap_width(panel->bmap);
			pos.y = screen_size.y - bmap_height(panel->bmap);
			break;
		}
		
		default:
		{
			vec_zero(pos);
		}
	}
	
	gui_panel_set_pos(panel, pos.x, pos.y);
}

/*
 * void gui_panel_set_scale( Panel *panel, const int __flags )
 * 
 * Shrinks a panel. The panel must contain a valid bitmap.
 */
void gui_panel_set_scale( Panel *panel, const int __flags )
{
	if( !panel ) return;
	
	Pair shrinkness;
	pair_set( &shrinkness, 1.0, 1.0 );
	
	if( __flags & SCALE_X )
	{
		shrinkness.first = screen_size.x / bmap_width(panel->bmap);
	}
	if( __flags & SCALE_Y )
	{
		shrinkness.second = screen_size.y / bmap_height(panel->bmap);
	}
	
	panel->scale_x = shrinkness.first;
	panel->scale_y = shrinkness.second;
	
	gui_panel_set_size( panel, panel->scale_x * bmap_width(panel->bmap), panel->scale_y * bmap_width(panel->bmap) );
}

/*
 * void gui_panel_set_center( Panel *panel, const int mode )
 * 
 * Align a panel to center. The panel must contain a valid bitmap.
 * The reason for this separate function instead of merging into gui_panel_set_align()
 * is you can't do flag combining on gui_panel_set_align(); besides, it was intended to only 
 * use a specific align mode instead of multiple modes (which are combined by the & operator).
 */
void gui_panel_set_center( Panel *panel, const int mode )
{
	if( !panel ) return;
	
	if( mode & CENTER_X )
	{
		panel->pos_x = (screen_size.x - bmap_width(panel->bmap)) / 2;
	}
	if( mode & CENTER_Y )
	{
		panel->pos_y = (screen_size.y - bmap_height(panel->bmap)) / 2;
	}
}
