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
#include "vapor/physics/Convert.h"

#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"
#include "vapor/Engine.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include "MotionState.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Body, Component)
	FIELD_PRIMITIVE(Body, float, mass)
END_CLASS()

//-----------------------------------//

Body::Body()
	: body(nullptr)
	, motionState(nullptr)
	, mass(10)
{
	Class& klass = getType();
	klass.onFieldChanged += fd::bind(&Body::onFieldChanged, this);
}

//-----------------------------------//

Body::~Body()
{
	Class& klass = getType();
	klass.onFieldChanged -= fd::bind(&Body::onFieldChanged, this);

	TransformPtr transform = motionState->transform;
	transform->onTransform -= fd::bind(&Body::onTransform, this);

	removeWorld();

	delete motionState;
	delete body;
}

//-----------------------------------//

void Body::update( double delta )
{
	if( !body && !createBody() )
	{
		TransformPtr transform = getNode()->getTransform();
		transform->onTransform += fd::bind(&Body::onTransform, this);
		return;
	}
}

//-----------------------------------//

void Body::onTransform()
{
	if( !body )
		return;

	if( motionState->ignoreTransform )
	{
		motionState->ignoreTransform = false;
		return;
	}

	TransformPtr transform = getNode()->getTransform();
	const Vector3& scale = transform->getScale();

	btCollisionShape* shape = getBulletShape();
	shape->setLocalScaling(Convert::toBullet(scale));

	const Matrix4x3& abs = transform->getAbsoluteTransform();
	Matrix4x4 trs = Matrix4x4(abs);

	trs.m11 = 1;
	trs.m22 = 1;
	trs.m33 = 1;

	body->getWorldTransform().setFromOpenGLMatrix(&trs.m11);
	body->activate();
}

//-----------------------------------//

btCollisionShape* Body::getBulletShape() const
{
	const NodePtr& node = getNode();

	if( !node )
		return false;

	ShapePtr shape = node->getTypedComponent<Shape>();

	if( !shape )
		return false;

	return shape->getBulletShape();
}

//-----------------------------------//

bool Body::createBody()
{
	if( body )
		return true;

	btCollisionShape* bulletShape = getBulletShape();

	if( !bulletShape )
		return false;

	const NodePtr& node = getNode();

	if( !node )
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

void Body::onFieldChanged(const Field& field)
{
	if( !body )
		return;

	body->setMassProps(mass, btVector3());
}

//-----------------------------------//

} // end namespace
