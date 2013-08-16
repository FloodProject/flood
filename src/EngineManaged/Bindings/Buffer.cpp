/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Buffer.h"
#include "GeometryBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Buffer::Buffer(::Buffer* native)
{
    NativePtr = native;
}

Flood::Buffer::Buffer(System::IntPtr native)
{
    auto __native = (::Buffer*)native.ToPointer();
    NativePtr = __native;
}

Flood::Buffer::Buffer()
{
    NativePtr = new ::Buffer();
}

Flood::Buffer::Buffer(Flood::BufferUsage usage, Flood::BufferAccess access)
{
    auto arg0 = (::BufferUsage)usage;
    auto arg1 = (::BufferAccess)access;
    NativePtr = new ::Buffer(arg0, arg1);
}

bool Flood::Buffer::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Buffer^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Buffer::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Buffer::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Buffer::Instance::set(System::IntPtr object)
{
    NativePtr = (::Buffer*)object.ToPointer();
}

Flood::BufferUsage Flood::Buffer::BufferUsage::get()
{
    auto ret = ((::Buffer*)NativePtr)->getBufferUsage();
    return (Flood::BufferUsage)ret;
}

void Flood::Buffer::BufferUsage::set(Flood::BufferUsage value)
{
    auto v = value;
    auto arg0 = (::BufferUsage)v;
    ((::Buffer*)NativePtr)->setBufferUsage(arg0);
}

Flood::BufferAccess Flood::Buffer::BufferAccess::get()
{
    auto ret = ((::Buffer*)NativePtr)->getBufferAccess();
    return (Flood::BufferAccess)ret;
}

void Flood::Buffer::BufferAccess::set(Flood::BufferAccess value)
{
    auto v = value;
    auto arg0 = (::BufferAccess)v;
    ((::Buffer*)NativePtr)->setBufferAccess(arg0);
}

Flood::GeometryBuffer^ Flood::Buffer::GeometryBuffer::get()
{
    auto ret = ((::Buffer*)NativePtr)->getGeometryBuffer();
    return gcnew Flood::GeometryBuffer((::GeometryBuffer*)ret);
}

void Flood::Buffer::GeometryBuffer::set(Flood::GeometryBuffer^ value)
{
    auto v = value;
    auto arg0 = (::GeometryBuffer*)v->NativePtr;
    ((::Buffer*)NativePtr)->setGeometryBuffer(arg0);
}

unsigned int Flood::Buffer::Id::get()
{
    return ((::Buffer*)NativePtr)->id;
}

void Flood::Buffer::Id::set(unsigned int value)
{
    ((::Buffer*)NativePtr)->id = (::BufferId)(::uint32)value;
}

Flood::BufferUsage Flood::Buffer::Usage::get()
{
    return (Flood::BufferUsage)((::Buffer*)NativePtr)->usage;
}

void Flood::Buffer::Usage::set(Flood::BufferUsage value)
{
    ((::Buffer*)NativePtr)->usage = (::BufferUsage)value;
}

Flood::BufferAccess Flood::Buffer::Access::get()
{
    return (Flood::BufferAccess)((::Buffer*)NativePtr)->access;
}

void Flood::Buffer::Access::set(Flood::BufferAccess value)
{
    ((::Buffer*)NativePtr)->access = (::BufferAccess)value;
}

Flood::GeometryBuffer^ Flood::Buffer::Gb::get()
{
    return gcnew Flood::GeometryBuffer((::GeometryBuffer*)((::Buffer*)NativePtr)->gb);
}

void Flood::Buffer::Gb::set(Flood::GeometryBuffer^ value)
{
    ((::Buffer*)NativePtr)->gb = (::GeometryBuffer*)value->NativePtr;
}

