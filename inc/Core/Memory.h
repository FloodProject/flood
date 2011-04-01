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

typedef void* (*MemoryAllocateFunction)(Allocator*, int32);
typedef void (*MemoryFreeFunction)(Allocator*, void*, int32);

struct Allocator
{
	MemoryAllocateFunction allocate;
	MemoryFreeFunction deallocate;
	const char* group;
};

extern Allocator* AllocatorGlobalHeap;
extern Allocator* AllocatorGlobalStack;

API_CORE Allocator* AllocatorGetHeap();
API_CORE Allocator* AllocatorGetStack();

API_CORE Allocator* AllocatorCreateHeap( Allocator* );
API_CORE Allocator* AllocatorCreateStack( Allocator* );
API_CORE Allocator* AllocatorCreatePage( Allocator* );
API_CORE Allocator* AllocatorCreatePool( Allocator* );
API_CORE Allocator* AllocatorCreateTemporary( Allocator* );

EXTERN_END

//-----------------------------------//

template<typename T> T* Allocate(Allocator* mem)
{
	// Allocates memory for the object.
	T* object = (T*) mem->allocate( mem, sizeof(T) );

	// Calls the object contructor using placement new.
	new (object) T;

	return object;
}

template<typename T> void Deallocate(Allocator* mem, T* object)
{
	// Calls the object destructor.
	if(object) object->~T();

	// Deallocates the memory.
	mem->deallocate( mem, object, sizeof(T) );
}

//-----------------------------------//

NAMESPACE_END
