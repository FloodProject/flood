/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "vapor/math/Math.h"

namespace vapor {

//-----------------------------------//

float Interpolate::linear(float a, float b, float t)
{
	// Linear interpolation.
	return a*(1-t) + b*t;
}

//-----------------------------------//

float Interpolate::cosine(float a, float b, float t)
{
	// Cosine interpolation.
	float ft = t * Math::PI;
	float f = (1 - std::cos(ft)) * 0.5f;

	// Linear interpolation.
	return Interpolate::linear(a, b, f);
}

//-----------------------------------//

float Interpolate::cubic(float a, float b, float t)
{
	// Cubic interpolation.
	float step = t*t*(3-2*t);

	// Linear interpolation.
	return Interpolate::linear(a, b, step);
}

//-----------------------------------//

bool Math::floatEqual(float a, float b)
{
	return fabs(a - b) < 
		Limits::FloatEpsilon;
}

//-----------------------------------//

float Math::logBase( float base, float value )
{
	return log(value) / log(base);
}

//-----------------------------------//

float Math::random( float min, float max )
{
	float n = (max - min) * (rand() / RAND_MAX) + min;
	
	return n;
}

//-----------------------------------//

float Math::degreeToRadian( float degree )
{
	return degree * PI / 180.0f;
}

//-----------------------------------//

float Math::radianToDegree( float radian )
{
	return radian * 180.0f / PI;
}

//-----------------------------------//

Vector3 calculateTriangleNormal( const Vector3& v1, const Vector3& v2, const Vector3& v3 )
{
	Vector3 vec1 = v2 - v1;
	Vector3 vec2 = v3 - v1;
	Vector3 normal = vec1.cross(vec2);
	
	return normal.normalize();
}

//-----------------------------------//

} // end namespace