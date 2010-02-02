/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Vector3.h"

namespace vapor { namespace math {

//-----------------------------------//

/**
 * Representation of an orientation as a set of Euler angles that can be
 * transformed into and back in a rotation matrix and quaternions.
 */

class VAPOR_API EulerAngles
{
public:

	EulerAngles();
	EulerAngles( float x, float y, float z );

	// Adds the given angles to the existing angles.
	void rotate( float x, float y, float z );

	// Reduces the angles to the cannonical range (0-360º).
	void canonize();

	// Resets the rotations so they don't have any effect.
	void identity();

	// Gets a orientation matrix represeting the rotation angles.
	Matrix4x3 getOrientationMatrix() const;

	// many more methods implemented here ...
	Matrix4x3 rotateX( float ang ) const;
	Matrix4x3 rotateY( float ang ) const;
	Matrix4x3 rotateZ( float ang ) const;

public:

	float xang, yang, zang;
};

//-----------------------------------//

} } // end namespaces