/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Matrix4x3.h"
#include "Core/Math/Ray.h"
#include "Core/Math/BoundingBox.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Bounding Sphere.
 */

struct API_CORE BoundingSphere
{
	BoundingSphere();
	BoundingSphere( const Vector3& center, float radius );
	BoundingSphere( const BoundingSphere& bs );
	BoundingSphere( const BoundingBox& box );

	// Adds a point to the bounding sphere.
	//void add( const Vector3& vertex );

	// Intersection with a ray.
	bool intersects( const Ray& ray, float& distance ) const;

	// Intersection with a sphere.
	bool intersects( const BoundingSphere& sphere ) const;

	// Intersection with a point.
	bool intersects( const Vector3& pt ) const;
	
	Vector3 center;
	float radius;
};

//-----------------------------------//

NAMESPACE_CORE_END
