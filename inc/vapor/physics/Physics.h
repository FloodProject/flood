/************************************************************************
*
* vaporEngine (2008-2009)
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/Singleton.h"

class hkVisualDebugger;
class hkpWorld;
class hkpWorldCinfo;
class hkpEntity;
//enum hkpWorldCinfo::SimulationType;
//enum hkpWorldCinfo::SolverType;

namespace vapor { namespace physics {

//-----------------------------------//

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

	void update(float delta);
	void addEntity(hkpEntity * entity);
	void removeEntity(hkpEntity * entity);

	float del;	
private:

	hkVisualDebugger* vdb;
	bool worldCreated;  
	hkpWorld* world; 
	char* stackBuffer;

};

//-----------------------------------//

} } // end namespaces