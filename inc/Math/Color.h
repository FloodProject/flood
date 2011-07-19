/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Math/Vector.h"

NAMESPACE_BEGIN

//-----------------------------------//

/**
 * Representation of colors in RGBA format.
 */

struct API_CORE ColorP
{
	float r, g, b, a;
};

struct API_CORE Color : public ColorP
{
	// Constructors
	Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	Color(int r, int g, int b, int a = 255);
	Color(const Color& c);

	// Equality.
	bool operator == (const Color& v) const;

	// Equality with tolerance.
	bool nearEqual(const Color& a, float tolerance = 0.05f );

	// Multiplication by a scalar.
	Color operator * (float s) const;
	
	// Conversion Operators.
	operator Vector3 () const;

	// Special colors.
	static const Color White;
	static const Color Black;
	static const Color LightGrey;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color NavyBlue;
	static const Color SkyBlue;
	static const Color Yellow;
};

//-----------------------------------//

NAMESPACE_END