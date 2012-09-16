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

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Axis-aligned Bounding Box.
 */

struct API_CORE BoundingBox
{
	BoundingBox();
	BoundingBox( const Vector3& min, const Vector3& max );
	BoundingBox( const BoundingBox& box );

	// Expands the bounding box to contain the vertex.
	void add( const Vector3& vertex );

	// Expands the bounding box to contain the bounding box.
	void add( const BoundingBox& box );

	// Resets the bounding box.
	void reset();

	// Sets the box to zero.
	void setZero();

	// Gets if the box is infinite.
	bool isInfinite() const;

	// Gets the center point of the bounding box.
	Vector3 getCenter() const;

	// Gets the chosen corner vertice.
	Vector3 getCorner( int index ) const;

	// Intersection with a ray.
	bool intersects( const Ray& ray, float& distance ) const;
	
	// Transforms the bounding box by a matrix.
	BoundingBox transform( const Matrix4x3& ) const;

	Vector3 min;
	Vector3 max;
};

//-----------------------------------//

NAMESPACE_CORE_END
