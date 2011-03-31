/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Memory.h"

//#define ALLOCATOR_NO_MEMORY
#define ALLOCATOR_DEFAULT_GROUP "General";
#define ALLOCATOR_TRACKING

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>

	#if defined(VAPOR_MEMORY_LEAK_DETECTOR) && defined(VAPOR_DEBUG)
		#include <vld.h>
	#endif
#endif

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

// Since all memory allocations must go through an allocator, we need
// to create a default allocator to make the first allocations.

static MemoryAllocator* GetDefaultHeapAllocator();
static MemoryAllocator* GetDefaultStackAllocator();

static MemoryAllocator* gs_defaultAllocator = GetDefaultHeapAllocator();
static MemoryAllocator* gs_defaultStackAllocator = GetDefaultStackAllocator();

MemoryAllocator* AllocatorGetDefault() { return gs_defaultAllocator; }
MemoryAllocator* AllocatorGetStack() { return gs_defaultStackAllocator; }

static std::map<const char*, int64> gs_memoryGroups; 

//-----------------------------------//

#ifdef ALLOCATOR_TRACKING
static void AllocatorTrackGroup(MemoryAllocator* mem, int32 size)
{
	if(!mem || !mem->group) return;
	gs_memoryGroups[mem->group] += size; 
}
#endif

static void* HeapAllocate(MemoryAllocator* mem, int32 size)
{
#ifdef ALLOCATOR_TRACKING
	AllocatorTrackGroup(mem, size);
#endif

#ifdef ALLOCATOR_NO_MEMORY
	return nullptr;
#else
	return malloc(size);
#endif
}

static void HeapDellocate(MemoryAllocator* mem, void* p, int32 size)
{
#ifdef ALLOCATOR_TRACKING
	AllocatorTrackGroup(mem, -size);
#endif

#ifdef ALLOCATOR_NO_MEMORY
	return;
#else
	free(p);
#endif
}

MemoryAllocator* AllocatorCreateHeap( MemoryAllocator* mem )
{
	MemoryAllocator* heap = Allocate<MemoryAllocator>(mem);
	heap->allocate = HeapAllocate;
	heap->deallocate = HeapDellocate;
	return heap;
}

static MemoryAllocator* GetDefaultHeapAllocator()
{
	static MemoryAllocator heap;
	heap.allocate = HeapAllocate;
	heap.deallocate = HeapDellocate;
	heap.group = ALLOCATOR_DEFAULT_GROUP;
	return &heap;
}

//-----------------------------------//

static void* StackAllocate(MemoryAllocator* mem, int32 size)
{
#ifdef ALLOCATOR_TRACKING
	AllocatorTrackGroup(mem, size);
#endif

#ifdef ALLOCATOR_NO_MEMORY
	return nullptr;
#else
	return alloca(size);
#endif
}

static void StackDellocate(MemoryAllocator* mem, void* p, int32 size)
{
#ifdef ALLOCATOR_TRACKING
	AllocatorTrackGroup(mem, -size);
#endif

	// memory is automatically freed by the stack
}

static MemoryAllocator* GetDefaultStackAllocator()
{
	static MemoryAllocator stack;
	stack.allocate = StackAllocate;
	stack.deallocate = StackDellocate;
	stack.group = ALLOCATOR_DEFAULT_GROUP;
	return &stack;
}

//-----------------------------------//

MemoryAllocator* AllocatorCreatePage( MemoryAllocator* mem )
{
	return nullptr;
}

//-----------------------------------//

MemoryAllocator* AllocatorCreatePool( MemoryAllocator* mem )
{
	return nullptr;
}

//-----------------------------------//

MemoryAllocator* AllocatorCreateTemporary( MemoryAllocator* mem )
{
	return nullptr;
}

//-----------------------------------//

NAMESPACE_EXTERN_END