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

namespace vapor {

//-----------------------------------//

namespace Convert
{
	static Vector3 fromBullet( const btVector3& vec )
	{
		return Vector3(
			vec.x(),
			vec.y(),
			vec.z() );
	}

	static btVector3 toBullet(const Vector3& vec )
	{
		return btVector3(
			vec.x,
			vec.y,
			vec.z );
	}

	static btVector3 toBullet(const AABB& box)
	{
		Vector3 extents = box.max-box.getCenter();
		return toBullet(extents);
	}
}

//-----------------------------------//

} // end namespace