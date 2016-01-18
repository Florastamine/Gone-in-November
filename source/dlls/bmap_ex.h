/* bmap_ex.h */
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
 * <bmap_ex>
 * Contains extra bmap_() functions which performs various transformations/filters
 * to images, that are currently missing from the Gamestudio API.
 * Based on CImg library 1.5.2.
 *
 * Taken and modified from the TUST project. (https://github.com/MasterQ32/TUST)
 * DLL source code can be found under ./source/cpp/bmap.
 * __________________________________________________________________
 */
#ifndef    __GUI_IP_EX__
#define    __GUI_IP_EX__

MATERIAL  *__bmapmatLerp__ = NULL;
MATERIAL  *__bmapmatRotate__ = NULL;

#define    MIRROR_X    1
#define    MIRROR_Y    2

/**
 * \brief Interpolates two bitmaps by a factor. Bitmap size independant.
 * \param	destiny		A bitmap to contain the interpolation.
 * \param	source1		First bitmap to interpolate.
 * \param	source2		Second bitmap to interpolate.
 * \param	factor		The factor of the interpolation.
 */
void bmap_lerp ( BMAP *destiny, BMAP *source1, BMAP *source2, var factor );

/**
 * \brief Draws a rotated bitmap over another bitmap
 * \param	destiny				A bitmap to contain the rotated bitmap.
 * \param	source				The bitmap to be rotated.
 * \param	source_offset_x	Offset from the source center
 *				source_offset_y
 * \param	destiny_offset_x	Offset from the destiny center
 *				destiny_offset_y
 * \param	angle					The amount of rotation.
 */
void bmap_rotate ( BMAP *destiny, BMAP *source, var source_offset_x, var source_offset_y, var destiny_offset_x, var destiny_offset_y, var angle );

/**
 * \brief Blurs a bitmap
 * \param	BMAP*					Source bitmap
 * \param	float					Blur factor
 * \return	Finished bitmap
 */
BMAP *bmap_blur(BMAP *_target, float _factor);

/**
 * \brief Inverts a bitmap
 * \param	BMAP*					Source bitmap
 * \return	Finished bitmap
 */
BMAP *bmap_invert(BMAP *_target);

/**
 * \brief Adds noise to bitmap
 * \param	BMAP*					Source bitmap
 * \param	float					Noise factor
 * \return	Finished bitmap
 */
BMAP *bmap_noise(BMAP *_target, float _factor);

/**
 * \brief Rotates a bitmap
 * \param	BMAP*					Source bitmap
 * \param	float					Rotation angle
 * \param	float					Rotation point (x value)
 * \param	float					Rotation point (y value)
 * \param	float					Zoom factor
 * \return	Finished bitmap
 */
BMAP *bmap_rotate(BMAP *_target, float _angle, float _cx, float _cy, float _zoom);

/**
 * \brief Resizes a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					New size (x value)
 * \param	int					New size (y value)
 * \return	Finished bitmap
 */
BMAP *bmap_resize(BMAP *_target, int _sizeX, int _sizeY);

/**
 * \brief Mirrors a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					Mirror axis (x=1, y=2)
 * \return	Finished bitmap
 */
BMAP *bmap_mirror(BMAP *_target, int _axis);

/**
 * \brief Crops an image (Get a specific part of the bmap)
 * \param	BMAP*					Source bitmap
 * \param	int					New top left (x value)
 * \param	int					New top left (y value)
 * \param	int					New bottom right (x value)
 * \param	int 					New bottom right (y value)
 * \return	Finished bitmap
 */
BMAP *bmap_crop(BMAP *_target, int _x1, int _y1, int _x2, int _y2);

/**
 * \brief Erodes a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					Erode factor
 * \return	Finished bitmap
 */
BMAP *bmap_erode(BMAP *_target, int _factor);

/**
 * \brief Add dilate filter
 * \param	BMAP*					Source bitmap
 * \param	int					Dilate factor
 * \return	Finished bitmap
 */
BMAP *bmap_dilate(BMAP *_target, int _factor);

/**
 * \brief Sharpens a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					Amplitude
 * \return	Finished bitmap
 */
BMAP *bmap_sharpen(BMAP *_target, int _amplitude);

/**
 * \brief Add laplace filter
 * \param	BMAP*					Source bitmap
 * \return	Finished bitmap
 */
BMAP *bmap_laplace(BMAP *_target);

/**
 * \brief Draws a point to a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					x value
 * \param	int					y value
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha of the point (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_point(BMAP *_target, int _x, int _y, COLOR* _color, float _alpha);

/**
 * \brief Draws a line to a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					Start point (x value)
 * \param	int					Start point (y value)
 * \param	int					End point (x value)
 * \param	int					End point (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha of the point (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_line(BMAP *_target, int _x1, int _y1, int _x2, int _y2, COLOR* _color, float _alpha);

/**
 * \brief Draws borders to a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					Distance to borders (x value)
 * \param	int					Distance to borders (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_border(BMAP *_target, int _distX, int _distY, COLOR* _color, float _alpha);

/**
 * \brief Draws a line to a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					Start point (x value)
 * \param	int					Start point (y value)
 * \param	int					End point (x value)
 * \param	int					End point (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha of the point (0.0-1.0)
 * \param	float					Angle of the arrow head
 * \return	Finished bitmap
 */
BMAP *bmap_draw_arrow(BMAP *_target, int _x1, int _y1, int _x2, int _y2, COLOR* _color, float _alpha, float _angle);

/**
 * \brief Draws a spline to a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					Start point (x value)
 * \param	int					Start point (y value)
 * \param	int					Start velocity (x value)
 * \param	int					Start velocity (y value)
 * \param	int					End point (x value)
 * \param	int					End point (y value)
 * \param	int					End velocity (x value)
 * \param	int					End velocity (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha of the point (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_spline(BMAP *_target, int _x1, int _y1, int _u1, int _v1, int _x2, int _y2, int _u2, int _v2, COLOR* _color, float _alpha);

/**
 * \brief Draws a triangle
 * \param	BMAP*					Source bitmap
 * \param	int					Point 1 (x value)
 * \param	int					Point 1 (y value)
 * \param	int					Point 2 (x value)
 * \param	int					Point 2 (y value)
 * \param	int					Point 3 (x value)
 * \param	int					Point 3 (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_triangle(BMAP *_target, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, COLOR* _color, float _alpha);

/**
 * \brief Draws a filled triangle
 * \param	BMAP*					Source bitmap
 * \param	int					Point 1 (x value)
 * \param	int					Point 1 (y value)
 * \param	int					Point 2 (x value)
 * \param	int					Point 2 (y value)
 * \param	int					Point 3 (x value)
 * \param	int					Point 3 (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_triangle_filled(BMAP *_target, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, COLOR* _color, float _alpha);

/**
 * \brief Draws a rectangle
 * \param	BMAP*					Source bitmap
 * \param	int					Point top left (x value)
 * \param	int					Point top left (y value)
 * \param	int					Point bottom right (x value)
 * \param	int					Point bottom right (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_rectangle(BMAP *_target, int _x1, int _y1, int _x2, int _y2, COLOR* _color, float _alpha);

/**
 * \brief Draws a filled rectangle
 * \param	BMAP*					Source bitmap
 * \param	int					Point top left (x value)
 * \param	int					Point top left (y value)
 * \param	int					Point bottom right (x value)
 * \param	int					Point bottom right (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_rectangle_filled(BMAP *_target, int _x1, int _y1, int _x2, int _y2, COLOR* _color, float _alpha);

/**
 * \brief Draws an ellipse
 * \param	BMAP*					Source bitmap
 * \param	int					Middle of the ellipse (x value)
 * \param	int					Middle of the ellipse (y value)
 * \param	int					Radius 1
 * \param	int					Radius 2
 * \param	float					Angle of first radius
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_ellipse(BMAP *_target, int _x1, int _y1, int _radius1, int _radius2, float _angle, COLOR* _color, float _alpha);

/**
 * \brief Draws a filled ellipse
 * \param	BMAP*					Source bitmap
 * \param	int					Middle of the ellipse (x value)
 * \param	int					Middle of the ellipse (y value)
 * \param	int					Radius 1
 * \param	int					Radius 2
 * \param	float					Angle of first radius
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_ellipse_filled(BMAP *_target, int _x1, int _y1, int _radius1, int _radius2, float _angle, COLOR* _color, float _alpha);

/**
 * \brief Draws a circle
 * \param	BMAP*					Source bitmap
 * \param	int					Middle of the circle (x value)
 * \param	int					Middle of the circle (y value)
 * \param	int					Radius
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_circle(BMAP *_target, int _x1, int _y1, int _radius, COLOR* _color, float _alpha);

/**
 * \brief Draws a filled circle
 * \param	BMAP*					Source bitmap
 * \param	int					Middle of the circle (x value)
 * \param	int					Middle of the circle (y value)
 * \param	int					Radius
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_circle_filled(BMAP *_target, int _x1, int _y1, int _radius, COLOR* _color, float _alpha);

/**
 * \brief Draws a BMAP *onto another
 * \param	BMAP*					Source bitmap
 * \param	BMAP*					Bmap that is put onto the other
 * \param	int					Target position (x value)
 * \param	int					Target position (y value)
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_draw_bmap(BMAP *_target, BMAP *_source, int _x1, int _y1, float _alpha);

/**
 * \brief Draws a text
 * \param	BMAP*					Source bitmap
 * \param	int					Draw position (x value)
 * \param	int					Draw position (y value)
 * \param	char*					Text to write
 * \param	COLOR*				Foreground color
 * \param	COLOR*				Background color
 * \param	float					Alpha value (0.0-1.0)
 * \param	int					Font size
 * \return	Finished bitmap
 */
BMAP *bmap_draw_text(BMAP *_target, int _x1, int _y1, char *_text, COLOR* _foregroundColor, COLOR* _backgroundColor, float _alpha, int _fontSize);

/**
 * \brief Fills a specific region of a bitmap
 * \param	BMAP*					Source bitmap
 * \param	int					Start position (x value)
 * \param	int					Start position (y value)
 * \param	COLOR*				Color to draw the point
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_fill_ext(BMAP *_target, int _x1, int _y1, COLOR* _color, float _alpha);

/**
 * \brief Draws a random plasma figure
 * \param	BMAP*					Source bitmap
 * \param	float					Alpha value (0.0-1.0)
 * \return	Finished bitmap
 */
BMAP *bmap_plasma(BMAP *_target, float _alpha);

#include "bmap_ex.c"
#endif /* bmap_ex.h */
