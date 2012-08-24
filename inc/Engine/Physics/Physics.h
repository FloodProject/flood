/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_PHYSICS_BULLET

#include "Core/References.h"
#include "Engine/Subsystem.h"
#include "Math/Vector.h"

class btCollisionConfiguration;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

FWD_DECL_INTRUSIVE(Body)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class BulletDebugDrawer;

/**
 * Global hub for physics simulations.
 */

class API_ENGINE PhysicsManager : public Subsystem
{
public:

	PhysicsManager();
	virtual ~PhysicsManager();

	// Creates a new physics world.
	void createWorld();

	// Draws the debug of the world.
	void drawDebugWorld();

	// Updates the transform of the body.
	void updateBody(const Body* body);

	// Sets the gravity of the world.
	void setWorldGravity(const Vector3& gravity);
	
	// Updates the physical simulation.
	void update( float delta );

	// Gets/sets the physics simulation state.
	ACESSOR(Simulation, bool, enableSimulation);

	// Gets/sets the physics world debug state.
	ACESSOR(DebugWorld, bool, drawDebug);

	// Adds a rigid body to the physics world.
	void addRigidBody(const Body* body);

	// Removes a rigid body from the physics world.
	void removeRigidBody(const Body* body);

	// Gets the physics world.
	GETTER(World, btDiscreteDynamicsWorld*, world);

protected:

	// Keeps track of simulation state.
	bool enableSimulation;
	
	// Draws a debug of the physics world.
	BulletDebugDrawer* debugDrawer;

	// On if we should draw the debug of the physics world.
	bool drawDebug;

	// Bullet collision configuration.
	btCollisionConfiguration* config;

	// Controls the physics broadphase.
	btBroadphaseInterface* broadphase;

	// Dispatches object collisions.
	btCollisionDispatcher* dispatcher;

	// Solves the physics equations.
	btSequentialImpulseConstraintSolver* solver;

	// Physics world.
	btDiscreteDynamicsWorld* world;
};

//-----------------------------------//

NAMESPACE_ENGINE_END

#else

NAMESPACE_ENGINE_BEGIN

class PhysicsManager
{
};

NAMESPACE_ENGINE_END

#endif