/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_PHYSICS_BULLET

#include "Physics/Convert.h"
#include "Physics/MeshShape.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Vector3 Convert::fromBullet( const btVector3& vec )
{
	return Vector3(
		vec.x(),
		vec.y(),
		vec.z() );
}

//-----------------------------------//

Quaternion Convert::fromBullet( const btQuaternion& quat )
{
	return Quaternion(
		quat.x(),
		quat.y(),
		quat.z(),
		quat.w() );
}

//-----------------------------------//

btVector3 Convert::toBullet(const Vector3& vec )
{
	return btVector3(
		vec.x,
		vec.y,
		vec.z );
}

//-----------------------------------//

btQuaternion Convert::toBullet(const Quaternion& quat )
{
	return btQuaternion(
		quat.x,
		quat.y,
		quat.z,
		quat.w );
}

//-----------------------------------//

btVector3 Convert::toBullet(const BoundingBox& box)
{
	Vector3 extents = box.max - box.getCenter();
	return toBullet(extents);
}

//-----------------------------------//

void Convert::fromBullet( const btTransform& bullet, const TransformPtr& transform )
{
	transform->setPosition( fromBullet(bullet.getOrigin()) );
	transform->setRotation( fromBullet(bullet.getRotation()) );
}

//-----------------------------------//

btTransform Convert::toBullet(const TransformPtr& transform)
{
	btTransform startTransform;
	startTransform.setOrigin( toBullet(transform->getPosition()) );
	startTransform.setRotation( toBullet(transform->getRotation()) );

	return startTransform;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif