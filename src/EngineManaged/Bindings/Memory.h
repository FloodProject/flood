/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Memory.h>
#include "Reflection.h"

namespace Flood
{
    ref class Allocator;
    ref class PoolAllocator;
    ref class BumpAllocator;
    ref class HeapAllocator;
    ref class Class;
    ref class Field;
    ref class Type;
    enum struct TypeKind : unsigned char;
    ref class ReflectionContext;
    enum struct ReflectionWalkType : unsigned char;
    enum struct FieldQualifier : unsigned short;
    ref class Object;
    ref class ReferenceCounted;
    ref class RefPtr;
    enum struct ThreadPriority;
    enum struct TaskState;
    ref class Thread;
    ref class Delegate2;
    ref class ClosurePtr;
    ref class GenericClass;
    ref class DelegateMemento;
    ref class Mutex;
    ref class Condition;
    ref class Task;
    ref class Delegate1;
    ref class TaskPool;
    ref class ConcurrentQueue;
    ref class Event1;
    ref class Delegate0;
    ref class TaskEvent;
    ref class scoped_ptr;

    public delegate System::IntPtr MemoryAllocateFunction(Flood::Allocator^, int size, int align);

    public delegate void MemoryFreeFunction(Flood::Allocator^, System::IntPtr object);

    public delegate void MemoryResetFunction(Flood::Allocator^);

    /// <summary>
    /// Interface for a custom memory allocator.
    /// </summary>
    public ref class Allocator
    {
    public:
        property ::Allocator* NativePtr;

        Allocator(::Allocator* native);
        Allocator(System::IntPtr native);
        property Flood::MemoryAllocateFunction^ Allocate;
        property Flood::MemoryFreeFunction^ Deallocate;
        property Flood::MemoryResetFunction^ Reset;
        property System::String^ Group;
        void Destroy();
        void ResetMemory();
        void SetGroup(System::String^ group);
        Flood::Allocator^ CreatePool(int size);
        Flood::Allocator^ CreateBump(int size);
        Flood::Allocator^ CreateHeap();
        Flood::Allocator^ CreateStack();
        static Flood::Allocator^ GetHeap();
        static Flood::Allocator^ GetStack();
        static Flood::Allocator^ GetObject(System::IntPtr _49);
        static void DumpInfo();
    };

    /// <summary>
    /// Manages memory allocation using a fixed-size object pool. When a new
    /// allocation is requested, we search for the first free object available and
    /// return it.
    /// </summary>
    public ref class PoolAllocator : Allocator
    {
    public:
        PoolAllocator(::PoolAllocator* native);
        PoolAllocator(System::IntPtr native);
        property System::IntPtr Current;
    };

    /// <summary>
    /// Manages memory allocations using a fixed-size buffer that can provide
    /// allocations and deallocations in O(1). The allocated space in the buffer is
    /// not re-used until the whole buffer is reset.
    /// </summary>
    public ref class BumpAllocator : Allocator
    {
    public:
        BumpAllocator(::BumpAllocator* native);
        BumpAllocator(System::IntPtr native);
        property System::IntPtr Start;
        property System::IntPtr Current;
        property unsigned int Size;
    };

    /// <summary>
    /// Manages memory allocation using Doug Lea's malloc implementation. This is a
    /// boundary-tag allocator that manages memory by keeping track of the
    /// used/free memory blocks.
    /// </summary>
    public ref class HeapAllocator : Allocator
    {
    public:
        HeapAllocator(::HeapAllocator* native);
        HeapAllocator(System::IntPtr native);
        property System::IntPtr Space;
    };
}
