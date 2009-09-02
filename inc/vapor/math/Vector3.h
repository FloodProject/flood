/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

namespace vapor {
	namespace math {

//-----------------------------------//

/**
 * Represents a 3-dimensional vector class template.
 */

template <typename T>
class Vector3T
{
public:

	T x, y, z;

	// Default empty constructor
	Vector3T ()
		: x(0.0f), y(0.0f), z(0.0f)
	{ }
	
	// Copy constructor
	Vector3T (const Vector3T<T> &v)
		: x(v.x), y(v.y), z(v.z)
	{ }

	// Constructs a vector from 3 values
	Vector3T (const T a, const T b, const T c)
		: x(a), y(b), z(c)
	{ }
	
	// Assignment
	Vector3T &operator = (const Vector3T<T> &v)
	{
		x = v.x; y = v.y; z = v.z;
		return *this;
	}

	// Equality
	bool operator == (const Vector3T<T> &v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z); 
	}

	// Inequality
    bool operator != (const Vector3T<T> &v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z);
	}

	// Addition combined
	Vector3T &operator += (const Vector3T<T> &v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	// Subtraction combined
	Vector3T &operator -= (const Vector3T<T> &v) 
	{
		x -= v.x; y -= v.y;	z -= v.z;
		return *this;
	}

	// Multiplication by a scalar combined
	Vector3T &operator *= (float s) {
		x *= s;	y *= s;	z *= s;
		return *this;
	}

	// Division by a scalar combined
	Vector3T &operator /= (float s) 
	{
		//assert(s != 0);
		*this *= 1.0f / s;
		return *this;
	}
	
	// Unary minus
	Vector3T operator - () const { return Vector3T(-x, -y, -z); }

	// Binary addition
	Vector3T operator + (const Vector3T<T> &v) const { return Vector3T(x+v.x, y+v.y, z+v.z); }

	// Binary subtraction
	Vector3T operator - (const Vector3T<T> &v) const { return Vector3T(x-v.x, y-v.y, z-v.z); }
	
	// Multiplication by a scalar
	Vector3T operator * (float s) const { return Vector3T(x*s, y*s, z*s); }

	// Division by a scalar	
	Vector3T operator / (float s) const
	{
		//assert(s != 0);
		float t = 1.0f / s;
		return Vector3T(x*t, y*t, z*t);
	}

	// Zeroes the vector
	void zero() { x = y = z = 0.0f; }

	// Gets the length of the vector
	T length() const { return sqrt(x*x + y*y + z*z); }
	
	// Normalize the vector
	void normalize() 
	{
		T len = length();

		if (len > 0.0f) // check for divide-by-zero
		{
			x /= len; y /= len; z /= len; 
		}
	}

	// Special points
	static const Vector3T<float> Zero;
    //static const Vector3 UNIT_X;
    //static const Vector3 UNIT_Y;
    //static const Vector3 UNIT_Z;
    //static const Vector3 NEGATIVE_UNIT_X;
    //static const Vector3 NEGATIVE_UNIT_Y;
    //static const Vector3 NEGATIVE_UNIT_Z;
    //static const Vector3 UNIT_SCALE;
};

//-----------------------------------//

/**
 * Specific types
 */

typedef Vector3T<float> Vector3;
typedef Vector3T<double> Vector3d;

typedef Vector3 Vertex;



//-----------------------------------//

} } // end namespaces


