/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/Vector3.h"

namespace vapor {

//-----------------------------------//

/**
 * Representation of an orientation as a set of Euler angles that can be
 * transformed into and back in a rotation matrix and quaternions.
 */

class CORE_API EulerAngles
{
public:

	EulerAngles();
	EulerAngles( float x, float y, float z );

	// Copy constructor.
	EulerAngles(const EulerAngles&);

	// Addition combined.
	EulerAngles& operator += (const EulerAngles&);

	// Addition combined.
	EulerAngles& operator += (const Vector3&);

	// Adds the given angles to the existing angles.
	void rotate( float x, float y, float z );

	// Resets the rotations so they don't have any effect.
	void identity();

public:

	float x, y, z;
};

//-----------------------------------//

} // end namespace