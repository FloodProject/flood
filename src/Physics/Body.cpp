/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Core/Event.h"

#ifdef ENABLE_PHYSICS_BULLET

#include "Physics/Body.h"
#include "Physics/Shape.h"
#include "Physics/Physics.h"
#include "Physics/Convert.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"

#include "Engine/Engine.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include "MotionState.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Body, Component)
	FIELD_PRIMITIVE(1, float, mass)
	FIELD_PRIMITIVE(2, float, friction)
	FIELD_PRIMITIVE(3, float, restitution)
REFLECT_CLASS_END()

//-----------------------------------//

Body::Body()
	: body(nullptr)
	, motionState(nullptr)
	, mass(50)
	, friction(0.5f)
	, restitution(0.3f)
{
}

//-----------------------------------//

Body::~Body()
{
	TransformPtr transform = getEntity()->getTransform();
	
	if(transform)
		transform->onTransform.Disconnect( this, &Body::onTransform );

	removeWorld();

	Deallocate(motionState);
	Deallocate(body);
}

//-----------------------------------//

void Body::update( float delta )
{
	if( body ) return;
	
	createBody();
	
	TransformPtr transform = getEntity()->getTransform();
	transform->onTransform.Connect( this, &Body::onTransform);
}

//-----------------------------------//

void Body::onTransform()
{
	if( !body ) return;

	if( motionState->ignoreTransform )
	{
		motionState->ignoreTransform = false;
		return;
	}

	TransformPtr transform = getEntity()->getTransform();
	const Vector3& scale = transform->getScale();

	btCollisionShape* shape = getBulletShape();
	shape->setLocalScaling(Convert::toBullet(scale));

	body->setWorldTransform(Convert::toBullet(transform));
	body->activate();
}

//-----------------------------------//

btCollisionShape* Body::getBulletShape() const
{
	ShapePtr shape = entity->getComponentFromFamily<Shape>();
	if( !shape ) return nullptr;

	return shape->getBulletShape();
}

//-----------------------------------//

bool Body::createBody()
{
	if( body ) return true;

	btCollisionShape* bulletShape = getBulletShape();
	if( !bulletShape ) return false;

	motionState = new BodyMotionState( entity->getTransform() );

	btVector3 localInertia;
	localInertia.setZero();
		
	if( isDynamic() )
		bulletShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo info(
		mass, motionState, bulletShape, localInertia);

	body = AllocateThis(btRigidBody, info);

	updateProperties();

	addWorld();

	return true;
}

//-----------------------------------//

void Body::addWorld()
{
	Engine* engine = GetEngine();
	
	PhysicsManager* physics = engine->getPhysicsManager();
	if( !physics ) return;
	
	physics->addRigidBody(this);
}

//-----------------------------------//

void Body::removeWorld()
{
	Engine* engine = GetEngine();

	PhysicsManager* physics = engine->getPhysicsManager();
	if( !physics ) return;
	
	physics->removeRigidBody(this);
}

//-----------------------------------//

void Body::updateProperties()
{
	if( !body ) return;

	btCollisionShape* bulletShape = getBulletShape();
	if( !bulletShape ) return;

	btVector3 localInertia;
	localInertia.setZero();
		
	if( isDynamic() )
		bulletShape->calculateLocalInertia(mass, localInertia);

	body->setMassProps(mass, localInertia);
	body->setFriction(friction);
	body->setRestitution(restitution);
}

//-----------------------------------//

bool Body::isDynamic() const
{
	return mass != 0;
}

//-----------------------------------//

void Body::onFieldChanged(const Field& field)
{
	updateProperties();
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif