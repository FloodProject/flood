/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"

namespace vapor { namespace math {

//-----------------------------------//

/**
 * Representation of colors in RGBA format.
 */

class VAPOR_API Color
{
public:

	Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	Color(int r, int g, int b, int a = 255);
	Color(const Color& c);
	
	operator Vector3 () const
	{
		return Vector3( r, g, b );
	}

	float r, g, b, a;
};

//-----------------------------------//

namespace Colors 
{
	static const Color White(1.0f, 1.0f, 1.0f);
	static const Color Black(0.0f, 0.0f, 0.0f);
	static const Color Red(1.0f, 0.0f, 0.0f);
	static const Color Green(0.0f, 1.0f, 0.0f);
	static const Color Blue(0.0f, 0.0f, 1.0f);
	static const Color SkyBlue(0.5f, 0.7f, 1.0f);
	static const Color Yellow(1.0f, 1.0f, 0.0f);
}

//-----------------------------------//

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

//-----------------------------------//

} } // end namespaces