/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/physics/Convert.h"
#include "vapor/physics/MeshShape.h"

#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"

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

btVector3 Convert::toBullet(const Vector3& vec )
{
	return btVector3(
		vec.x,
		vec.y,
		vec.z );
}

//-----------------------------------//

btVector3 Convert::toBullet(const BoundingBox& box)
{
	Vector3 extents = box.max-box.getCenter();
	return toBullet(extents);
}

//-----------------------------------//

Vector3 getOffset(const NodePtr& node)
{
	ShapePtr shape = node->getTypedComponent<Shape>();

	if( shape->getInstanceType().is<MeshShape>() )
		return Vector3::Zero;

	const TransformPtr& transform = node->getTransform();
	const BoundingBox& box = transform->getBoundingVolume();
	const Vector3& scale = transform->getScale();

	Vector3 offset = (box.max - box.min) / 2;
	offset = (offset + box.min) * scale;

	return offset;
}

//-----------------------------------//

void Convert::fromBullet( const btTransform& bullet, const TransformPtr& transform )
{
	Vector3 offset = getOffset(transform->getNode());

	Vector3 origin = fromBullet( bullet.getOrigin() );
	transform->setPosition( origin-offset );

	EulerAngles rot;
	bullet.getBasis().getEulerZYX(rot.z, rot.y, rot.x);
	transform->setRotation(rot);
}

//-----------------------------------//

btTransform Convert::toBullet(const TransformPtr& transform)
{
	const Vector3& pos = transform->getPosition();
	Vector3 offset = getOffset( transform->getNode() );
	
	const Matrix4x3& abs = transform->getAbsoluteTransform();
	Matrix4x4 trs = Matrix4x4(abs);

	btTransform startTransform;

	trs.m11 = 1;
	trs.m22 = 1;
	trs.m33 = 1;

	startTransform.setFromOpenGLMatrix(&trs.m11);
	startTransform.setOrigin(Convert::toBullet(pos+offset));

	return startTransform;
}

//-----------------------------------//

} // end namespace
