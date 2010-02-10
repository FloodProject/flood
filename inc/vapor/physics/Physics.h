/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/Singleton.h"

// Uncomment this to enable the Havok Visual Debugger
#define HAVOK_ENABLE_VDB

class hkVisualDebugger;
class hkpWorld;
class hkpWorldCinfo;
class hkpEntity;
class hkThreadMemory;

namespace vapor { namespace physics {

//-----------------------------------//

/**
 * Global hub for physics simulations.
 */

class VAPOR_API PhysicsManager 
	: public Singleton<PhysicsManager>, private boost::noncopyable
{
public:

	PhysicsManager();
	virtual ~PhysicsManager();
	
	void createWorld();
	
	//void createWorld(math::Vector3 gravity, float broadphaseSize,
	//	float collisionTolerance, float maxVelocity, float delta, 
	//	signed char contactpoint, hkpWorldCinfo::SimulationType sim, hkpWorldCinfo::SolverType solver);

	void update( double delta );
	void addEntity( hkpEntity* entity );
	void removeEntity( hkpEntity* entity );

	bool getSimulationEnabled();
	void setSimulationEnabled( bool enable );

	float del;
	hkpWorld* getWorld(){ return world; }

private:

	bool enableSimulation;

	hkpWorld* world; 
	hkThreadMemory* threadMemory;
	
#ifdef HAVOK_ENABLE_VDB
	hkVisualDebugger* vdb;
#endif

	bool worldCreated;  
	char* stackBuffer;
};

//-----------------------------------//

} } // end namespaces
