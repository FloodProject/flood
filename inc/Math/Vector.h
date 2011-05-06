/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_BEGIN

//-----------------------------------//

struct Vector3
{
	float x, y, z;

	// Constructors.
	Vector3 () : x(0.0f), y(0.0f), z(0.0f) { }
	Vector3 (const Vector3& v) : x(v.x), y(v.y), z(v.z) { }
	Vector3 (float v) : x(v), y(v), z(v) { }
	Vector3 (float a, float b, float c) : x(a), y(b), z(c) { }

	// Assignment.
	Vector3 &operator = (const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }

	// Relational.
	bool operator == (const Vector3& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
    bool operator != (const Vector3& v) const { return (x != v.x) || (y != v.y) || (z != v.z); }
	bool operator < ( const Vector3& rhs ) const { return (x < rhs.x) && (y < rhs.y) && (z < rhs.z); }
	bool operator > ( const Vector3& rhs ) const { return (x > rhs.x) && (y > rhs.y) && (z > rhs.z); }

	// Addition.
	Vector3 operator + (const Vector3& v) const { return Vector3(x+v.x, y+v.y, z+v.z); }
	Vector3 &operator += (const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	
	// Subtraction.
	Vector3 operator - () const { return Vector3(-x, -y, -z); }
	Vector3 operator - (const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
	Vector3 &operator -= (const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

	// Multiplication.
	Vector3 operator * (float s) const { return Vector3(x*s, y*s, z*s); }
	Vector3 operator * (const Vector3& v) const { return Vector3(x*v.x, y*v.y, z*v.z); }
	Vector3 &operator *= (float s) { x *= s; y *= s;	z *= s; return *this; }

	// Division.
	Vector3 operator / (float s) const { assert(s != 0); float t = 1.0f / s; return *this * t; }
	Vector3 &operator /= (float s) { assert(s != 0); *this *= 1.0f / s; return *this; }

#ifndef SWIG
	static API_CORE const Vector3 Zero;
	static API_CORE const Vector3 UnitX;
    static API_CORE const Vector3 UnitY;
    static API_CORE const Vector3 UnitZ;
#endif

	// Zeroes the vector.
	void zero() { x = y = z = 0.0f; }

	// Gets the length of the vector.
	float length() const { return std::sqrt(x*x + y*y + z*z); }

	// Gets the length of the vector squared.
	float lengthSquared() const { return x*x + y*y + z*z; }

	// Dot product.
	float dot( const Vector3& v ) const { return x*v.x + y*v.y + z*v.z; }

	// Cross product.
	Vector3 cross( const Vector3& v ) const { return Vector3( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x ); }

	// Distance between two points.
	float distance( const Vector3& v ) const { return (v-*this).length(); }

	// Projects the vector and returns the result.
	Vector3 project(const Vector3& v) const { return v * dot(v) / length(); }

	// Linear interpolation between two vectors (blend range from 0 to 1).
	Vector3 lerp(const Vector3& vec, float blend) const 
	{
		Vector3 res;

		res.x = blend * (vec.x - x) + x;
		res.y = blend * (vec.y - y) + y;
		res.z = blend * (vec.z - z) + z;

		return res;
	}

	// Smooth interpolation between two vectors (http://en.wikipedia.org/wiki/Smoothstep).
	Vector3 serp(const Vector3& vec, float t) const 
	{
		float step = t*t*(3-2*t);
		return lerp(vec, step);
	}

	// Normalize the vector.
	Vector3 normalize() 
	{
		float len = length();
		if (len > 0.0f) { x /= len; y /= len; z /= len; }
		return *this;
	}
};

//-----------------------------------//

struct Vector4
{
	float x, y, z, w;

	// Constructors.
	Vector4 () : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
	Vector4 (const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
	Vector4 (float v) : x(v), y(v), z(v), w(v) { }
	Vector4 (float a, float b, float c, float d) : x(a), y(b), z(c), w(d) { }
	Vector4 (const Vector3& v, float d) : x(v.x), y(v.y), z(v.z), w(d) { }
	// Assignment.
	Vector4 &operator = (const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

	// Relational.
	bool operator == (const Vector4& v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
    bool operator != (const Vector4& v) const { return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w); }
	bool operator < ( const Vector4& rhs ) const { return (x < rhs.x) && (y < rhs.y) && (z < rhs.z) && (w < rhs.w); }
	bool operator > ( const Vector4& rhs ) const { return (x > rhs.x) && (y > rhs.y) && (z > rhs.z) && (w > rhs.w); }

	// Addition.
	Vector4 operator + (const Vector4& v) const { return Vector4(x+v.x, y+v.y, z+v.z, w+v.w); }
	Vector4 &operator += (const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	
	// Subtraction.
	Vector4 operator - () const { return Vector4(-x, -y, -z, -w); }
	Vector4 operator - (const Vector4& v) const { return Vector4(x-v.x, y-v.y, z-v.z, w-v.w); }
	Vector4 &operator -= (const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

	// Multiplication.
	Vector4 operator * (float s) const { return Vector4(x*s, y*s, z*s, w*s); }
	Vector4 operator * (const Vector4& v) const { return Vector4(x*v.x, y*v.y, z*v.z, w*v.w); }
	Vector4 &operator *= (float s) { x *= s; y *= s;	z *= s; w *= s; return *this; }

	// Division.
	Vector4 operator / (float s) const { assert(s != 0); float t = 1.0f / s; return *this * t; }
	Vector4 &operator /= (float s) { assert(s != 0); *this *= 1.0f / s; return *this; }

	// Zeroes the vector.
	void zero() { x = y = z = w = 0.0f; }

#ifndef SWIG
	static API_CORE const Vector4 Zero;
	static API_CORE const Vector4 UnitX;
	static API_CORE const Vector4 UnitY;
	static API_CORE const Vector4 UnitZ;
	static API_CORE const Vector4 UnitW;
#endif
};

//-----------------------------------//

struct Vector2
{
	float x, y;

	// Constructors.
	Vector2() : x(0), y(0) { }
	Vector2(float x, float y) : x(x), y(y) { }
	Vector2 (const Vector2& v) : x(v.x), y(v.y) { }

	// Assignment
	Vector2& operator = (const Vector2& v) { x = v.x; y = v.y; return *this; }

	// Relational.
	bool operator == (const Vector2& v) const { return (x == v.x) && (y == v.y); }
    bool operator != (const Vector2& v) const { return (x != v.x) || (y != v.y); }
	bool operator < ( const Vector2& rhs ) const { return (x < rhs.x) && (y < rhs.y); }
	bool operator > ( const Vector2& rhs ) const { return (x > rhs.x) && (y > rhs.y); }

	// Addition.
	Vector2 operator + (const Vector2& v) const { return Vector2(x+v.x, y+v.y); }	
	Vector2& operator += (const Vector2& v) { x += v.x; y += v.y; return *this; }
	
	// Subtraction.
	Vector2 operator - () const { return Vector2(-x, -y); }	
	Vector2 operator - (const Vector2& v) const { return Vector2(x-v.x, y-v.y); }
	Vector2& operator -= (const Vector2& v) { x -= v.x; y -= v.y; return *this; }

	// Multiplication.
	Vector2 operator * (float s) const { return Vector2(x*s, y*s); }
	
	// Division.
	Vector2 operator / (int s) const { return Vector2(x/s, y/s); }

	// Conversion.
	operator Vector3() const  { return Vector3(float(x), float(y), 0.0f); }

	// Zeroes the vector.
	void zero() { x = y = 0; }

#ifndef SWIG
	static API_CORE const Vector2 Zero;
	static API_CORE const Vector2 UnitX;
	static API_CORE const Vector2 UnitY;
#endif
};

//-----------------------------------//

struct Vector2i
{
	int32 x, y;

	// Constructors.
	Vector2i() : x(0), y(0) { }
	Vector2i(int32 x, int32 y) : x(x), y(y) { }
	Vector2i (const Vector2i& v) : x(v.x), y(v.y) { }

	// Assignment
	Vector2i& operator = (const Vector2i& v) { x = v.x; y = v.y; return *this; }

	// Relational.
	bool operator == (const Vector2i& v) const { return (x == v.x) && (y == v.y); }
    bool operator != (const Vector2i& v) const { return (x != v.x) || (y != v.y); }
	bool operator < ( const Vector2i& rhs ) const { return (x < rhs.x) && (y < rhs.y); }
	bool operator > ( const Vector2i& rhs ) const { return (x > rhs.x) && (y > rhs.y); }

	// Addition.
	Vector2i operator + (const Vector2i& v) const { return Vector2i(x+v.x, y+v.y); }	
	Vector2i& operator += (const Vector2i& v) { x += v.x; y += v.y; return *this; }
	
	// Subtraction.
	Vector2i operator - () const { return Vector2i(-x, -y); }	
	Vector2i operator - (const Vector2i& v) const { return Vector2i(x-v.x, y-v.y); }
	Vector2i& operator -= (const Vector2i& v) { x -= v.x; y -= v.y; return *this; }

	// Multiplication.
	Vector2i operator * (int32 s) const { return Vector2i(x*s, y*s); }
	
	// Division.
	Vector2i operator / (int s) const { return Vector2i(x/s, y/s); }

	// Zeroes the vector.
	void zero() { x = y = 0; }
};

//-----------------------------------//

NAMESPACE_END


