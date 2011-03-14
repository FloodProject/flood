/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once
#include <Core/API.h>

namespace vapor { extern "C" {

//-----------------------------------//

typedef void* (*MemoryAllocateFunction)(int32);
typedef void (*MemoryFreeFunction)(void*);

struct MemoryAllocator
{
	MemoryAllocateFunction allocate;
	MemoryFreeFunction deallocate;
};

CORE_API MemoryAllocator* AllocatorGetDefault();
CORE_API MemoryAllocator* AllocatorCreateHeap();
CORE_API MemoryAllocator* AllocatorCreatePage();
CORE_API MemoryAllocator* AllocatorCreatePool();
CORE_API MemoryAllocator* AllocatorCreateTemporary();

//-----------------------------------//

} // end extern

//-----------------------------------//

template<typename T> T* Allocate(MemoryAllocator* mem)
{
	T* object = (T*) mem->allocate( sizeof(T) );
	new (object) T;
	return object;
}

template<typename T> void Deallocate(MemoryAllocator* mem, T* object)
{
	if(object) object->~T();
	mem->deallocate(object);
}

//-----------------------------------//

} // end namespace
