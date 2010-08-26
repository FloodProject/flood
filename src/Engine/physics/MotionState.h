/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

namespace vapor {

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
	{ }

	~BodyMotionState()
	{ }

    virtual void getWorldTransform(btTransform& worldTransform) const
	{
		worldTransform = Convert::toBullet(transform);
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

} // end namespace