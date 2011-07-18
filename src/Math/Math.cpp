/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#include "Core/API.h"
#include "Math/Helpers.h"

NAMESPACE_BEGIN

//-----------------------------------//

float InterpolateLinear(float a, float b, float t)
{
	// Linear interpolation.
	return a*(1-t) + b*t;
}

//-----------------------------------//

float InterpolateCosine(float a, float b, float t)
{
	// Cosine interpolation.
	float ft = t * PI;
	float f = (1 - std::cos(ft)) * 0.5f;

	// Linear interpolation.
	return InterpolateLinear(a, b, f);
}

//-----------------------------------//

float InterpolateCubic(float a, float b, float t)
{
	// Cubic interpolation.
	float step = t*t*(3-2*t);

	// Linear interpolation.
	return InterpolateLinear(a, b, step);
}

//-----------------------------------//

bool MathFloatCompare(float a, float b)
{
	return fabs(a - b) < LimitsFloatEpsilon;
}

//-----------------------------------//

float MathFloatRandom( float min, float max )
{
	return (max - min) * (rand() / float(RAND_MAX)) + min;
}

//-----------------------------------//

float MathLogBase( float base, float value )
{
	return log(value) / log(base);
}

//-----------------------------------//

float MathDegreeToRadian( float degree )
{
	return degree * PI / 180.0f;
}

//-----------------------------------//

float MathRadianToDegree( float radian )
{
	return radian * 180.0f / PI;
}

//-----------------------------------//

NAMESPACE_END