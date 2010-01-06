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
{
	hkPoolMemory* memoryManager = new hkPoolMemory();
	hkThreadMemory* threadMemory = new hkThreadMemory(memoryManager);
	hkBaseSystem::init( memoryManager, threadMemory, errorReportFunction );
	memoryManager->removeReference();
	
	{
		int stackSize = 0x100000;
		stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE);
		hkThreadMemory::getInstance().setStackArea( stackBuffer, stackSize);
	}

	worldCreated = false;
}

//-----------------------------------//

PhysicsManager::~PhysicsManager()
{
	hkBaseSystem::quit();
	if(worldCreated) world->removeReference();
}

//-----------------------------------//

void PhysicsManager::createWorld()
{
	if(!worldCreated)
	{
		hkpWorldCinfo info;
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		info.m_collisionTolerance = 0.1f; 
		info.setBroadPhaseWorldSize( 150.0f );
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_REJECT_DUBIOUS; 
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
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
		world->addEntity(entity);
}

//-----------------------------------//

} } // end namespaces