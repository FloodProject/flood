/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Memory.h>

namespace Flood
{
    ref class Allocator;
    ref class BumpAllocator;
    ref class HeapAllocator;
    ref class PoolAllocator;
}

namespace Flood
{
    public delegate void* MemoryAllocateFunction(Flood::Allocator^ _0, int size, int align);

    public delegate void MemoryFreeFunction(Flood::Allocator^ _0, void* object);

    public delegate void MemoryResetFunction(Flood::Allocator^ _0);

    /// <summary>
    /// Interface for a custom memory allocator.
    /// </summary>
    public ref class Allocator : ICppInstance
    {
    public:

        property ::Allocator* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Allocator(::Allocator* native);
        Allocator(System::IntPtr native);
        Allocator();

        property System::String^ Group
        {
            System::String^ get();
            void set(System::String^);
        }

        property char Id
        {
            char get();
            void set(char);
        }

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

        void Destroy();

        void ResetMemory();

        void SetGroup(System::String^ group);

        Flood::Allocator^ CreatePool(int size);

        Flood::Allocator^ CreateBump(int size);

        Flood::Allocator^ CreateHeap();

        Flood::Allocator^ CreateStack();

        static Flood::Allocator^ GetHeap();

        static Flood::Allocator^ GetStack();

        static Flood::Allocator^ GetObject(void* _0);

        static void DumpInfo();
    };

    /// <summary>
    /// Manages memory allocation using a fixed-size object pool. When a new
    /// allocation is requested, we search for the first free object available and
    /// return it.
    /// </summary>
    public ref class PoolAllocator : Flood::Allocator
    {
    public:

        PoolAllocator(::PoolAllocator* native);
        PoolAllocator(System::IntPtr native);
        PoolAllocator();

        property unsigned char* Current
        {
            unsigned char* get();
            void set(unsigned char*);
        }

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    /// <summary>
    /// Manages memory allocations using a fixed-size buffer that can provide
    /// allocations and deallocations in O(1). The allocated space in the buffer is
    /// not re-used until the whole buffer is reset.
    /// </summary>
    public ref class BumpAllocator : Flood::Allocator
    {
    public:

        BumpAllocator(::BumpAllocator* native);
        BumpAllocator(System::IntPtr native);
        BumpAllocator();

        property unsigned char* Start
        {
            unsigned char* get();
            void set(unsigned char*);
        }

        property unsigned char* Current
        {
            unsigned char* get();
            void set(unsigned char*);
        }

        property unsigned int Size
        {
            unsigned int get();
            void set(unsigned int);
        }

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    /// <summary>
    /// Manages memory allocation using Doug Lea's malloc implementation. This is a
    /// boundary-tag allocator that manages memory by keeping track of the
    /// used/free memory blocks.
    /// </summary>
    public ref class HeapAllocator : Flood::Allocator
    {
    public:

        HeapAllocator(::HeapAllocator* native);
        HeapAllocator(System::IntPtr native);
        HeapAllocator();

        property void* Space
        {
            void* get();
            void set(void*);
        }

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
