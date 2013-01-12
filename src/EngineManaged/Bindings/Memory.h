/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Memory.h>

namespace Flood
{
    ref class Allocator;
    ref class PoolAllocator;
    ref class BumpAllocator;
    ref class HeapAllocator;

    delegate System::IntPtr MemoryAllocateFunction(Flood::Allocator^, int size, int align);

    delegate void MemoryFreeFunction(Flood::Allocator^, System::IntPtr object);

    delegate void MemoryResetFunction(Flood::Allocator^);


    /// <summary>
    /// Interface for a custom memory allocator.
    /// </summary>
    public ref class Allocator
    {
        property ::Allocator* NativePtr;

    public:
        Allocator(::Allocator* native);
    };

    /// <summary>
    /// Manages memory allocation using a fixed-size object pool. When a new
    /// allocation is requested, we search for the first free object available
    /// and return it.
    /// </summary>
    public ref class PoolAllocator
    {
        property ::PoolAllocator* NativePtr;

    public:
        PoolAllocator(::PoolAllocator* native);
    };

    /// <summary>
    /// Manages memory allocations using a fixed-size buffer that can provide
    /// allocations and deallocations in O(1). The allocated space in the buffer
    /// is not re-used until the whole buffer is reset.
    /// </summary>
    public ref class BumpAllocator
    {
        property ::BumpAllocator* NativePtr;

    public:
        BumpAllocator(::BumpAllocator* native);
    };

    /// <summary>
    /// Manages memory allocation using Doug Lea's malloc implementation. This
    /// is a boundary-tag allocator that manages memory by keeping track of the
    /// used/free memory blocks.
    /// </summary>
    public ref class HeapAllocator
    {
        property ::HeapAllocator* NativePtr;

    public:
        HeapAllocator(::HeapAllocator* native);
    };

    public ref class FloodMemory
    {
    public:
        static Flood::Allocator^ AllocatorGetHeap ();
        static Flood::Allocator^ AllocatorGetStack ();
        static Flood::Allocator^ AllocatorGetObject (System::IntPtr _0);
        static void AllocatorDestroy (Flood::Allocator^ _1);
        static void AllocatorReset (Flood::Allocator^ _2);
        static void AllocatorSetGroup (Flood::Allocator^ _3, System::String^ group);
        static void AllocatorDumpInfo ();
        static System::IntPtr AllocatorAllocate (Flood::Allocator^ _4, int size, int align);
        static void AllocatorDeallocate (System::IntPtr _5);
        static Flood::Allocator^ AllocatorCreatePool (Flood::Allocator^ _6, int size);
        static Flood::Allocator^ AllocatorCreateBump (Flood::Allocator^ _7, int size);
        static Flood::Allocator^ AllocatorCreateHeap (Flood::Allocator^ _8);
        static Flood::Allocator^ AllocatorCreateStack (Flood::Allocator^ _9);
    };
}

