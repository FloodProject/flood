/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Vector.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Ray.
 */

struct API_CORE Ray
{
	Ray();
	Ray( const Vector3& origin, const Vector3& direction );
	Ray( const Ray& ray );

	// Gets a point on the ray.
	Vector3 getPoint( float distance ) const;

	// Intersects the ray with a triangle.
	bool intersects( const Vector3 (&tri)[3], float& t, float& u, float& v ) const;

	// Point representing the origin of the ray.
	Vector3 origin;

	// Vector representing the direction of the ray.
	Vector3 direction;
};

//-----------------------------------//

NAMESPACE_CORE_END