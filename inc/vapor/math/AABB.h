/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Utilities.h"
#include "vapor/math/Vector3.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Ray.h"

namespace vapor {

//-----------------------------------//

/**
 * Axis-aligned Bounding Box.
 */

class VAPOR_API AABB : private boost::noncopyable
{
public:

	AABB();
	AABB( const Vector3& min, const Vector3& max );
	AABB( const AABB& box );

	// Gets the chosen corner vertice.
	Vector3 getCorner( int index ) const;

	// Resets the bounding box.
	void reset();

	// Gets the center point of the bounding box.
	Vector3 getCenter() const;

	// Gets the minimum corner of the bounding box.
	GETTER(Minimum, const Vector3&, min)

	// Gets the maximum corner of the bounding box.
	GETTER(Maximum, const Vector3&, max)
    
	// Gets the size of the bounding box.
	//Vector3 getSize() const;

	// Expands the bounding box to contain the vertex.
	void add( const Vector3& vertex );

	// Expands the bounding box to contain the bounding box.
	void add( const AABB& aabb );

	// Intersection with a ray.
	bool intersects( const Ray& ray, float& distance ) const;
	
	// Transforms the AABB by a matrix.
	AABB transform( const Matrix4x3& mat ) const;

public:

	Vector3 min;
	Vector3 max;
};

//-----------------------------------//

} // end namespace
