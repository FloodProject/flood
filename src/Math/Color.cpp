/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Math/Color.h"
#include "Math/Helpers.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

//-----------------------------------//

#define D255(P) (P/255.0f)

Color::Color(int r, int g, int b, int a)
{
	this->r = D255(r);
	this->g = D255(g);
	this->b = D255(b);
	this->a = D255(a);
}

//-----------------------------------//

Color::Color(const Color& c)
{
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
	this->a = c.a;
}

//-----------------------------------//

bool Color::operator == (const Color& c) const
{
	return (r == c.r)
		&& (g == c.g)
		&& (b == c.b)
		&& (a == c.a); 
}

//-----------------------------------//

bool Color::nearEqual(const Color& a, float tolerance)
{
	return (fabs(a.r - r) < tolerance)
		&& (fabs(a.g - g) < tolerance)
		&& (fabs(a.b - b) < tolerance);
}

//-----------------------------------//

Color Color::operator * (float s) const
{
	return Color(r*s, g*s, b*s, a); 
}

//-----------------------------------//

Color::operator Vector3 () const
{
	return Vector3( r, g, b );
}

//-----------------------------------//

const API_CORE Color Color::White(1.0f, 1.0f, 1.0f);
const API_CORE Color Color::Black(0.0f, 0.0f, 0.0f);
const API_CORE Color Color::LightGrey(0.7f, 0.7f, 0.7f);
const API_CORE Color Color::Red(1.0f, 0.0f, 0.0f);
const API_CORE Color Color::Green(0.0f, 1.0f, 0.0f);
const API_CORE Color Color::Blue(0.0f, 0.0f, 1.0f);
const API_CORE Color Color::NavyBlue(0.0f, 0.0f, 0.5f);
const API_CORE Color Color::SkyBlue(0.5f, 0.7f, 1.0f);
const API_CORE Color Color::Yellow(1.0f, 1.0f, 0.0f);

//-----------------------------------//

NAMESPACE_CORE_END