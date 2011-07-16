/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#pragma once

#include "Math/Vector.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

const float LimitsFloatMinimum = -std::numeric_limits<float>::max();
const float LimitsFloatMaximum =  std::numeric_limits<float>::max();
const float LimitsFloatEpsilon =  std::numeric_limits<float>::epsilon();
const float PI = 3.141592654f;

API_CORE bool  MathFloatCompare(float a, float b);
API_CORE float MathFloatRandom( float min, float max );
API_CORE float MathLogBase( float base, float value );

API_CORE float MathDegreeToRadian( float degree );
API_CORE float MathRadianToDegree( float radian );

API_CORE float InterpolateLinear(float a, float b, float t);
API_CORE float InterpolateCosine(float a, float b, float t);
API_CORE float InterpolateCubic(float a, float b, float t);

//-----------------------------------//

NAMESPACE_EXTERN_END

// Clamps a variable to the given range.
template< typename T > bool MathClamp( T& var, T min = 0, T max = 1 )
{
	// Equivalent to: max(min, min(var, max))
	if( var >= max )
	{
		var = max;
		return true;
	}
	else if( var <= min )
	{
		var = min;
		return true;
	}

	return false;
}