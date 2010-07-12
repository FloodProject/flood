/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Ray.h"

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
