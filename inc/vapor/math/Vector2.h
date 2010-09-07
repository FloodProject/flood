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
 * Simple struct to hold 2-dimensional stuff.
 */

template< typename T >
struct Vector2T
{
public:

	// X-coordinate of the point.
	T x;

	// Y-coordinate of the point.
	T y;

	//-----------------------------------//
	
	// Default empty constructor
	Vector2T()
		: x(0), y(0)
	{ }

	//-----------------------------------//
	
	// Constructs a vector from 2 values
	Vector2T( const T x, const T y )
		: x(x), y(y)
	{ }

	//-----------------------------------//
	
	// Copy constructor
	Vector2T (const Vector2T<T>& v)
		: x(v.x), y(v.y)
	{ }

	//-----------------------------------//

	// Assignment
	Vector2T& operator = (const Vector2T<T>& v)
	{
		x = v.x; y = v.y;
		return *this;
	}

	//-----------------------------------//

	// Equality
	bool operator == (const Vector2T<T>& v) const
	{
		return (x == v.x) && (y == v.y);
	}

	//-----------------------------------//

	// Inequality
    bool operator != (const Vector2T<T>& v) const
	{
		return (x != v.x) || (y != v.y);
	}

	//-----------------------------------//

	// Addition combined
	Vector2T& operator += (const Vector2T<T>& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	//-----------------------------------//

	// Subtraction combined
	Vector2T& operator -= (const Vector2T<T>& v) 
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	//-----------------------------------//
	
	// Unary minus
	Vector2T operator - () const { return Vector2T(-x, -y); }

	//-----------------------------------//

	// Binary addition
	Vector2T operator + (const Vector2T<T>& v) const { return Vector2T(x+v.x, y+v.y); }

	//-----------------------------------//

	// Binary subtraction
	Vector2T operator - (const Vector2T<T>& v) const { return Vector2T(x-v.x, y-v.y); }

	//-----------------------------------//
	
	// Multiplication by a scalar
	Vector2T operator * (float s) const { return Vector2T(x*s, y*s); }

	//-----------------------------------//
	
	// Division by a scalar
	Vector2T operator / (int s) const { return Vector2T(x/s, y/s); }

	//-----------------------------------//
	
	// Conversion Operators
	operator Vector3() const 
	{
		return Vector3(float(x), float(y), 0.0f);
	}

	//-----------------------------------//

	// Zeroes the vector
	void zero() { x = y = 0; }

	//-----------------------------------//

#ifndef SWIG
	// Special points
	static VAPOR_API const Vector2T<int> Zero;
	static VAPOR_API const Vector2T<int> UnitX;
	static VAPOR_API const Vector2T<int> UnitY;
#endif
};

//-----------------------------------//

typedef VAPOR_API Vector2T<float> Vector2;
typedef VAPOR_API Vector2T<int> Vector2i;

//-----------------------------------//

} // end namespace

