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
using ::ceil;

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
void limit( T& var, const T min, const T max )
{
	if( var >= max )
		var = max;
	else if( var <= min )
		var = min;
}

//-----------------------------------//

// Calculate the normal from a triangle vertices.
Vector3 VAPOR_API calculateTriangleNormal( const Vector3& v1, 
	const Vector3& v2, const Vector3& v3 );

//-----------------------------------//

} } // end namespaces