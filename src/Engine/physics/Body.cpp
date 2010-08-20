/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/physics/Body.h"
#include "vapor/physics/Shape.h"
#include "vapor/physics/Physics.h"
#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"
#include "vapor/Engine.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace vapor {

//-----------------------------------//

Vector3 convertToVector3( const btVector3& vec )
{
	return Vector3(
		vec.x(),
		vec.y(),
		vec.z() );
}

//-----------------------------------//

class BodyMotionState : public btMotionState
{
public:

	BodyMotionState(const TransformPtr& transform)
		: transform(transform)
	{
		assert( transform != nullptr );
		
		transform->onTransform += fd::bind(&BodyMotionState::onTransform, this);
	}

	~BodyMotionState()
	{
		transform->onTransform -= fd::bind(&BodyMotionState::onTransform, this);
	}

    virtual void getWorldTransform(btTransform& worldTransform) const
	{
		const Matrix4x3& abs = transform->getAbsoluteTransform();
		const Matrix4x4& trs = Matrix4x4(abs);

		worldTransform.setFromOpenGLMatrix(&trs.m11);
    }

    virtual void setWorldTransform(const btTransform& worldTransform)
	{
		transform->setPosition( convertToVector3(worldTransform.getOrigin()) );

		EulerAngles rot;
		worldTransform.getBasis().getEulerZYX(rot.z, rot.y, rot.x);

		transform->setRotation(rot);
    }

    void onTransform()
	{
		Engine* engine = Engine::getInstancePtr();
		PhysicsManager* physics = engine->getPhysicsManager();

		if( physics->getSimulation() )
			return;

		if( !body )
			return;

		const Matrix4x3& abs = transform->getAbsoluteTransform();
		const Matrix4x4& trs = Matrix4x4(abs);

		body->getWorldTransform().setFromOpenGLMatrix(&trs.m11);
    }

    TransformPtr transform;
	btRigidBody* body;
};

//-----------------------------------//

BEGIN_CLASS_PARENT(Body, Component)
	FIELD_PRIMITIVE(Body, float, mass)
END_CLASS()

//-----------------------------------//

Body::Body()
	: body(nullptr)
	, motionState(nullptr)
	, mass(10)
{ }

//-----------------------------------//

Body::~Body()
{
	delete motionState;

	removeWorld();
	delete body;
}

//-----------------------------------//

void Body::update( double delta )
{
	if( !body && !createBody() )
		return;
}

//-----------------------------------//

bool Body::createBody()
{
	if( body )
		return true;

	const NodePtr& node = getNode();

	if( !node )
		return false;

	ShapePtr shape = node->getTypedComponent<Shape>();

	if( !shape )
		return false;

	btCollisionShape* bulletShape = shape->getBulletShape();

	if( !bulletShape )
		return false;

	motionState = new BodyMotionState( node->getTransform() );

	btRigidBody::btRigidBodyConstructionInfo info( mass, motionState, bulletShape );

	body = new btRigidBody(info);

	motionState->body = body;

	addWorld();

	return true;
}

//-----------------------------------//

void Body::addWorld()
{
	Engine* engine = Engine::getInstancePtr();
	PhysicsManager* physics = engine->getPhysicsManager();
	
	if( !physics )
		return;
	
	physics->addRigidBody(this);
}

//-----------------------------------//

void Body::removeWorld()
{
	Engine* engine = Engine::getInstancePtr();
	PhysicsManager* physics = engine->getPhysicsManager();
	
	if( !physics )
		return;
	
	physics->removeRigidBody(this);
}

//-----------------------------------//

} // end namespace
