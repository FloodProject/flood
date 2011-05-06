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

API_CORE void* AllocatorAllocate( Allocator*, int32 size, int32 align );
API_CORE void  AllocatorDeallocate( void* );

API_CORE void AllocatorDestroy( Allocator* );
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

#ifdef COMPILER_MSVC
#pragma warning(disable : 4345)
#endif

// Calls the object contructor using placement new.
#define Construct(Type, Object, ...) ::new (Object) Type(__VA_ARGS__)

template<typename T> T* AllocateObject(Allocator* alloc)
{
	T* object = (T*) AllocatorAllocate(alloc, sizeof(T), alignof(T));
	return object;
}

// Allocates memory for the object.
#define Allocate(Type, Alloc, ...) Construct(Type, AllocateObject<Type>(Alloc), __VA_ARGS__)

template<typename T> void Deallocate(T* object)
{
	// Calls the object destructor.
	if(object) object->~T();

	AllocatorDeallocate(object);
}

//-----------------------------------//

NAMESPACE_END
