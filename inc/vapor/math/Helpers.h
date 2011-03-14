/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#pragma once

#include "Math/Vector.h"

BEGIN_NAMESPACE_EXTERN

//-----------------------------------//

const float LimitsFloatMinimum = -std::numeric_limits<float>::max();
const float LimitsFloatMaximum =  std::numeric_limits<float>::max();
const float LimitsFloatEpsilon =  std::numeric_limits<float>::epsilon();
const float PI = 3.141592654f;

CORE_API bool  MathFloatCompare(float a, float b);
CORE_API float MathFloatRandom( float min, float max );
CORE_API float MathLogBase( float base, float value );

CORE_API float MathDegreeToRadian( float degree );
CORE_API float MathRadianToDegree( float radian );

CORE_API float InterpolateLinear(float a, float b, float t);
CORE_API float InterpolateCosine(float a, float b, float t);
CORE_API float InterpolateCubic(float a, float b, float t);

//-----------------------------------//

END_NAMESPACE_EXTERN

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