/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/node.h"

#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Vector3.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/math/AABB.h"

#include "vapor/physics/Physics.h"

class hkpRigidBodyCinfo;
class hkpBoxShape;
class hkVector4;
class hkpRigidBody;
namespace hkpMotion { enum MotionType; }

namespace vapor { namespace scene {

//-----------------------------------//

class VAPOR_API Body : public Component
{
public:

	Body();
	Body(float mass, hkpMotion::MotionType motion);
	virtual ~Body();

	// Inits the physics body.
	void init();

	// Gets the type of this component. 
	virtual const std::string& getType() const;

	// Called once per frame to update the component.
	virtual void update( double delta );

	void addToWorld();
	void removeFromWorld();
	void setLinearVelocity(const math::Vector3 &lVel);
	void setAngularVelocity(const math::Vector3 &aVel); 
	void applyForce(const math::Vector3 &force);
	void applyTorque(const math::Vector3 &torque);
	void applyLinearImpulse(const math::Vector3 &imp);
	bool initiated;

	void  setPosition (const math::Vector3 &position); 
	void  setPositionAndRotation (const math::Matrix4x3 &m); 

private:
	
	void setTransform(hkpRigidBodyCinfo& info);
	hkpBoxShape* getShape(const math::AABB &bb);
	hkVector4 convertVector(const math::Vector3 &v);
	hkpRigidBody * body;
	Transform* transform;
	physics::PhysicsManager* physicsManager;
	bool inWorld;
	bool firstUpdate;

	hkpRigidBodyCinfo* info;
	hkpBoxShape* shape;

protected:

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Body );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Body );

//-----------------------------------//

} } // end namespaces
