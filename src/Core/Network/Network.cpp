/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/Network.h"
#include "Core/Log.h"
#include "Core/Memory.h"

#include <enet/enet.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static Allocator* gs_NetworkAllocator = nullptr;

Allocator* AllocatorGetNetwork()
{
	return gs_NetworkAllocator;
}

static void* ENET_CALLBACK enet_custom_malloc(size_t size)
{
	return AllocatorAllocate(gs_NetworkAllocator, size, 0);
}

static void ENET_CALLBACK enet_custom_free(void * memory)
{
	return AllocatorDeallocate(memory);
}

//-----------------------------------//

bool NetworkInitialize()
{
	gs_NetworkAllocator = AllocatorCreateHeap(AllocatorGetHeap());
	AllocatorSetGroup(gs_NetworkAllocator, "Network");

	ENetCallbacks callbacks;
	callbacks.malloc = enet_custom_malloc;
	callbacks.free = enet_custom_free;

	int ret = enet_initialize_with_callbacks(ENET_VERSION, &callbacks);

	if(ret != 0)
	{
		LogError("Error initializing ENet");
		return false;
	}

	LogInfo("Initialized ENet %d.%d.%d networking layer",
		ENET_VERSION_MAJOR, ENET_VERSION_MINOR, ENET_VERSION_PATCH);

	return true;
}

//-----------------------------------//

void NetworkDeinitialize()
{
	enet_deinitialize();
	
	AllocatorDestroy(gs_NetworkAllocator);
	gs_NetworkAllocator = nullptr;

	LogInfo("Deinitialized ENet");
}

//-----------------------------------//

NAMESPACE_CORE_END
