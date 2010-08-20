/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/physics/Physics.h"
#include "vapor/physics/Body.h"

#include "btBulletDynamicsCommon.h"

namespace vapor {

//-----------------------------------//

PhysicsManager::PhysicsManager()
	: enableSimulation(false)
	, config(nullptr)
	, broadphase(nullptr)
	, dispatcher(nullptr)
	, solver(nullptr)
	, world(nullptr)
{
	config = new btDefaultCollisionConfiguration();
    broadphase = new btDbvtBroadphase();
	dispatcher = new btCollisionDispatcher(config);
	solver = new btSequentialImpulseConstraintSolver;
}

//-----------------------------------//

PhysicsManager::~PhysicsManager()
{
    delete world;
    delete solver;
    delete dispatcher;
    delete config;
    delete broadphase;
}

//-----------------------------------//

btVector3 toBulletVector(const Vector3& vec)
{
	return btVector3(
		vec.x,
		vec.y,
		vec.z );
}

//-----------------------------------//

void PhysicsManager::createWorld()
{
	assert( !world );

    world = new btDiscreteDynamicsWorld(
		dispatcher, broadphase, solver, config);
	
	setWorldGravity( Vector3(0, -10, 0) );
}

//-----------------------------------//

void PhysicsManager::setWorldGravity(const Vector3& gravity)
{
	if( !world )
		return;

	world->setGravity(toBulletVector(gravity));
}

//-----------------------------------//

void PhysicsManager::update( double delta )
{
	if( !world )
		return;

	if( !enableSimulation )
		return;

	world->stepSimulation(delta);
}

//-----------------------------------//

void PhysicsManager::addRigidBody( const Body* body )
{
	assert( world != nullptr );

	if( !body )
		return;

	btRigidBody* bulletBody = body->getBulletBody();

	if( !bulletBody )
		return;

	world->addRigidBody(bulletBody);
}

//-----------------------------------//

void PhysicsManager::removeRigidBody( const Body* body )
{
	assert( world != nullptr );

	if( !body )
		return;

	btRigidBody* bulletBody = body->getBulletBody();

	if( !bulletBody )
		return;

	world->removeRigidBody(bulletBody);
}

//-----------------------------------//

} // end namespace
