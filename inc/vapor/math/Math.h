/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include <cmath>

namespace vapor { namespace math {

//-----------------------------------//

using ::sinf;
using ::asinf;
using ::cosf;
using ::acosf;
using ::tanf;
using ::sqrt;
using ::pow;
using ::log;
using ::log10;
using ::acos;
using ::atan2;

//-----------------------------------//

const float PI = 3.141592654f;

//-----------------------------------//

float logn( float base, float value );

//-----------------------------------//

float random( float min, float max );

//-----------------------------------//

float degreeToRadian( float degree );
float radianToDegree( float radian );

//-----------------------------------//

template< typename T >
void limit( T& var, const T min, const T max )
{
	if( var >= max )
		var = max;
	else if( var <= min )
		var = min;
}

//-----------------------------------//

// Calculate the normal from a triangle vertices.
Vector3 calculateTriangleNormal( const Vector3& v1, 
	const Vector3& v2, const Vector3& v3 );

//-----------------------------------//

} } // end namespaces