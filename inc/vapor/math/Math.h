/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#pragma once

#include "math/Vector3.h"

namespace vapor {

//-----------------------------------//

namespace Limits
{
	const float FloatMinimum = -std::numeric_limits<float>::max();
	const float FloatMaximum =  std::numeric_limits<float>::max();
	const float FloatEpsilon =  std::numeric_limits<float>::epsilon();
}

//-----------------------------------//

namespace Interpolate
{
	// Linear interpolation.
	float CORE_API linear(float a, float b, float t);

	// Cosine interpolation.
	float CORE_API cosine(float a, float b, float t);

	// Cubic interpolation.
	float CORE_API cubic(float a, float b, float t);
}

namespace Math
{
	// The value of Pi.
	const float PI = 3.141592654f;

	// Compares float values for equality.
	bool CORE_API floatEqual(float a, float b);

	// Calculates the logarithm of a given base.
	float CORE_API logBase( float base, float value );

	// Returns a random value in the given interval.
	float CORE_API random( float min, float max );

	// Converts from degrees to radians.
	float CORE_API degreeToRadian( float degree );

	// Converts from radians to degrees.
	float CORE_API radianToDegree( float radian );

	// Clamps a variable to the given range.
	template< typename T >
	bool clamp( T& var, const T min = 0, const T max = 1 )
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
}

// Calculate the normal from a triangle vertices.
Vector3 CORE_API calculateTriangleNormal( const Vector3& v1, 
	const Vector3& v2, const Vector3& v3 );

//-----------------------------------//

} // end namespace