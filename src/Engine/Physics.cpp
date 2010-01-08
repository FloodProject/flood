/************************************************************************
*
* vaporEngine (2008-2009)
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "vapor/physics/Physics.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h> // include for hkBaseSystem
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h> // hkPoolMemory
#include <Common/Base/Memory/hkThreadMemory.h>

#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Dynamics/Entity/hkpEntity.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>

#ifdef HAVOK_ENABLE_VDB
	#include <Common/Visualize/hkVisualDebugger.h>
	#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif

#define INCLUDE_HAVOK_PHYSICS_CLASSES
#define HK_CLASSES_FILE "Common/Serialize/ClassList/hkClasses.h"
#include "Common/Serialize/Util/hkBuiltinTypeRegistry.cxx"

// Generate a custom list to trim memory requirements
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_None.cxx>

namespace vapor { namespace physics {

//-----------------------------------//

static void errorReportFunction(const char* str, void* errorOutputObject)
{
    debug("%s", str);
}

//-----------------------------------//

PhysicsManager::PhysicsManager() : worldCreated( false ), enableSimulation( true )
{
	hkPoolMemory* memoryManager = new hkPoolMemory();
	threadMemory = new hkThreadMemory(memoryManager);
	
	hkBaseSystem::init( memoryManager, threadMemory, errorReportFunction );
	memoryManager->removeReference();

	{
		int stackSize = 0x100000;
		stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE );
		hkThreadMemory::getInstance().setStackArea( stackBuffer, stackSize );
	}
}

//-----------------------------------//

PhysicsManager::~PhysicsManager()
{
	if(worldCreated) 
		world->removeReference();

#ifdef HAVOK_ENABLE_VDB
	vdb->removeReference();
#endif

	// Deallocate stack area
	threadMemory->setStackArea( 0, 0 );
	hkDeallocate( stackBuffer );

	threadMemory->removeReference();

	// Quit base system
	hkBaseSystem::quit();
}

//-----------------------------------//

void PhysicsManager::createWorld()
{
	if(!worldCreated)
	{
		hkpWorldCinfo info;
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		info.m_collisionTolerance = 0.1f; 
		info.setBroadPhaseWorldSize( 52000.0f );
		info.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY;
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_REJECT_DUBIOUS; 
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		world = new hkpWorld( info );
		worldCreated = true;
	}

	hkpAgentRegisterUtil::registerAllAgents( world->getCollisionDispatcher() );

#ifdef HAVOK_ENABLE_VDB
	hkpPhysicsContext* physicsContext = new hkpPhysicsContext;
	physicsContext->addWorld( world ); // add all worlds as you have
	hkpPhysicsContext::registerAllPhysicsProcesses();
	hkArray<hkProcessContext*> contexts;
	contexts.pushBack( physicsContext );  
	vdb = new hkVisualDebugger( contexts );
	vdb->capture("teste.hkm");
	//vdb->serve(/* optional port number */);
#endif
}

//-----------------------------------//

//void PhysicsManager::createWorld(math::Vector3 gravity, float broadphaseSize,
//		float collisionTolerance, float maxVelocity, float delta, 
//		signed char contactpoint, hkpWorldCinfo::SimulationType sim, hkpWorldCinfo::SolverType solver)
//{
//	if(!worldCreated)
//	{
//		hkpWorldCinfo info;
//		info.m_simulationType = sim;
//		info.m_gravity.set( gravity.x, gravity.y, gravity.z);
//		info.m_collisionTolerance = collisionTolerance; 
//		info.setBroadPhaseWorldSize( broadphaseSize );
//		info.setupSolverInfo( solver );
//		world = new hkpWorld( info );
//		worldCreated = true;
//	}
//}

//-----------------------------------//

void PhysicsManager::update(float delta)
{
	if( !enableSimulation ) return;

	// TODO: hack!
	del = delta;

	if(worldCreated)
	{

#ifdef HAVOK_ENABLE_VDB
		vdb->step( delta );
#endif

		world->stepDeltaTime(delta);
	}
}

//-----------------------------------//

void PhysicsManager::addEntity(hkpEntity* entity)
{
	if(worldCreated)
		world->addEntity(entity);
}

//-----------------------------------//

void PhysicsManager::removeEntity(hkpEntity* entity)
{
	if(worldCreated)
		world->removeEntity(entity);
}

//-----------------------------------//

bool PhysicsManager::getSimulationEnabled()
{
	return enableSimulation;
}

//-----------------------------------//

void PhysicsManager::setSimulationEnabled( bool enable )
{
	this->enableSimulation = enable;
}

//-----------------------------------//

} } // end namespaces
