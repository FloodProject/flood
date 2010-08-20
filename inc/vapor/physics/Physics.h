/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Subsystem.h"
#include "vapor/math/Vector3.h"

class btCollisionConfiguration;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

FWD_DECL_SHARED(Body)

namespace vapor {

//-----------------------------------//

/**
 * Global hub for physics simulations.
 */

class VAPOR_API PhysicsManager : public Subsystem
{
public:

	PhysicsManager();
	virtual ~PhysicsManager();

	// Creates a new physics world.
	void createWorld();

	// Sets the gravity of the world.
	void setWorldGravity(const Vector3& gravity);
	
	// Updates the physical simulation.
	void update( double delta );

	// Gets/sets the physics simulation state.
	ACESSOR(Simulation, bool, enableSimulation);

	// Adds a rigid body to the physics world.
	void addRigidBody( const Body* body );

	// Removes a rigid body from the physics world.
	void removeRigidBody( const Body* body );

protected:

	// Keeps track of simulation state.
	bool enableSimulation;

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

} // end namespace