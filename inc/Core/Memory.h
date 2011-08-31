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
API_CORE Allocator* AllocatorGetObject(void*);
#define AllocatorGetThis() (AllocatorGetObject(this))

API_CORE Allocator* AllocatorCreateHeap( Allocator* );
API_CORE Allocator* AllocatorCreateStack( Allocator* );
API_CORE Allocator* AllocatorCreatePool( Allocator*, int32 size );
API_CORE Allocator* AllocatorCreatePage( Allocator* );
API_CORE Allocator* AllocatorCreateBump( Allocator*, int32 size );

API_CORE void AllocatorDestroy( Allocator* );
API_CORE void AllocatorReset( Allocator* );
API_CORE void AllocatorSetGroup( Allocator*, const char* group );
API_CORE void AllocatorDumpInfo();

API_CORE void* AllocatorAllocate( Allocator*, int32 size, int32 align );
API_CORE void  AllocatorDeallocate( const void* );

typedef void* (*MemoryAllocateFunction)(Allocator*, int32 size, int32 align);
typedef void  (*MemoryFreeFunction)(Allocator*, const void* object);
typedef void  (*MemoryResetFunction)(Allocator*);

struct API_CORE Allocator
{
	MemoryAllocateFunction allocate;
	MemoryFreeFunction deallocate;
	MemoryResetFunction reset;
	const char* group;
};

struct API_CORE PoolAllocator : public Allocator
{
	uint8* current;
};

struct API_CORE BumpAllocator : public Allocator
{
	uint8* start;
	uint8* current;
	uint32 size;
};

EXTERN_END

//-----------------------------------//

#ifdef COMPILER_MSVC
#pragma warning(disable : 4345) // behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized
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
#define AllocateThis(Type, ...) Allocate(Type, AllocatorGetThis(), __VA_ARGS__)

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
