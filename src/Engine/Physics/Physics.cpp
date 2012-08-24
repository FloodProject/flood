/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_PHYSICS_BULLET

#include "Physics/Physics.h"
#include "Physics/Body.h"
#include "Physics/Convert.h"
#include "Physics/DebugDraw.h"
#include "Graphics/RenderDevice.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h> 

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

PhysicsManager::PhysicsManager()
	: enableSimulation(false)
	, debugDrawer(nullptr)
	, drawDebug(true)
	, config(nullptr)
	, broadphase(nullptr)
	, dispatcher(nullptr)
	, solver(nullptr)
	, world(nullptr)
{
	config = AllocateThis(btDefaultCollisionConfiguration);
	broadphase = AllocateThis(btDbvtBroadphase);
	dispatcher = AllocateThis(btCollisionDispatcher, config);
	solver = AllocateThis(btSequentialImpulseConstraintSolver);
}

//-----------------------------------//

PhysicsManager::~PhysicsManager()
{
	Deallocate(debugDrawer);
	Deallocate(world);
	Deallocate(solver);
	Deallocate(dispatcher);
	Deallocate(config);
	Deallocate(broadphase);
}

//-----------------------------------//

void PhysicsManager::updateBody(const Body* body)
{
	if( !body ) return;

	btRigidBody* bulletBody = body->getBulletBody();
	if( !bulletBody ) return;

	world->synchronizeSingleMotionState(bulletBody);
}

//-----------------------------------//

void PhysicsManager::drawDebugWorld()
{
	if( !world ) return;
	if( !drawDebug ) return;

	debugDrawer->clearBuffer();
	world->debugDrawWorld();

	RenderDevice* renderDevice = GetRenderDevice();

	RenderState state( debugDrawer->getRenderable().get() );
	renderDevice->render( state, LightQueue() );
}

//-----------------------------------//

void PhysicsManager::createWorld()
{
	assert( !world );

	world = AllocateThis(btDiscreteDynamicsWorld, dispatcher, broadphase, solver, config);

#if 0
	// Enable collision callbacks for ghost objects.
	btOverlappingPairCache* cache = world->getPairCache();
	cache->setInternalGhostPairCallback( new btGhostPairCallback() );
#endif

#ifdef ENABLE_PHYSICS_DEBUG
	debugDrawer = AllocateThis(BulletDebugDrawer);
	world->setDebugDrawer( debugDrawer );
#endif

	setWorldGravity( Vector3(0, -10, 0) );
}

//-----------------------------------//

void PhysicsManager::setWorldGravity(const Vector3& gravity)
{
	if( !world ) return;
	world->setGravity(Convert::toBullet(gravity));
}

//-----------------------------------//

void PhysicsManager::update( float delta )
{
	if( !world ) return;
	if( !enableSimulation ) return;

	world->stepSimulation((btScalar) delta);
}

//-----------------------------------//

void PhysicsManager::addRigidBody( const Body* body )
{
	assert( world != nullptr );

	if( !body ) return;

	btRigidBody* bulletBody = body->getBulletBody();
	if( !bulletBody ) return;

	world->addRigidBody(bulletBody);
}

//-----------------------------------//

void PhysicsManager::removeRigidBody( const Body* body )
{
	assert( world != nullptr );

	if( !body ) return;

	btRigidBody* bulletBody = body->getBulletBody();
	if( !bulletBody ) return;

	world->removeRigidBody(bulletBody);
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif