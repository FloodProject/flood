/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct Allocator;

API_CORE Allocator* AllocatorGetHeap();
API_CORE Allocator* AllocatorGetStack();

API_CORE Allocator* AllocatorCreateHeap( Allocator*, const char* group );
API_CORE Allocator* AllocatorCreateStack( Allocator* );
API_CORE Allocator* AllocatorCreatePool( Allocator*, int32 size );
API_CORE Allocator* AllocatorCreatePage( Allocator* );
API_CORE Allocator* AllocatorCreateTemporary( Allocator* );

API_CORE void AllocatorDestroy( Allocator* allocatee, Allocator* allocator );
API_CORE void AllocatorDumpInfo();

typedef void* (*MemoryAllocateFunction)(Allocator*, int32 size, int32 align);
typedef void  (*MemoryFreeFunction)(Allocator*, void* object);

struct Allocator
{
	MemoryAllocateFunction allocate;
	MemoryFreeFunction deallocate;
	const char* group;
};

struct PoolAllocator : Allocator
{
	uint8* current;
};

EXTERN_END

//-----------------------------------//

template<typename T> T* Allocate(Allocator* alloc)
{
	// Allocates memory for the object.
	T* object = (T*) alloc->allocate( alloc, sizeof(T), alignof(T) );

	// Calls the object contructor using placement new.
	new (object) T;

	return object;
}

template<typename T> void Deallocate(Allocator* alloc, T* object)
{
	// Calls the object destructor.
	if(object) object->~T();

	// Deallocates the memory.
	alloc->deallocate( alloc, object );
}

//-----------------------------------//

NAMESPACE_END
