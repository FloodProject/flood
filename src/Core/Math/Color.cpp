/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Math/Color.h"
#include "Core/Math/Helpers.h"
#include <algorithm>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Color::Color()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 255;
}

//-----------------------------------//

Color::Color(byte r, byte g, byte b, byte a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
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
	return (abs(a.r - r) < tolerance)
		&& (abs(a.g - g) < tolerance)
		&& (abs(a.b - b) < tolerance);
}

//-----------------------------------//

Color Color::operator * (float s) const
{
	byte nr = (byte)std::min(255.0f,r*s);
	byte ng = (byte)std::min(255.0f,g*s);
	byte nb = (byte)std::min(255.0f,b*s);
	return Color(nr, ng, nb, a); 
}

//-----------------------------------//

Color::operator Vector3 () const
{
	return Vector3( r, g, b );
}

//-----------------------------------//

const API_CORE Color Color::White(255, 255, 255);
const API_CORE Color Color::Black(0, 0, 0);
const API_CORE Color Color::LightGrey(178, 178, 178);
const API_CORE Color Color::Red(255, 0, 0);
const API_CORE Color Color::Green(0, 255, 0);
const API_CORE Color Color::Blue(0, 0, 255);
const API_CORE Color Color::NavyBlue(0, 0, 127);
const API_CORE Color Color::SkyBlue(127, 178, 255);
const API_CORE Color Color::Yellow(255, 255, 0);

//-----------------------------------//

NAMESPACE_CORE_END