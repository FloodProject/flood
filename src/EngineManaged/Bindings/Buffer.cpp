/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Buffer.h"
#include "GeometryBuffer.h"
#include "VertexBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Buffer::Buffer(::Buffer* native)
{
    NativePtr = native;
}

Flood::Buffer::Buffer(System::IntPtr native)
{
    NativePtr = (::Buffer*)native.ToPointer();
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

Flood::BufferUsage Flood::Buffer::GetBufferUsage()
{
    auto ret = NativePtr->getBufferUsage();
    return (Flood::BufferUsage)ret;
}

void Flood::Buffer::SetBufferUsage(Flood::BufferUsage v)
{
    auto arg0 = (::BufferUsage)v;
    NativePtr->setBufferUsage(arg0);
}

Flood::BufferAccess Flood::Buffer::GetBufferAccess()
{
    auto ret = NativePtr->getBufferAccess();
    return (Flood::BufferAccess)ret;
}

void Flood::Buffer::SetBufferAccess(Flood::BufferAccess v)
{
    auto arg0 = (::BufferAccess)v;
    NativePtr->setBufferAccess(arg0);
}

Flood::GeometryBuffer^ Flood::Buffer::GetGeometryBuffer()
{
    auto ret = NativePtr->getGeometryBuffer();
    return gcnew Flood::GeometryBuffer((::GeometryBuffer*)ret);
}

void Flood::Buffer::SetGeometryBuffer(Flood::GeometryBuffer^ v)
{
    auto arg0 = v->NativePtr;
    NativePtr->setGeometryBuffer(arg0);
}

