/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"

namespace vapor { namespace math {

//-----------------------------------//

/**
 * Ray class.
 */

class VAPOR_API Ray
{
public:

	Ray( Vector3 origin, Vector3 direction );

	// Gets a point with the origin of the ray.
	const Vector3& getOrigin() const;

	// Gets a vector with the direction of the ray.
	const Vector3& getDirection() const;

public:

	// Point representing the origin of the ray.
	Vector3 origin;

	// Vector representing the direction of the ray.
	Vector3 direction;
};

//-----------------------------------//

} } // end namespaces

