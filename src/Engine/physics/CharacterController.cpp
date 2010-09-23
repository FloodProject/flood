/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/physics/CharacterController.h"
#include "vapor/physics/Shape.h"
#include "vapor/physics/Convert.h"
#include "vapor/physics/Physics.h"
#include "vapor/physics/btKinematicController.h"

#include "vapor/math/Math.h"
#include "vapor/scene/Node.h"

#include "vapor/Engine.h"
#include "vapor/input/InputManager.h"
#include "vapor/input/Keyboard.h"

#include <BulletCollision/CollisionShapes/btConvexShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h> 
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(CharacterController, Controller)
	FIELD_PRIMITIVE(CharacterController, float, stepHeight)
	FIELD_PRIMITIVE(CharacterController, float, walkVelocity)
	FIELD_PRIMITIVE(CharacterController, float, fallSpeed)
	FIELD_PRIMITIVE(CharacterController, float, jumpSpeed)
	FIELD_PRIMITIVE(CharacterController, float, maxJumpHeight)
	FIELD_PRIMITIVE(CharacterController, float, maxSlope)
END_CLASS()

//-----------------------------------//

CharacterController::CharacterController()
	: controller(nullptr)
	, stepHeight(0.3f)
	, walkVelocity(8.0f)
	, fallSpeed(1.0f)
	, jumpSpeed(1.0f)
	, maxJumpHeight(1.0f)
	, maxSlope(45)
{
	Class& klass = getType();
	klass.onFieldChanged += fd::bind(&CharacterController::onFieldChanged, this);
}

//-----------------------------------//

CharacterController::~CharacterController()
{
	Class& klass = getType();
	klass.onFieldChanged -= fd::bind(&CharacterController::onFieldChanged, this);

	Engine* engine = Engine::getInstancePtr();
	PhysicsManager* physics = engine->getPhysicsManager();
	
	btDiscreteDynamicsWorld* world = physics->getWorld();
	
	if( ghostObject )	
		world->removeCollisionObject(ghostObject);

	if( controller )
		world->removeAction(controller);

	delete ghostObject;
	delete controller;
}

//-----------------------------------//

void CharacterController::_update( double delta )
{
	if( !createController() )
		return;

	Engine* engine = Engine::getInstancePtr();
	PhysicsManager* physics = engine->getPhysicsManager();
	btDiscreteDynamicsWorld* world = physics->getWorld();

	controller->updateAction(world, delta);
	updateCharacterTransform();

	InputManager* input = engine->getInputManager();
	Keyboard* keyboard = input->getKeyboard();
	
	btVector3 walkDirection;
	walkDirection.setZero();

	if( keyboard->isKeyPressed(Keys::I) )
		walkDirection += Convert::toBullet(  Vector3::UnitZ );
	
	if( keyboard->isKeyPressed(Keys::K) )
		walkDirection += Convert::toBullet( -Vector3::UnitZ );

	if( keyboard->isKeyPressed(Keys::J) )
		walkDirection += Convert::toBullet(  Vector3::UnitX );

	if( keyboard->isKeyPressed(Keys::L) )
		walkDirection += Convert::toBullet( -Vector3::UnitX );
	
	if( keyboard->isKeyPressed(Keys::Space) && controller->canJump() )
		controller->jump();

	btScalar walkSpeed = walkVelocity * delta;
	controller->setWalkDirection(walkDirection * walkSpeed);
}

//-----------------------------------//

void CharacterController::updateBulletTransform()
{
	TransformPtr transform = getNode()->getTransform();
	ghostObject->setWorldTransform(Convert::toBullet(transform));
}

//-----------------------------------//

void CharacterController::updateCharacterTransform()
{
	const btTransform& bullet = ghostObject->getWorldTransform();
	TransformPtr transform = getNode()->getTransform();
	Convert::fromBullet(bullet, transform);
}

//-----------------------------------//

bool CharacterController::createController()
{
	if( controller )
		return true;

	const NodePtr& node = getNode();
	ShapePtr shape = node->getTypedComponent<Shape>();

	if( !shape )
		return false;

	//if( !shape->getBulletShape()->isConvex() )
		//return false;

	btConvexShape* actualShape = (btConvexShape*) shape->getBulletShape();

	if( !actualShape )
		return false;

	ghostObject = new btPairCachingGhostObject();
	ghostObject->setCollisionShape( actualShape );
	ghostObject->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
	ghostObject->setActivationState( DISABLE_DEACTIVATION );

	controller = new btKinematicController(ghostObject, actualShape, stepHeight);

	updateBulletTransform();
	//updateProperties();

	Engine* engine = Engine::getInstancePtr();
	PhysicsManager* physics = engine->getPhysicsManager();
	btDiscreteDynamicsWorld* world = physics->getWorld();

	world->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
	world->addAction(controller);

	return true;
}

//-----------------------------------//

void CharacterController::updateProperties()
{
	if( !controller )
		return;

	controller->setFallSpeed( fallSpeed );
	controller->setJumpSpeed( jumpSpeed );
	controller->setMaxJumpHeight( maxJumpHeight );
	controller->setMaxSlope( Math::degreeToRadian(maxSlope) );
}

//-----------------------------------//

void CharacterController::onFieldChanged(const Field& field)
{
	updateProperties();
}

//-----------------------------------//

} // end namespace