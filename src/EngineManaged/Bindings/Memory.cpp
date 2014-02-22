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

Flood::Allocator::Allocator(System::IntPtr native)
{
    auto __native = (::Allocator*)native.ToPointer();
    NativePtr = __native;
}

Flood::Allocator::Allocator()
{
    NativePtr = new ::Allocator();
}

bool Flood::Allocator::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Allocator^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Allocator::GetHashCode()
{
    return (int)NativePtr;
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

Flood::Allocator^ Flood::Allocator::CreatePool(int size)
{
    auto arg0 = (::Allocator*)NativePtr;
    auto arg1 = (::int32)(::int32_t)size;
    auto __ret = ::AllocatorCreatePool(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Allocator((::Allocator*)__ret);
}

Flood::Allocator^ Flood::Allocator::CreateBump(int size)
{
    auto arg0 = (::Allocator*)NativePtr;
    auto arg1 = (::int32)(::int32_t)size;
    auto __ret = ::AllocatorCreateBump(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Allocator((::Allocator*)__ret);
}

Flood::Allocator^ Flood::Allocator::CreateHeap()
{
    auto arg0 = (::Allocator*)NativePtr;
    auto __ret = ::AllocatorCreateHeap(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Allocator((::Allocator*)__ret);
}

Flood::Allocator^ Flood::Allocator::CreateStack()
{
    auto arg0 = (::Allocator*)NativePtr;
    auto __ret = ::AllocatorCreateStack(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Allocator((::Allocator*)__ret);
}

Flood::Allocator^ Flood::Allocator::GetHeap()
{
    auto __ret = ::AllocatorGetHeap();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Allocator((::Allocator*)__ret);
}

Flood::Allocator^ Flood::Allocator::GetStack()
{
    auto __ret = ::AllocatorGetStack();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Allocator((::Allocator*)__ret);
}

Flood::Allocator^ Flood::Allocator::GetObject(void* _0)
{
    auto arg0 = (void*)_0;
    auto __ret = ::AllocatorGetObject(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Allocator((::Allocator*)__ret);
}

void Flood::Allocator::DumpInfo()
{
    ::AllocatorDumpInfo();
}

System::IntPtr Flood::Allocator::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Allocator::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Allocator*)object.ToPointer();
}

System::String^ Flood::Allocator::Group::get()
{
    return clix::marshalString<clix::E_UTF8>(((::Allocator*)NativePtr)->group);
}

void Flood::Allocator::Group::set(System::String^ value)
{
    auto _value = clix::marshalString<clix::E_UTF8>(value);
    ((::Allocator*)NativePtr)->group = _value.c_str();
}

Flood::PoolAllocator::PoolAllocator(::PoolAllocator* native)
    : Flood::Allocator((::Allocator*)native)
{
}

Flood::PoolAllocator::PoolAllocator(System::IntPtr native)
    : Flood::Allocator(native)
{
    auto __native = (::PoolAllocator*)native.ToPointer();
}

Flood::PoolAllocator::PoolAllocator()
    : Flood::Allocator((::Allocator*)nullptr)
{
    NativePtr = new ::PoolAllocator();
}

bool Flood::PoolAllocator::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<PoolAllocator^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::PoolAllocator::GetHashCode()
{
    return (int)NativePtr;
}

unsigned char* Flood::PoolAllocator::Current::get()
{
    return ((::PoolAllocator*)NativePtr)->current;
}

void Flood::PoolAllocator::Current::set(unsigned char* value)
{
    ((::PoolAllocator*)NativePtr)->current = (::uint8*)value;
}

Flood::BumpAllocator::BumpAllocator(::BumpAllocator* native)
    : Flood::Allocator((::Allocator*)native)
{
}

Flood::BumpAllocator::BumpAllocator(System::IntPtr native)
    : Flood::Allocator(native)
{
    auto __native = (::BumpAllocator*)native.ToPointer();
}

Flood::BumpAllocator::BumpAllocator()
    : Flood::Allocator((::Allocator*)nullptr)
{
    NativePtr = new ::BumpAllocator();
}

bool Flood::BumpAllocator::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<BumpAllocator^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::BumpAllocator::GetHashCode()
{
    return (int)NativePtr;
}

unsigned char* Flood::BumpAllocator::Start::get()
{
    return ((::BumpAllocator*)NativePtr)->start;
}

void Flood::BumpAllocator::Start::set(unsigned char* value)
{
    ((::BumpAllocator*)NativePtr)->start = (::uint8*)value;
}

unsigned char* Flood::BumpAllocator::Current::get()
{
    return ((::BumpAllocator*)NativePtr)->current;
}

void Flood::BumpAllocator::Current::set(unsigned char* value)
{
    ((::BumpAllocator*)NativePtr)->current = (::uint8*)value;
}

unsigned int Flood::BumpAllocator::Size::get()
{
    return ((::BumpAllocator*)NativePtr)->size;
}

void Flood::BumpAllocator::Size::set(unsigned int value)
{
    ((::BumpAllocator*)NativePtr)->size = (::uint32)(::uint32_t)value;
}

Flood::HeapAllocator::HeapAllocator(::HeapAllocator* native)
    : Flood::Allocator((::Allocator*)native)
{
}

Flood::HeapAllocator::HeapAllocator(System::IntPtr native)
    : Flood::Allocator(native)
{
    auto __native = (::HeapAllocator*)native.ToPointer();
}

Flood::HeapAllocator::HeapAllocator()
    : Flood::Allocator((::Allocator*)nullptr)
{
    NativePtr = new ::HeapAllocator();
}

bool Flood::HeapAllocator::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<HeapAllocator^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::HeapAllocator::GetHashCode()
{
    return (int)NativePtr;
}

void* Flood::HeapAllocator::Space::get()
{
    return ((::HeapAllocator*)NativePtr)->space;
}

void Flood::HeapAllocator::Space::set(void* value)
{
    ((::HeapAllocator*)NativePtr)->space = (void*)value;
}

