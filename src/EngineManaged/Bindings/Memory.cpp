/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Memory.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Allocator::Allocator(::Allocator* native)
{
    NativePtr = native;
}

Flood::PoolAllocator::PoolAllocator(::PoolAllocator* native)
{
    NativePtr = native;
}

Flood::BumpAllocator::BumpAllocator(::BumpAllocator* native)
{
    NativePtr = native;
}

Flood::HeapAllocator::HeapAllocator(::HeapAllocator* native)
{
    NativePtr = native;
}

Flood::Allocator^ Flood::FloodMemory::AllocatorGetHeap()
{
    return nullptr;
}

Flood::Allocator^ Flood::FloodMemory::AllocatorGetStack()
{
    return nullptr;
}

Flood::Allocator^ Flood::FloodMemory::AllocatorGetObject(System::IntPtr _0)
{
    return nullptr;
}

void Flood::FloodMemory::AllocatorDestroy(Flood::Allocator^ _1)
{
}

void Flood::FloodMemory::AllocatorReset(Flood::Allocator^ _2)
{
}

void Flood::FloodMemory::AllocatorSetGroup(Flood::Allocator^ _3, System::String^ group)
{
}

void Flood::FloodMemory::AllocatorDumpInfo()
{
}

System::IntPtr Flood::FloodMemory::AllocatorAllocate(Flood::Allocator^ _4, int size, int align)
{
    return IntPtr();
}

void Flood::FloodMemory::AllocatorDeallocate(System::IntPtr _5)
{
}

Flood::Allocator^ Flood::FloodMemory::AllocatorCreatePool(Flood::Allocator^ _6, int size)
{
    return nullptr;
}

Flood::Allocator^ Flood::FloodMemory::AllocatorCreateBump(Flood::Allocator^ _7, int size)
{
    return nullptr;
}

Flood::Allocator^ Flood::FloodMemory::AllocatorCreateHeap(Flood::Allocator^ _8)
{
    return nullptr;
}

Flood::Allocator^ Flood::FloodMemory::AllocatorCreateStack(Flood::Allocator^ _9)
{
    return nullptr;
}


