/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/BoundingBox.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>

FWD_DECL_SHARED(Transform)

namespace vapor {

//-----------------------------------//

namespace Convert
{
	// Converts vectors from Bullet.
	Vector3 fromBullet( const btVector3& vec );
	
	// Converts quaternions from Bullet.
	Quaternion fromBullet( const btQuaternion& quat );

	// Converts transform from Bullet.
	void fromBullet( const btTransform& bullet, const TransformPtr& transform );

	// Converts vectors to Bullet.
	btVector3 toBullet(const Vector3& vec );

	// Converts bounding boxes to Bullet.
	btVector3 toBullet(const BoundingBox& box);

	// Converts transforms to Bullet.
	btTransform toBullet(const TransformPtr& transform);
}

//-----------------------------------//

} // end namespace