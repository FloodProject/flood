/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"

namespace vapor {
	namespace math {

//-----------------------------------//

/**
 * Axis-aligned Bounding Box.
 */

class VAPOR_API AABB
{
public:

	AABB();

	// Gets the chosen corner vertice.
	Vector3 getCorner( int index ) const;

	// Resets the bounding box.
	void reset();

	// Gets the minimum corner of the bounding box.
	const Vector3& getMinimum() const;

	// Gets the maximum corner of the bounding box.
	const Vector3& getMaximum() const;
    
	// Gets the size of the bounding box.
	//Vector3 getSize() const;

	// Expands the bounding box to contain the vertex.
	void add( const Vector3& vertex );

	// Expands the bounding box to contain the bounding box.
	void add( const AABB& aabb );

	// Intersection with a ray
	// bool intersects( const Ray& ray, float& distance );

public:

	Vector3 min;
	Vector3 max;
};

//-----------------------------------//

} } // end namespaces
