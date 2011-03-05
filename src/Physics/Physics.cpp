/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_PHYSICS_BULLET

#include "physics/Physics.h"
#include "physics/Body.h"
#include "physics/Convert.h"
#include "physics/DebugDraw.h"
#include "render/Device.h"
#include "Engine.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h> 
//#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h> 

namespace vapor {

//-----------------------------------//

PhysicsManager::PhysicsManager()
	: enableSimulation(false)
	, debugDrawer(nullptr)
	, drawDebug(false)
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
	//btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
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

void PhysicsManager::drawDebugWorld()
{
	if( !world )
		return;

	if( !drawDebug )
		return;

	debugDrawer->clearBuffer();
	world->debugDrawWorld();

	RenderDevice* renderDevice = GetEngine()->getRenderDevice();

	RenderState state;
	state.renderable = debugDrawer->getRenderable();

	renderDevice->render( state, LightQueue() );
}

//-----------------------------------//

void PhysicsManager::createWorld()
{
	assert( !world );

    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);

	//btOverlappingPairCache* cache = world->getPairCache();
	//cache->setInternalGhostPairCallback( new btGhostPairCallback() );

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

void PhysicsManager::update( float delta )
{
	if( !world )
		return;

	if( !enableSimulation )
		return;

	world->stepSimulation( (btScalar) delta);
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

#endif