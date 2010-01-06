/************************************************************************
*
* vaporEngine (2008-2009)
*
************************************************************************/

#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h> // include for hkBaseSystem
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h> // hkPoolMemory
#include <Common/Base/Memory/hkThreadMemory.h>

#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Dynamics/Entity/hkpEntity.h>

#include "vapor/math/Vector3.h"
#include "vapor/Singleton.h"

class hkpWorld;

namespace vapor { namespace physics {

//-----------------------------------//

class VAPOR_API PhysicsManager 
	: public Singleton<PhysicsManager>, private boost::noncopyable
{
public:

	PhysicsManager();
	virtual ~PhysicsManager();
	
	void createWorld();
	
	void createWorld(math::Vector3 gravity, float broadphaseSize,
		float collisionTolerance, float maxVelocity, float delta, 
		signed char contactpoint, hkpWorldCinfo::SimulationType sim, hkpWorldCinfo::SolverType solver);

	void update(float delta);
	void addEntity(hkpEntity * entity);
	void removeEntity(hkpEntity * entity);

private:

	bool worldCreated;  
	hkpWorld* world; 
	char* stackBuffer;

};

//-----------------------------------//

} } // end namespaces