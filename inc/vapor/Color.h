/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//// 
//// fuckEngine by triton
////
//// License: fuckGNU License
////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Engine.h"

namespace vapor {
	namespace resources {

struct Color
{
	uint r;
	uint g;
	uint b;
	uint a;

public:

	Color(uint red, uint green, uint blue)
		: r(red), g(green), b(blue)
	{ }
};

typedef struct Color Color;

/////////////////////////////////////////////////////////////////////////////
// Color definitions
/////////////////////////////////////////////////////////////////////////////

static const Color White (255, 255, 255);

//ColorRGB operator+(const ColorRGB& color, const ColorRGB& color2);
//ColorRGB operator-(const ColorRGB& color, const ColorRGB& color2);
//ColorRGB operator*(const ColorRGB& color, int a);
//ColorRGB operator*(int a, const ColorRGB& color);
//ColorRGB operator/(const ColorRGB& color, int a);
//bool operator==(const ColorRGB& color, const ColorRGB& color2);
//bool operator!=(const ColorRGB& color, const ColorRGB& color2);

//static const ColorRGB RGB_Black    (  0,   0,   0);
//static const ColorRGB RGB_Red      (255,   0,   0);
//static const ColorRGB RGB_Green    (  0, 255,   0);
//static const ColorRGB RGB_Blue     (  0,   0, 255);
//static const ColorRGB RGB_Cyan     (  0, 255, 255);
//static const ColorRGB RGB_Magenta  (255,   0, 255);
//static const ColorRGB RGB_Yellow   (255, 255,   0);
//static const ColorRGB RGB_White    (255, 255, 255);
//static const ColorRGB RGB_Gray     (128, 128, 128);
//static const ColorRGB RGB_Grey     (192, 192, 192);
//static const ColorRGB RGB_Maroon   (128,   0,   0);
//static const ColorRGB RGB_Darkgreen(  0, 128,   0);
//static const ColorRGB RGB_Navy     (  0,   0, 128);
//static const ColorRGB RGB_Teal     (  0, 128, 128);
//static const ColorRGB RGB_Purple   (128,   0, 128);
//static const ColorRGB RGB_Olive    (128, 128,   0);

} } // end namespaces