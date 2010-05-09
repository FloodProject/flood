/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/Color.h"

namespace vapor { namespace math {

//-----------------------------------//

Color::Color(float r, float g, float b, float a)
	: r(r),
	g(g),
	b(b),
	a(a)
{ }

//-----------------------------------//

Color::Color(int _r, int _g, int _b, int _a)
	: r(float(_r/255.0f)),
	g(float(_g/255.0f)),
	b(float(_b/255.0f)),
	a(float(_a/255.0f))
{ }

//-----------------------------------//

Color::Color(const Color& c)
	: r(c.r),
	b (c.b),
	g (c.g),
	a (c.a)
{ }

//-----------------------------------//

bool Color::operator == (const Color& c) const
{
	return (r == c.r)
		&& (g == c.g)
		&& (b == c.b)
		&& (a == c.a); 
}

//-----------------------------------//

const VAPOR_API Color Color::White(1.0f, 1.0f, 1.0f);
const VAPOR_API Color Color::Black(0.0f, 0.0f, 0.0f);
const VAPOR_API Color Color::Red(1.0f, 0.0f, 0.0f);
const VAPOR_API Color Color::Green(0.0f, 1.0f, 0.0f);
const VAPOR_API Color Color::Blue(0.0f, 0.0f, 1.0f);
const VAPOR_API Color Color::SkyBlue(0.5f, 0.7f, 1.0f);
const VAPOR_API Color Color::Yellow(1.0f, 1.0f, 0.0f);

//-----------------------------------//

} } // end namespaces