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
	FIELD_PRIMITIVE(Body, float, friction)
	FIELD_PRIMITIVE(Body, float, restitution)
END_CLASS()

//-----------------------------------//

Body::Body()
	: body(nullptr)
	, motionState(nullptr)
	, mass(50)
	, friction(0.5f)
	, restitution(0.3f) 
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
	if( body )
		return;
	
	createBody();
	
	TransformPtr transform = getNode()->getTransform();
	transform->onTransform += fd::bind(&Body::onTransform, this);
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

	body->setWorldTransform(Convert::toBullet(transform));
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

bool Body::isDynamic() const
{
	return mass != 0;
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
	assert( node != nullptr );

	motionState = new BodyMotionState( node->getTransform() );

	btVector3 localInertia;
	localInertia.setZero();
		
	if( isDynamic() )
		bulletShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo info( mass, motionState,
		bulletShape, localInertia);

	body = new btRigidBody(info);

	updateProperties();

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

void Body::updateProperties()
{
	if( !body )
		return;

	btCollisionShape* bulletShape = getBulletShape();

	if( !bulletShape )
		return;

	btVector3 localInertia;
	localInertia.setZero();
		
	if( isDynamic() )
		bulletShape->calculateLocalInertia(mass, localInertia);

	body->setMassProps(mass, localInertia);
	body->setFriction(friction);
	body->setRestitution(restitution);
}

//-----------------------------------//

void Body::onFieldChanged(const Field& field)
{
	updateProperties();
}

//-----------------------------------//

} // end namespace
