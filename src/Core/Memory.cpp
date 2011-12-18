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
#include "Core/References.h"
#include "Core/Object.h"

#define ALLOCATOR_TRACKING
#define ALLOCATOR_DEFAULT_GROUP "General";

#ifdef PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	
	#undef NOMINMAX
	#define NOMINMAX
	
	#include <Windows.h>

	#if defined(ENABLE_MEMORY_LEAK_DETECTOR) && defined(DEBUG_BUILD)
		#include <vld.h>
	#endif
	
	#include <malloc.h>
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

#ifdef ENABLE_REFERENCES_DEBUG

static DebugReferenceDelegate gs_debugReferences;
DebugReferenceDelegate& GetDebugReferences() { return gs_debugReferences; }

#endif

// Since all memory allocations must go through an allocator,
// there needs to be a default allocator to allocate memory.

static Allocator* GetDefaultHeapAllocator();
static Allocator* GetDefaultStackAllocator();

Allocator* AllocatorGetHeap() { return GetDefaultHeapAllocator(); }
Allocator* AllocatorGetStack() { return GetDefaultStackAllocator(); }

static bool AllocatorSimulateLowMemory = false;

//-----------------------------------//

struct AllocationGroup
{
	AllocationGroup();

	int64 freed;
	int64 total;
};

AllocationGroup::AllocationGroup()
	: freed(0), total(0)
{
}

struct AllocationMetadata
{
	AllocationMetadata(); 

	int32 size;
	const char* group;
	Allocator* allocator;
	int32 pattern;
};

AllocationMetadata::AllocationMetadata()
	: size(0), group(nullptr), allocator(nullptr)
{
}

//-----------------------------------//

const int32 MEMORY_PATTERN = 0xDEADF69F;

typedef std::map<const char*, AllocationGroup, RawStringCompare> MemoryGroupMap;

static MemoryGroupMap& GetMemoryGroupMap()
{
	static MemoryGroupMap memoryGroups;
	return memoryGroups;
}

static void AllocatorTrackGroup(AllocationMetadata* metadata, bool alloc)
{
	if(!metadata) return;

	MemoryGroupMap& memoryGroups = GetMemoryGroupMap();
	memoryGroups[metadata->group].total += alloc ? metadata->size : 0;
	memoryGroups[metadata->group].freed += alloc ? 0 : metadata->size;
}

//-----------------------------------//

Allocator* AllocatorGetObject(void* object)
{
	if( !object )
		return AllocatorGetHeap();

	char* addr = (char*) object - sizeof(AllocationMetadata);
	AllocationMetadata* metadata = (AllocationMetadata*) addr;

	if(metadata->pattern != MEMORY_PATTERN)
		return AllocatorGetHeap();

	return metadata->allocator;
}

//-----------------------------------//

void AllocatorReset( Allocator* alloc )
{
	if(!alloc) return;
	if(!alloc->reset) return;

	alloc->reset(alloc);
}

//-----------------------------------//

void AllocatorSetGroup( Allocator* alloc, const char* group )
{
	alloc->group = group;
}

//-----------------------------------//

void AllocatorDumpInfo()
{
	MemoryGroupMap& groups = GetMemoryGroupMap();
	if( groups.empty() ) return;

	LogDebug("-----------------------------------------------------");
	LogDebug("Memory stats");
	LogDebug("-----------------------------------------------------");

	MemoryGroupMap::iterator it;
	for(it = groups.begin(); it != groups.end(); ++it)
	{
		const char* id = it->first;
		AllocationGroup& group = it->second;
		
		const char* fs = "%s\t| total freed: %I64d bytes, total allocated: %I64d bytes";
		String format = StringFormat(fs, id, group.freed, group.total );

		LogDebug( format.c_str() );
	}
}

//-----------------------------------//

void AllocatorDestroy( Allocator* object )
{
	Deallocate(object);
}

//-----------------------------------//

void* AllocatorAllocate( Allocator* alloc, int32 size, int32 align )
{
	return alloc->allocate( alloc, size, align );
}

//-----------------------------------//

void AllocatorDeallocate( const void* object )
{
	if( !object ) return;

	char* addr = (char*) object - sizeof(AllocationMetadata);
	AllocationMetadata* metadata = (AllocationMetadata*) addr;
	Allocator* alloc = metadata->allocator;

	if(metadata->pattern != MEMORY_PATTERN)
	{
		free((void*)object);
		return;
	}

	// Deallocates the memory.
	alloc->deallocate( alloc, object );
}

//-----------------------------------//

static void* HeapAllocate(Allocator* alloc, int32 size, int32 align)
{
	if(AllocatorSimulateLowMemory) return nullptr;
	
	int32 total_size = size + sizeof(AllocationMetadata);
	void* instance = nullptr;
	
#ifdef ALIGNED_MALLOC
	if(align == 0)
		instance = malloc(total_size);
	else
		instance = _aligned_malloc(total_size, align);
#endif

	instance = malloc(total_size);

	AllocationMetadata* metadata = (AllocationMetadata*) instance;
	metadata->size = size;
	metadata->group = alloc->group;
	metadata->allocator = alloc;
	metadata->pattern = MEMORY_PATTERN;

#ifdef ALLOCATOR_TRACKING
	AllocatorTrackGroup(metadata, true);
#endif

	return (char*) instance + sizeof(AllocationMetadata);
}

static void HeapDellocate(Allocator* alloc, const void* p)
{
#ifdef ALLOCATOR_TRACKING
	void* base = (char*) p - sizeof(AllocationMetadata);
	AllocatorTrackGroup((AllocationMetadata*) base, false);
#endif

#if ALIGNED_MALLOC
	_aligned_free(base);
#endif

	free(base);
}

//-----------------------------------//

Allocator* AllocatorCreateHeap( Allocator* alloc )
{
	Allocator* heap = Allocate(alloc, Allocator);

	heap->allocate = HeapAllocate;
	heap->deallocate = HeapDellocate;
	heap->reset = nullptr;
	heap->group = nullptr;

	return heap;
}

//-----------------------------------//

static Allocator* GetDefaultHeapAllocator()
{
	static Allocator heap;
	heap.allocate = HeapAllocate;
	heap.deallocate = HeapDellocate;
	heap.reset = nullptr;
	heap.group = ALLOCATOR_DEFAULT_GROUP;

	return &heap;
}

//-----------------------------------//

static void* StackAllocate(Allocator* alloc, int32 size, int32 align)
{
	if(AllocatorSimulateLowMemory) return nullptr;

#ifdef ALLOCATOR_TRACKING
	//AllocatorTrackGroup(alloc, size);
#endif

	return alloca(size);
}

//-----------------------------------//

static void StackDellocate(Allocator* alloc, const void* p)
{
#ifdef ALLOCATOR_TRACKING
	//AllocatorTrackGroup(alloc, 0);
#endif

	// Stack memory is automatically freed.
}

//-----------------------------------//

static Allocator* GetDefaultStackAllocator()
{
	static Allocator stack;
	stack.allocate = StackAllocate;
	stack.deallocate = StackDellocate;
	stack.group = ALLOCATOR_DEFAULT_GROUP;
	return &stack;
}

//-----------------------------------//

static void* PoolAllocate(Allocator* alloc, int32 size, int32 align)
{
	if(AllocatorSimulateLowMemory) return nullptr;

	PoolAllocator* pool = (PoolAllocator*) alloc;

	void* p = pool->current; 
	pool->current += size;

	return p;
}

//-----------------------------------//

static void PoolDeallocate(Allocator* alloc, const void* p)
{
}

//-----------------------------------//

Allocator* AllocatorCreatePool( Allocator* alloc, int32 size )
{
	PoolAllocator* pool = (PoolAllocator*) alloc->allocate(alloc,
		sizeof(PoolAllocator) + size, alignof(PoolAllocator));

	pool->current = (uint8*) pool + sizeof(PoolAllocator);
	pool->allocate = PoolAllocate;
	pool->deallocate = PoolDeallocate;
	pool->reset = nullptr;
	pool->group = nullptr;

	return pool;
}

//-----------------------------------//

Allocator* AllocatorCreatePage( Allocator* alloc )
{
	return nullptr;
}

//-----------------------------------//

static void* BumpAllocate(Allocator* alloc, int32 size, int32 align)
{
	if(AllocatorSimulateLowMemory) return nullptr;

	BumpAllocator* bump = (BumpAllocator*) alloc;

	void* current = bump->current;

	uint8* end = bump->start + bump->size;
	size_t left = end - bump->current;

	if((size_t)size > left) // Not enough space to allocate.
		return nullptr;

	bump->current += size;

	// Zero the memory.
	memset(current, 0, size);

	return current;
}

//-----------------------------------//

static void BumpDeallocate(Allocator* alloc, const void* p)
{
	// Do nothing.
}

//-----------------------------------//

static void BumpReset(Allocator* alloc)
{
	BumpAllocator* bump = (BumpAllocator*) alloc;
	bump->current = bump->start;
}

//-----------------------------------//

Allocator* AllocatorCreateBump( Allocator* alloc, int32 size )
{
	BumpAllocator* bump = (BumpAllocator*) alloc->allocate(
		alloc, sizeof(BumpAllocator) + size, alignof(BumpAllocator));

	bump->start = (uint8*) bump + sizeof(BumpAllocator);
	bump->current = bump->start;
	bump->size = size;
	bump->allocate = BumpAllocate;
	bump->deallocate = BumpDeallocate;
	bump->reset = BumpReset;
	bump->group = nullptr;

	return bump;
}

//-----------------------------------//

int32 ReferenceGetCount(ReferenceCounted* ref)
{
	return AtomicRead(&ref->references);
}

void ReferenceAdd(ReferenceCounted* ref)
{
	AtomicIncrement(&ref->references);
}

bool ReferenceRelease(ReferenceCounted* ref)
{
	return AtomicDecrement(&ref->references) == 0;
}

//-----------------------------------//

NAMESPACE_CORE_END