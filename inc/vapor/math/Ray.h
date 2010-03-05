/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"

namespace vapor { namespace math {

//-----------------------------------//

/**
 * Ray class.
 */

class VAPOR_API Ray : private boost::noncopyable
{
public:

	Ray( Vector3 origin, Vector3 direction );
	Ray( const Ray& ray );

	// Gets a point with the origin of the ray.
	const Vector3& getOrigin() const;

	// Gets a vector with the direction of the ray.
	const Vector3& getDirection() const;

	// Intersects the ray with a triangle.
	bool intersects( const Vector3 tri[3], 
		Vector3& intersectionPoint, float& t ) const;

public:

	// Point representing the origin of the ray.
	Vector3 origin;

	// Vector representing the direction of the ray.
	Vector3 direction;
};

//-----------------------------------//

} } // end namespaces

