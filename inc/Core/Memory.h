/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"

// Doug Lea's malloc memory space.
typedef void* mspace;

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct Allocator;

#define AllocatorGetThis() (AllocatorGetObject((void*)this))

API_CORE Allocator* AllocatorGetHeap();
API_CORE Allocator* AllocatorGetStack();
API_CORE Allocator* AllocatorGetObject(void*);

API_CORE void AllocatorDestroy( Allocator* );
API_CORE void AllocatorReset( Allocator* );
API_CORE void AllocatorSetGroup( Allocator*, const char* group );
API_CORE void AllocatorDumpInfo();

API_CORE void* AllocatorAllocate( Allocator*, int32 size, int32 align );
API_CORE void  AllocatorDeallocate( const void* );

typedef void* (*MemoryAllocateFunction)(Allocator*, int32 size, int32 align);
typedef void  (*MemoryFreeFunction)(Allocator*, const void* object);
typedef void  (*MemoryResetFunction)(Allocator*);

/**
 * Interface for a custom memory allocator.
 */

struct API_CORE Allocator
{
	FLD_IGNORE MemoryAllocateFunction allocate;
	FLD_IGNORE MemoryFreeFunction deallocate;
	FLD_IGNORE MemoryResetFunction reset;
	const char* group;
};

/**
 * Manages memory allocation using a fixed-size object pool. When a new
 * allocation is requested, we search for the first free object available
 * and return it.
 */

struct API_CORE PoolAllocator : public Allocator
{
	uint8* current;
};

API_CORE Allocator* AllocatorCreatePool( Allocator*, int32 size );

/**
 * Manages memory allocations using a fixed-size buffer that can provide
 * allocations and deallocations in O(1). The allocated space in the buffer
 * is not re-used until the whole buffer is reset.
 */

struct API_CORE BumpAllocator : public Allocator
{
	uint8* start;
	uint8* current;
	uint32 size;
};

API_CORE Allocator* AllocatorCreateBump( Allocator*, int32 size );

/**
 * Manages memory allocation using Doug Lea's malloc implementation.
 * This is a boundary-tag allocator that manages memory by keeping
 * track of the used/free memory blocks.
 */

struct API_CORE HeapAllocator : public Allocator
{
	mspace space;
};

API_CORE Allocator* AllocatorCreateHeap( Allocator* );
API_CORE Allocator* AllocatorCreateStack( Allocator* );

//-----------------------------------//

#ifdef COMPILER_MSVC
#pragma warning(disable : 4345) // behavior change: an object of POD type
// constructed with an initializer of the form () will be default-initialized
#endif

// Calls the object contructor using placement new.
#define AllocateConstruct(Type, Object, ...) ::new (Object) Type(__VA_ARGS__)

template<typename T> T* AllocateObject(Allocator* alloc)
{
	T* object = (T*) AllocatorAllocate(alloc, sizeof(T), alignof(T));
	return object;
}

// Allocates memory for the object.
#define Allocate(Alloc, Type, ...) AllocateConstruct(Type, \
	AllocateObject<Type>(Alloc), __VA_ARGS__)

// Allocation helpers.
#define AllocateThis(Type, ...) Allocate(AllocatorGetThis(), Type, __VA_ARGS__)
#define AllocateHeap(Type, ...) Allocate(AllocatorGetHeap(), Type, __VA_ARGS__)

template<typename T> void DeallocateObject(const T* object)
{
	// Calls the object destructor.
	if(object) object->~T();

	AllocatorDeallocate(object);
}

#define Deallocate(object) \
	MULTI_LINE_MACRO_BEGIN \
	DeallocateObject(object); object = nullptr; \
	MULTI_LINE_MACRO_END

//-----------------------------------//

NAMESPACE_CORE_END
