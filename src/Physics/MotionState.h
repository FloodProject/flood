/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "Physics/MeshShape.h"

NAMESPACE_ENGINE_BEGIN

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

	// Object transform.
	TransformPtr transform;

	// Ignores next transform notification.
	bool ignoreTransform;
};

//-----------------------------------//

NAMESPACE_ENGINE_END