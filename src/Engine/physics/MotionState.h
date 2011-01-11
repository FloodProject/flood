/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "vapor/physics/MeshShape.h"

namespace vapor {

//-----------------------------------//

static Vector3 getOffset(const EntityPtr& node)
{
	ShapePtr shape = node->getTypedComponent<Shape>();

	if( shape->getType().is<MeshShape>() )
		return Vector3::Zero;

	const TransformPtr& transform = node->getTransform();
	const BoundingBox& box = transform->getWorldBoundingVolume();

	return box.max - box.getCenter();
}

//-----------------------------------//

/**
 * This class synchronizes the transform between Bullet and the object.
 */

class BodyMotionState : public btMotionState
{
public:

	BodyMotionState(const TransformPtr& transform)
		: transform(transform)
		, ignoreTransform(false)
	{
		offset = getOffset(transform->getEntity()) * Vector3::UnitY;
		//centerOffset.setOrigin( Convert::toBullet(offset) );
	}

	~BodyMotionState()
	{ }

    virtual void getWorldTransform(btTransform& worldTransform) const
	{
		worldTransform = Convert::toBullet(transform);
		//worldTransform.setOrigin( worldTransform.getOrigin() + Convert::toBullet(offset) );
    }

    virtual void setWorldTransform(const btTransform& worldTransform)
	{
		Convert::fromBullet(worldTransform, transform);
		ignoreTransform = true;
    }

	Vector3 offset;

	// Object transform.
	TransformPtr transform;

	// Ignores next transform notification.
	bool ignoreTransform;
};

//-----------------------------------//

} // end namespace