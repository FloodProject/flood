/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Log.h"
#include "Core/References.h"
#include "Core/Object.h"

#define ALLOCATOR_TRACKING
#define ALLOCATOR_DEFAULT_GROUP "General";
#define ALLOCATOR_DEFAULT_GROUP_ID 0;

#ifdef PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    
    #undef NOMINMAX
    #define NOMINMAX
    
    #include <Windows.h>

    #if defined(ENABLE_MEMORY_LEAK_DETECTOR) && defined(BUILD_DEBUG)
        #include <vld.h>
    #endif
    
    #include <malloc.h>
#else
    #include <alloca.h>
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

/**
 * Tracks statistics for each allocator.
 */

struct AllocationGroup
{
    AllocationGroup();
    AllocationGroup(const char * name);

    int64 freed;
    int64 total;
    const char * name;
};

AllocationGroup::AllocationGroup()
    : freed(0)
    , total(0)
{
}
AllocationGroup::AllocationGroup(const char * name)
    : freed(0)
    , total(0)
    , name(name)
{
}

/**
 * Metadata stored with each memory allocation.
 */

struct AllocationMetadata
{
    AllocationMetadata(); 

    int32 size;
    int8 group;
    Allocator* allocator;
    int32 pattern;
};

AllocationMetadata::AllocationMetadata()
    : size(0), group(0), allocator(nullptr)
{
}

//-----------------------------------//

static const int32 MEMORY_PATTERN = 0xDEADBEEF;

// TODO: Do not use STL in this low level code.

typedef Vector<AllocationGroup, 32> MemoryGroupVector;

static void InitGroups(MemoryGroupVector &memoryGroups)
{
    char * gen = ALLOCATOR_DEFAULT_GROUP;
    memoryGroups.Push(AllocationGroup(gen));
}

static MemoryGroupVector& GetMemoryGroupVector()
{
    static MemoryGroupVector memoryGroups;
    if(memoryGroups.Empty())
        InitGroups(memoryGroups);
    return memoryGroups;
}

static void AllocatorTrackGroup(AllocationMetadata* metadata, bool alloc)
{
    if(!metadata) return;

    MemoryGroupVector& memoryGroups = GetMemoryGroupVector();
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
    MemoryGroupVector& groups = GetMemoryGroupVector();
    if (groups.Empty()) return;

    LogDebug("-----------------------------------------------------");
    LogDebug("Memory stats");
    LogDebug("-----------------------------------------------------");

    MemoryGroupVector::Iterator it;
    for(it = groups.begin(); it != groups.end(); ++it)
    {
        AllocationGroup& group = *it;
        
        const char* fs = "%s\t| total freed: %I64d bytes, total allocated: %I64d bytes";
        String format = String(StringFormat(fs, group.name, group.freed, group.total ));

        LogDebug( format.CString() );
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
    metadata->group = alloc->id;
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
    heap.id = ALLOCATOR_DEFAULT_GROUP_ID;

    return &heap;
}

//-----------------------------------//

static void* StackAllocate(Allocator* alloc, int32 size, int32 align)
{
    if(AllocatorSimulateLowMemory) return nullptr;

#ifdef ALLOCATOR_TRACKING
    //AllocatorTrackGroup(alloc, size);
#endif

#ifdef COMPILER_MSVC
    return _malloca(size);
#else
    return alloca(size);
#endif
}

//-----------------------------------//

static void StackDellocate(Allocator* alloc, const void* p)
{
#ifdef ALLOCATOR_TRACKING
    //AllocatorTrackGroup(alloc, 0);
#endif

#ifdef COMPILER_MSVC
    _freea((void *) p);
#endif
}

//-----------------------------------//

Allocator* AllocatorCreateStack( Allocator* alloc )
{
    Allocator* stack = Allocate(alloc, Allocator);

    stack->allocate = StackAllocate;
    stack->deallocate = StackDellocate;
    stack->reset = nullptr;
    stack->group = nullptr;

    return stack;
}

//-----------------------------------//

static Allocator* GetDefaultStackAllocator()
{
    static Allocator stack;
    stack.allocate = StackAllocate;
    stack.deallocate = StackDellocate;
    stack.group = ALLOCATOR_DEFAULT_GROUP;
    stack.id = ALLOCATOR_DEFAULT_GROUP_ID;
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
    return ref->references.read();
}

void ReferenceAdd(ReferenceCounted* ref)
{
    ref->references.increment();
}

bool ReferenceRelease(ReferenceCounted* ref)
{
    return ref->references.decrement() == 0;
}

//-----------------------------------//

NAMESPACE_CORE_END