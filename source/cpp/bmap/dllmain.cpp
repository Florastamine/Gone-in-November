#include "stdafx.h"
#include "CImg.h"

#define    DLL_USE
#include "adll.h"

using namespace cimg_library;

// ----------------------------------------------------------------------------------------
// HEADER
// ----------------------------------------------------------------------------------------

// Internal
BMAP *cImg2LiteC(CImg<unsigned char> _img);
CImg<unsigned char> LiteC2cImg(BMAP *_img);
double round(double d);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	engine_bind();
	return TRUE;
}

// ----------------------------------------------------------------------------------------
// External
// ----------------------------------------------------------------------------------------
DLLFUNC BMAP *bmap_blur(BMAP *_target, float _factor)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.blur(_factor);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_invert(BMAP *_target)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.invert(false);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_noise(BMAP *_target, float _factor)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.noise(_factor);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_rotate(BMAP *_target, float _angle, float _cx, float _cy, float _zoom)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.rotate(_angle, _cx, _cy, _zoom, 0, 0);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_resize(BMAP *_target, int _sizeX, int _sizeY)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.resize(_sizeX, _sizeY);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_mirror(BMAP *_target, int _axis)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	if (_axis == 1) {
		tempImg.mirror('x');
	} else {
		tempImg.mirror('y');
	}
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_crop(BMAP *_target, int _x1, int _y1, int _x2, int _y2)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.crop(_x1, _y1, _x2, _y2);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_erode(BMAP *_target, int _factor)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.erode(_factor);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_dilate(BMAP *_target, int _factor)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.dilate(_factor);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_sharpen(BMAP *_target, int _amplitude)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.sharpen(_amplitude);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_laplace(BMAP *_target)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.laplacian();
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_point(BMAP *_target, int _x, int _y, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_point(_x, _y, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_line(BMAP *_target, int _x1, int _y1, int _x2, int _y2, COLOR* _color, float _alpha)
{

	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_line(_x1, _y1, _x2, _y2, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_arrow(BMAP *_target, int _x1, int _y1, int _x2, int _y2, COLOR* _color, float _alpha, float _angle)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_arrow(_x1, _y1, _x2, _y2, _c, _alpha, _angle);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_spline(BMAP *_target, int _x1, int _y1, int _u1, int _v1, int _x2, int _y2, int _u2, int _v2, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_spline(_x1, _y1, _u1, _v1, _x2, _y2, _u2, _v2, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_triangle(BMAP *_target, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_triangle(_x1, _y1, _x2, _y2, _x3, _y3, _c, _alpha, ~0U);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_triangle_filled(BMAP *_target, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_triangle(_x1, _y1, _x2, _y2, _x3, _y3, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_border(BMAP *_target, int _distX, int _distY, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_line(_distX, _distY, tempImg.width() - _distX - 1, _distY, _c, _alpha);
	tempImg.draw_line(tempImg.width() - _distX - 1, _distY, tempImg.width() - _distX - 1, tempImg.height() - _distY - 1, _c, _alpha);
	tempImg.draw_line(tempImg.width() - _distX - 1, tempImg.height() - _distY - 1, _distX, tempImg.height() - _distY - 1, _c, _alpha);
	tempImg.draw_line(_distX, tempImg.height() - _distY - 1, _distX, _distY, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_rectangle(BMAP *_target, int _x1, int _y1, int _x2, int _y2, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_rectangle(_x1, _y1, _x2, _y2, _c, _alpha, ~0U);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_rectangle_filled(BMAP *_target, int _x1, int _y1, int _x2, int _y2, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_rectangle(_x1, _y1, _x2, _y2, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_ellipse(BMAP *_target, int _x1, int _y1, int _radius1, int _radius2, float _angle, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_ellipse(_x1, _y1, _radius1, _radius2, _angle, _c, _alpha, ~0U);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_ellipse_filled(BMAP *_target, int _x1, int _y1, int _radius1, int _radius2, float _angle, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_ellipse(_x1, _y1, _radius1, _radius2, _angle, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_circle(BMAP *_target, int _x1, int _y1, int _radius, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_circle(_x1,_y1,_radius, _c, _alpha, ~0U);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_circle_filled(BMAP *_target, int _x1, int _y1, int _radius, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_circle(_x1,_y1,_radius, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_bmap(BMAP *_target, BMAP *_source, int _x1, int _y1, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	CImg<unsigned char> tempImg2(LiteC2cImg(_source));
	tempImg.draw_image(_x1,_y1,tempImg2,_alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_draw_text(BMAP *_target, int _x1, int _y1, char* _text, COLOR* _foregroundColor, COLOR* _backgroundColor, float _alpha, int _fontSize)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c1[] = { _INT(_foregroundColor->red), _INT(_foregroundColor->green), _INT(_foregroundColor->blue) };
	unsigned char _c2[] = { _INT(_backgroundColor->red), _INT(_backgroundColor->green), _INT(_backgroundColor->blue) };
	tempImg.draw_text(_x1, _y1, _text, _c1, _c2, _alpha, _fontSize);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_fill_ext(BMAP *_target, int _x1, int _y1, COLOR* _color, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	unsigned char _c[3] = { _INT(_color->red), _INT(_color->green), _INT(_color->blue) };
	tempImg.draw_fill(_x1, _y1, _c, _alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

DLLFUNC BMAP *bmap_plasma(BMAP *_target, float _alpha)
{
	CImg<unsigned char> tempImg(LiteC2cImg(_target));
	tempImg.draw_plasma(_alpha);
	_target = cImg2LiteC(tempImg);
	return _target;
}

// ----------------------------------------------------------------------------------------
// Internal
// ----------------------------------------------------------------------------------------
BMAP *cImg2LiteC(CImg<unsigned char> _img)
{
	BMAP *result = bmap_createblack(_VAR(_img.width()), _VAR(_img.height()), 32);
	var vFormat = bmap_lock(result, 0);
	var vPixel;
	COLOR cColor;
	for (int i = 0; i<_img.width(); i++)
	{
		for(int j = 0; j<_img.height(); j++)
		{
			cColor.red = _VAR(_img(i,j,0,0));
			cColor.green = _VAR(_img(i,j,0,1));
			cColor.blue = _VAR(_img(i,j,0,2));
			vPixel = pixel_for_vec(&cColor, 100, vFormat);
			pixel_to_bmap(result, _VAR(i), _VAR(j), vPixel);
		}
	}
	bmap_unlock(result);
	return result;
}

CImg<unsigned char> LiteC2cImg(BMAP *_img)
{
	CImg<unsigned char> tempImg(_INT(bmap_width(_img)),_INT(bmap_height(_img)),1,3,0);
	COLOR tempColor;
	var tempAlpha;
	var tempPixel;
	bmap_lock(_img, 0);
	for(int i=0; i<_INT(bmap_width(_img)); i++)
	{
		for(int j=0; j<_INT(bmap_height(_img)); j++)
		{

				tempPixel = pixel_for_bmap(_img, _VAR(i), _VAR(j));
				pixel_to_vec(&tempColor, &tempAlpha, _VAR(8888), tempPixel);
				tempImg(i,j,0,0) = _INT(tempColor.red);
				tempImg(i,j,0,1) = _INT(tempColor.green);
				tempImg(i,j,0,2) = _INT(tempColor.blue);
		}
	}
	bmap_unlock(_img);
	return tempImg;
}

double round(double x) {
   if (x >= 0)
      return (long) (x+0.5);
   return (long) (x-0.5);
}
