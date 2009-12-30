/************************************************************************
*
* vaporEngine (2008-2009)
*
************************************************************************/

#pragma once

#include <Common/Base/hkBase.h>

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
	
	//void createWorld(math::Vector3 gravity, float broadphaseSize,
		//float collisionTolerance, float maxVelocity, float delta, 
		//signed char contactpoint, enum Sim SimType sim, enum SolvType solver);

private:

	bool worldCreated;  
	hkpWorld* world; 
};

//-----------------------------------//

} } // end namespaces