/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_PHYSICS_BULLET

#include "Physics/Convert.h"
#include "Physics/MeshShape.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace vapor {

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

static Vector3 getOffset(Entity* entity)
{
	ShapePtr shape = entity->getComponentFromFamily<Shape>();

	if( ReflectionIsEqual(shape->getType(), ReflectionGetType(MeshShape)) )
		return Vector3::Zero;

	const TransformPtr& transform = entity->getTransform();
	const BoundingBox& box = transform->getWorldBoundingVolume();

	return box.max - box.getCenter();
}

//-----------------------------------//

void Convert::fromBullet( const btTransform& bullet, const TransformPtr& transform )
{
	Vector3 offset = getOffset(transform->getEntity()) * Vector3::UnitY;
	transform->setPosition( fromBullet(bullet.getOrigin()) - offset );
	transform->setRotation( fromBullet(bullet.getRotation()) );
}

//-----------------------------------//

btTransform Convert::toBullet(const TransformPtr& transform)
{
	Vector3 offset = getOffset(transform->getEntity()) * Vector3::UnitY;
	
	btTransform startTransform;
	startTransform.setOrigin( toBullet(transform->getPosition() + offset) );
	startTransform.setRotation( toBullet(transform->getRotation()) );

	return startTransform;
}

//-----------------------------------//

} // end namespace

#endif