/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include <cmath>

namespace vapor {

//-----------------------------------//

const float PI = 3.141592654f;

//-----------------------------------//

bool VAPOR_API fcmp(float a, float b);

//-----------------------------------//

float VAPOR_API logn( float base, float value );

//-----------------------------------//

float VAPOR_API random( float min, float max );

//-----------------------------------//

float VAPOR_API degreeToRadian( float degree );
float VAPOR_API radianToDegree( float radian );

//-----------------------------------//

template< typename T >
bool clamp( T& var, const T min, const T max )
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

//-----------------------------------//

// Calculate the normal from a triangle vertices.
Vector3 VAPOR_API calculateTriangleNormal( const Vector3& v1, 
	const Vector3& v2, const Vector3& v3 );

//-----------------------------------//

} // end namespace