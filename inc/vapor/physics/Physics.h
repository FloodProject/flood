/************************************************************************
*
* vaporEngine (2008-2009)
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/Singleton.h"

class hkpWorld;

namespace vapor { namespace physics {


enum SolvType { 
	SOLVER_TYPE_INVALID, 
	SOLVER_TYPE_2ITERS_SOFT, 
	SOLVER_TYPE_2ITERS_MEDIUM, 
	SOLVER_TYPE_2ITERS_HARD, 
	SOLVER_TYPE_4ITERS_SOFT, 
	SOLVER_TYPE_4ITERS_MEDIUM, 
	SOLVER_TYPE_4ITERS_HARD, 
	SOLVER_TYPE_8ITERS_SOFT, 
	SOLVER_TYPE_8ITERS_MEDIUM, 
	SOLVER_TYPE_8ITERS_HARD, 
	SOLVER_TYPE_MAX_ID 
}; 

enum SimType
{ 
	SIMULATION_TYPE_INVALID, 
	SIMULATION_TYPE_DISCRETE, 
	SIMULATION_TYPE_CONTINUOUS, 
	SIMULATION_TYPE_MULTITHREADED 
}; 


class VAPOR_API PhysicsManager 
	: public Singleton<PhysicsManager>, private boost::noncopyable
{
public:
	PhysicsManager();
	virtual ~PhysicsManager();
	void createWorld();
	void createWorld(math::Vector3 gravity, float broadphaseSize,
		float collisionTolerance, float maxVelocity, float delta, 
		signed char contactpoint, SimType sim, SolvType solver);

private:
	bool worldCreated;  
	hkpWorld* world; 


};

} } // end namespaces