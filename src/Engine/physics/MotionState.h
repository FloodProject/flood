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
		, body(nullptr)
		, ignoreTransform(false)
	{ }

	~BodyMotionState()
	{ }

    virtual void getWorldTransform(btTransform& worldTransform) const
	{
		const Matrix4x3& abs = transform->getAbsoluteTransform();
		Matrix4x4 trs = Matrix4x4(abs);

		trs.m11 = 1;
		trs.m22 = 1;
		trs.m33 = 1;

		worldTransform.setFromOpenGLMatrix(&trs.m11);
    }

    virtual void setWorldTransform(const btTransform& worldTransform)
	{
		Vector3 origin = Convert::fromBullet(worldTransform.getOrigin());
		transform->setPosition(origin);

		EulerAngles rot;
		worldTransform.getBasis().getEulerZYX(rot.z, rot.y, rot.x);

		transform->setRotation(rot);

		ignoreTransform = true;
    }

	// Object transform.
	TransformPtr transform;

	// Bullet rigid body.
	btRigidBody* body;

	bool ignoreTransform;
};

//-----------------------------------//

} // end namespace