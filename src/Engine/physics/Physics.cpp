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
#include "vapor/physics/Convert.h"
#include "vapor/physics/DebugDraw.h"

#include "vapor/Engine.h"
#include "vapor/render/Device.h"

#include <btBulletDynamicsCommon.h>

namespace vapor {

//-----------------------------------//

PhysicsManager::PhysicsManager()
	: enableSimulation(false)
	, debugDrawer(nullptr)
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
	delete debugDrawer;
    delete world;
    delete solver;
    delete dispatcher;
    delete config;
    delete broadphase;
}

//-----------------------------------//

void PhysicsManager::updateBody(const Body* body)
{
	if( !body )
		return;

	btRigidBody* bulletBody = body->getBulletBody();

	if( !bulletBody )
		return;

	world->synchronizeSingleMotionState(bulletBody);
}

//-----------------------------------//

void PhysicsManager::drawDebug()
{
	if( !world )
		return;

	debugDrawer->clearBuffer();
	
	world->debugDrawWorld();
	
	debugDrawer->getVertexBuffer()->forceRebuild();

	Engine* engine = Engine::getInstancePtr();
	RenderDevice* renderDevice = engine->getRenderDevice();

	RenderState state;
	state.renderable = debugDrawer->getRenderable();

	renderDevice->render( state, LightQueue() );
}

//-----------------------------------//

void PhysicsManager::createWorld()
{
	assert( !world );

    world = new btDiscreteDynamicsWorld(
		dispatcher, broadphase, solver, config);

	debugDrawer = new BulletDebugDrawer();
	world->setDebugDrawer( debugDrawer );

	setWorldGravity( Vector3(0, -10, 0) );
}

//-----------------------------------//

void PhysicsManager::setWorldGravity(const Vector3& gravity)
{
	if( !world )
		return;

	world->setGravity(Convert::toBullet(gravity));
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
