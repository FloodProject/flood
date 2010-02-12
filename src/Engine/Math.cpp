/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "vapor/math/Math.h"

namespace vapor { namespace math {

//-----------------------------------//

float logn( float base, float value )
{
	return log( value ) / log( base );
}

//-----------------------------------//

float random( float min, float max )
{
	return ((max - min) 
		* ( (float(rand()) / RAND_MAX ) ) 
		+ min);
}

//-----------------------------------//

float degreeToRadian( float degree )
{
	return degree * PI / 180.0f;
}

//-----------------------------------//

float radianToDegree( float radian )
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

} } // end namespaces