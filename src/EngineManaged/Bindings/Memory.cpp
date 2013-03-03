/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Memory.h"
#include "Reflection.h"
#include "Serialization.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Allocator::Allocator(::Allocator* native)
{
    NativePtr = native;
}

Flood::Allocator::Allocator(System::IntPtr native)
{
    auto __native = (::Allocator*)native.ToPointer();
    NativePtr = __native;
}

Flood::Allocator^ Flood::Allocator::GetHeap()
{
    auto ret = ::AllocatorGetHeap();
    return gcnew Flood::Allocator((::Allocator*)ret);
}

Flood::Allocator^ Flood::Allocator::GetStack()
{
    auto ret = ::AllocatorGetStack();
    return gcnew Flood::Allocator((::Allocator*)ret);
}

Flood::Allocator^ Flood::Allocator::GetObject(System::IntPtr _49)
{
    auto arg0 = _49.ToPointer();
    auto ret = ::AllocatorGetObject(arg0);
    return gcnew Flood::Allocator((::Allocator*)ret);
}

void Flood::Allocator::Destroy()
{
    auto arg0 = (::Allocator*)NativePtr;
    ::AllocatorDestroy(arg0);
}

void Flood::Allocator::ResetMemory()
{
    auto arg0 = (::Allocator*)NativePtr;
    ::AllocatorReset(arg0);
}

void Flood::Allocator::SetGroup(System::String^ group)
{
    auto arg0 = (::Allocator*)NativePtr;
    auto _arg1 = clix::marshalString<clix::E_UTF8>(group);
    auto arg1 = _arg1.c_str();
    ::AllocatorSetGroup(arg0, arg1);
}

void Flood::Allocator::DumpInfo()
{
    ::AllocatorDumpInfo();
}

Flood::Allocator^ Flood::Allocator::CreatePool(int size)
{
    auto arg0 = (::Allocator*)NativePtr;
    auto arg1 = (int32)size;
    auto ret = ::AllocatorCreatePool(arg0, arg1);
    return gcnew Flood::Allocator((::Allocator*)ret);
}

Flood::Allocator^ Flood::Allocator::CreateBump(int size)
{
    auto arg0 = (::Allocator*)NativePtr;
    auto arg1 = (int32)size;
    auto ret = ::AllocatorCreateBump(arg0, arg1);
    return gcnew Flood::Allocator((::Allocator*)ret);
}

Flood::Allocator^ Flood::Allocator::CreateHeap()
{
    auto arg0 = (::Allocator*)NativePtr;
    auto ret = ::AllocatorCreateHeap(arg0);
    return gcnew Flood::Allocator((::Allocator*)ret);
}

Flood::Allocator^ Flood::Allocator::CreateStack()
{
    auto arg0 = (::Allocator*)NativePtr;
    auto ret = ::AllocatorCreateStack(arg0);
    return gcnew Flood::Allocator((::Allocator*)ret);
}

Flood::PoolAllocator::PoolAllocator(::PoolAllocator* native)
    : Allocator(native)
{
}

Flood::PoolAllocator::PoolAllocator(System::IntPtr native)
    : Allocator(native)
{
    auto __native = (::PoolAllocator*)native.ToPointer();
}

Flood::BumpAllocator::BumpAllocator(::BumpAllocator* native)
    : Allocator(native)
{
}

Flood::BumpAllocator::BumpAllocator(System::IntPtr native)
    : Allocator(native)
{
    auto __native = (::BumpAllocator*)native.ToPointer();
}

Flood::HeapAllocator::HeapAllocator(::HeapAllocator* native)
    : Allocator(native)
{
}

Flood::HeapAllocator::HeapAllocator(System::IntPtr native)
    : Allocator(native)
{
    auto __native = (::HeapAllocator*)native.ToPointer();
}

