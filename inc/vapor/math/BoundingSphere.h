/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/Vector3.h"
#include "math/Matrix4x3.h"
#include "math/Ray.h"

namespace vapor {

//-----------------------------------//

/**
 * Bounding Sphere.
 */

class VAPOR_API BoundingSphere
{
public:

	BoundingSphere();
	BoundingSphere( const Vector3& center, float radius );
	BoundingSphere( const BoundingSphere& bs );

	// Adds a point to the bounding sphere.
	//void add( const Vector3& vertex );

	// Intersection with a ray.
	bool intersects( const Ray& ray, float& distance ) const;

	// Intersection with a point.
	bool intersects( const Vector3& pt ) const;
	
public:

	Vector3 center;
	float radius;
};

//-----------------------------------//

} // end namespace
