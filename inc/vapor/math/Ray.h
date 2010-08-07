/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"

namespace vapor {

//-----------------------------------//

/**
 * Ray class.
 */

class VAPOR_API Ray
{
public:

	Ray( const Vector3& origin, const Vector3& direction );
	Ray( const Ray& ray );

	// Gets a point on the ray.
	Vector3 getPoint( float distance ) const;

	// Intersects the ray with a triangle.
	bool intersects( const Vector3 tri[3], Vector3& intersection, float& t ) const;

public:

	// Point representing the origin of the ray.
	Vector3 origin;

	// Vector representing the direction of the ray.
	Vector3 direction;
};

//-----------------------------------//

} // end namespace

