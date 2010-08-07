/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"

namespace vapor {

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
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color NavyBlue;
	static const Color SkyBlue;
	static const Color Yellow;

public:

	float r, g, b, a;
};

//-----------------------------------//

} // end namespace