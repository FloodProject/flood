/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * Represents a 4-dimensional vector class template.
 */

template <typename T>
class Vector4T
{
public:

	// Default empty constructor.
	Vector4T ()
		: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{ }

	//-----------------------------------//
	
	// Copy constructor.
	Vector4T (const Vector4T<T>& v)
		: x(v.x), y(v.y), z(v.z), w(v.w)
	{ }

	//-----------------------------------//
	
	// Constructs a vector from a vector and 1 value.
	Vector4T (const Vector3T<T>& v, T w)
		: x(v.x), y(v.y), z(v.z), w(w)
	{ }

	//-----------------------------------//

	// Constructs a vector from 1 value.
	Vector4T (const T v)
		: x(v), y(v), z(v), w(v)
	{ }

	//-----------------------------------//

	// Constructs a vector from 4 values.
	Vector4T (const T a, const T b, const T c, const T d)
		: x(a), y(b), z(c), w(d)
	{ }

	//-----------------------------------//
	
	// Assignment.
	Vector4T &operator = (const Vector4T<T>& v)
	{
		x = v.x; y = v.y; z = v.z; w = v.w;
		return *this;
	}

	//-----------------------------------//

	// Equality.
	bool operator == (const Vector4T<T>& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); 
	}

	//-----------------------------------//

	// Inequality.
    bool operator != (const Vector4T<T>& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
	}

	//-----------------------------------//

	// Addition combined.
	Vector4T &operator += (const Vector4T<T>& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	//-----------------------------------//

	// Subtraction combined.
	Vector4T &operator -= (const Vector4T<T>& v) 
	{
		x -= v.x; y -= v.y;	z -= v.z; w -= v.w;
		return *this;
	}

	//-----------------------------------//

	// Multiplication by a scalar combined.
	Vector4T &operator *= (float s) {
		x *= s;	y *= s;	z *= s; w *= s;
		return *this;
	}

	//-----------------------------------//

	// Division by a scalar combined.
	Vector4T &operator /= (float s) 
	{
		//assert(s != 0);
		*this *= 1.0f / s;
		return *this;
	}

	//-----------------------------------//
	
	// Unary minus.
	Vector4T operator - () const
	{
		return Vector4T(-x, -y, -z, -w);
	}

	//-----------------------------------//

	// Binary addition.
	Vector4T operator + (const Vector4T<T>& v) const
	{
		return Vector4T(x+v.x, y+v.y, z+v.z, w+v.w);
	}

	//-----------------------------------//

	// Binary subtraction.
	Vector4T operator - (const Vector4T<T>& v) const
	{
		return Vector4T(x-v.x, y-v.y, z-v.z, w-v.w);
	}

	//-----------------------------------//
	
	// Multiplication by a scalar.
	Vector4T operator * (float s) const
	{
		return Vector4T(x*s, y*s, z*s, w*s);
	}

	//-----------------------------------//

	// Division by a scalar.
	Vector4T operator / (float s) const
	{
		//assert(s != 0);
		float t = 1.0f / s;
		return Vector4T(x*t, y*t, z*t, w*t);
	}

	//-----------------------------------//

	// Zeroes the vector.
	void zero()
	{
		x = y = z = w = 0.0f;
	}

	////-----------------------------------//

	//// Gets the length of the vector.
	//T length() const
	//{
	//	return sqrt(x*x + y*y + z*z + w*w);
	//}

	////-----------------------------------//

	//// Gets the length of the vector squared.
	//T lengthSquared() const
	//{
	//	return x*x + y*y + z*z + w*w;
	//}

	//-----------------------------------//
	
	//// Normalize the vector.
	//Vector4T normalize() 
	//{
	//	T len = length();

	//	if (len > 0.0f) // check for divide-by-zero
	//	{
	//		x /= len;
	//		y /= len;
	//		z /= len;
	//		w /= len;
	//	}

	//	return *this;
	//}

	////-----------------------------------//

	//// Dot product.
	//float dot( const Vector4T& v ) const
	//{
	//	return x*v.x + y*v.y + z*v.z + w*v.w;
	//}

	////-----------------------------------//

	//// Cross product.
	//Vector4T cross( const Vector4T& v ) const
	//{
	//	return Vector4T<T>(
	//		y*v.z - z*v.y,
	//		z*v.x - x*v.z,
	//		x*v.y - y*v.x,
	//		);
	//}

	//-----------------------------------//

	//// Distance between two points.
	//T distance( const Vector4T& v ) const
	//{
	//	return (v-*this).length();
	//}

	//-----------------------------------//

	bool operator < ( const Vector4T& rhs ) const
	{
		return (x < rhs.x) && (y < rhs.y) && (z < rhs.z) && (w < rhs.w);
	}

	//-----------------------------------//

	bool operator > ( const Vector4T& rhs ) const
	{
		return (x > rhs.x) && (y > rhs.y) && (z > rhs.z) && (w > rhs.w);
	}

	//-----------------------------------//

#ifndef SWIG
	// Special points
	static VAPOR_API const Vector4T<float> Zero;
	static VAPOR_API const Vector4T<float> UnitX;
    static VAPOR_API const Vector4T<float> UnitY;
    static VAPOR_API const Vector4T<float> UnitZ;
	static VAPOR_API const Vector4T<float> UnitW;
#endif

public:

	T x, y, z, w;
};

//-----------------------------------//

/**
 * Specific types
 */

typedef VAPOR_API Vector4T<int> Vector4i;
typedef VAPOR_API Vector4T<float> Vector4;
typedef VAPOR_API Vector4T<double> Vector4d;

//-----------------------------------//

} // end namespace


