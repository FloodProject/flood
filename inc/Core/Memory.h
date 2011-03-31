/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <Core/API.h>

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

typedef void* (*MemoryAllocateFunction)(MemoryAllocator*, int32);
typedef void (*MemoryFreeFunction)(MemoryAllocator*, void*, int32);

struct MemoryAllocator
{
	MemoryAllocateFunction allocate;
	MemoryFreeFunction deallocate;
	const char* group;
};

API_CORE MemoryAllocator* AllocatorGetDefault();
API_CORE MemoryAllocator* AllocatorGetStack();

API_CORE MemoryAllocator* AllocatorCreateHeap( MemoryAllocator* );
API_CORE MemoryAllocator* AllocatorCreateStack( MemoryAllocator* );
API_CORE MemoryAllocator* AllocatorCreatePage( MemoryAllocator* );
API_CORE MemoryAllocator* AllocatorCreatePool( MemoryAllocator* );
API_CORE MemoryAllocator* AllocatorCreateTemporary( MemoryAllocator* );

EXTERN_END

//-----------------------------------//

template<typename T> T* Allocate(MemoryAllocator* mem)
{
	// Allocates memory for the object.
	T* object = (T*) mem->allocate( mem, sizeof(T) );

	// Calls the object contructor using placement new.
	new (object) T;

	return object;
}

template<typename T> void Deallocate(MemoryAllocator* mem, T* object)
{
	// Calls the object destructor.
	if(object) object->~T();

	// Deallocates the memory.
	mem->deallocate( mem, object, sizeof(T) );
}

//-----------------------------------//

NAMESPACE_END
