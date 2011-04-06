/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#define ALLOCATOR_TRACKING
#define ALLOCATOR_DEFAULT_GROUP "General";

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

// Since all memory allocations must go through an allocator,
// there needs to be a default allocator to allocate memory.

static Allocator* GetDefaultHeapAllocator();
static Allocator* GetDefaultStackAllocator();

Allocator* AllocatorGetHeap() { return GetDefaultHeapAllocator(); }
Allocator* AllocatorGetStack() { return GetDefaultStackAllocator(); }

struct AllocationGroup
{
	AllocationGroup() : freed(0), total(0) { }

	int64 freed;
	int64 total;
};

typedef std::map<const char*, AllocationGroup> MemoryGroupMap;

static MemoryGroupMap& GetMemoryGroupMap()
{
	static MemoryGroupMap memoryGroups;
	return memoryGroups;
}

static bool AllocatorSimulateLowMemory = false;

//-----------------------------------//

static void AllocatorTrackGroup(AllocationMetadata* metadata, bool alloc)
{
	if(!metadata) return;

	MemoryGroupMap& memoryGroups = GetMemoryGroupMap();
	memoryGroups[metadata->group].total += alloc ? metadata->size : 0;
	memoryGroups[metadata->group].freed += alloc ? 0 : metadata->size;
}

void AllocatorDumpInfo()
{
	MemoryGroupMap& memoryGroups = GetMemoryGroupMap();
	if(memoryGroups.empty()) return;

	OutputDebugStringA("-----------------------------------------------------\n");
	OutputDebugStringA("Memory stats\n");
	OutputDebugStringA("-----------------------------------------------------\n");

	MemoryGroupMap::iterator it;
	for(it = memoryGroups.begin(); it != memoryGroups.end(); it++)
	{
		AllocationGroup& group = it->second;

		String format = StringFormat("%s\t| free: %I64d bytes, total: %I64d bytes\n",
			it->first, group.freed, group.total );

		OutputDebugStringA( format.c_str() );
	}

	OutputDebugStringA("\n");
}

//-----------------------------------//

static void* HeapAllocate(Allocator* alloc, int32 size)
{
	if(AllocatorSimulateLowMemory) return nullptr;

	void* ptr = malloc(size + sizeof(AllocationMetadata));
	
	AllocationMetadata* metadata = (AllocationMetadata*) ptr;
	metadata->size = size;
	metadata->group = alloc->group;

#ifdef ALLOCATOR_TRACKING
	AllocatorTrackGroup(metadata, true);
#endif

	return (char*) ptr + sizeof(AllocationMetadata);
}

static void HeapDellocate(Allocator* alloc, void* p)
{
#ifdef ALLOCATOR_TRACKING
	void* base = (char*) p - sizeof(AllocationMetadata);
	AllocatorTrackGroup((AllocationMetadata*) base, false);
#endif

	free(base);
}

Allocator* AllocatorCreateHeap( Allocator* alloc, const char* group )
{
	Allocator* heap = Allocate<Allocator>(alloc);

	heap->allocate = HeapAllocate;
	heap->deallocate = HeapDellocate;
	heap->group = group;

	return heap;
}

static Allocator* GetDefaultHeapAllocator()
{
	static Allocator heap;
	heap.allocate = HeapAllocate;
	heap.deallocate = HeapDellocate;
	heap.group = ALLOCATOR_DEFAULT_GROUP;
	return &heap;
}

//-----------------------------------//

static void* StackAllocate(Allocator* alloc, int32 size)
{
	if(AllocatorSimulateLowMemory) return nullptr;

#ifdef ALLOCATOR_TRACKING
	//AllocatorTrackGroup(alloc, size);
#endif

	return alloca(size);
}

static void StackDellocate(Allocator* alloc, void* p)
{
#ifdef ALLOCATOR_TRACKING
	//AllocatorTrackGroup(alloc, 0);
#endif

	// memory is automatically freed by the stack
}

static Allocator* GetDefaultStackAllocator()
{
	static Allocator stack;
	stack.allocate = StackAllocate;
	stack.deallocate = StackDellocate;
	stack.group = ALLOCATOR_DEFAULT_GROUP;
	return &stack;
}

//-----------------------------------//

Allocator* AllocatorCreatePage( Allocator* alloc )
{
	return nullptr;
}

//-----------------------------------//

Allocator* AllocatorCreatePool( Allocator* alloc )
{
	return nullptr;
}

//-----------------------------------//

Allocator* AllocatorCreateTemporary( Allocator* alloc )
{
	return nullptr;
}

//-----------------------------------//

void AllocatorDestroy( Allocator* object, Allocator* allocator )
{
	Deallocate<Allocator>(allocator, object);
}

//-----------------------------------//

NAMESPACE_EXTERN_END