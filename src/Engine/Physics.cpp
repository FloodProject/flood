/************************************************************************
*
* vaporEngine (2008-2009)
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "vapor/physics/Physics.h"

namespace vapor { namespace physics {

//-----------------------------------//

static void errorReportFunction(const char* str, void* errorOutputObject)
{
    debug("%s", str);
}

//-----------------------------------//

PhysicsManager::PhysicsManager()
	: worldCreated(false)
{
	hkBaseSystem::init( new hkPoolMemory(), HK_NULL, errorReportFunction );
}

//-----------------------------------//

PhysicsManager::~PhysicsManager()
{
	hkBaseSystem::quit();
	if(worldCreated) delete world;
}

//-----------------------------------//

void PhysicsManager::createWorld()
{
	if(!worldCreated)
	{
		hkpWorldCinfo info;
		world = new hkpWorld( info );
		worldCreated = true;
	}
}

//-----------------------------------//

void PhysicsManager::createWorld(math::Vector3 gravity, float broadphaseSize,
		float collisionTolerance, float maxVelocity, float delta, 
		signed char contactpoint, hkpWorldCinfo::SimulationType sim, hkpWorldCinfo::SolverType solver)
{
	if(!worldCreated)
	{
		hkpWorldCinfo info;
		info.m_simulationType = sim;
		info.m_gravity.set( gravity.x, gravity.y, gravity.z);
		info.m_collisionTolerance = collisionTolerance; 
		info.setBroadPhaseWorldSize( broadphaseSize );
		info.setupSolverInfo( solver );
		world = new hkpWorld( info );
		worldCreated = true;
	}
}

//-----------------------------------//

void PhysicsManager::update(float delta)
{
	if(worldCreated) 
		world->stepDeltaTime(delta);
}

//-----------------------------------//

void PhysicsManager::addEntity(hkpEntity * entity)
{
	if(worldCreated)
		world->addEntity(entity);
}

//-----------------------------------//

void PhysicsManager::removeEntity(hkpEntity * entity)
{
	if(worldCreated)
		world->removeEntity(entity);
}

//-----------------------------------//

} } // end namespaces