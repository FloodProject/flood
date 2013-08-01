/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "IndexBuffer.h"
#include "GeometryBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::IndexBuffer::IndexBuffer(::IndexBuffer* native)
    : Flood::Buffer(native)
{
}

Flood::IndexBuffer::IndexBuffer(System::IntPtr native)
    : Flood::Buffer(native)
{
    auto __native = (::IndexBuffer*)native.ToPointer();
}

Flood::IndexBuffer::IndexBuffer()
    : Flood::Buffer(nullptr)
{
    NativePtr = new ::IndexBuffer();
}

bool Flood::IndexBuffer::Equals(System::Object^ object)
{
    if (!object) return false;
    return Instance == safe_cast<ICppInstance^>(object)->Instance;
}

int Flood::IndexBuffer::GetHashCode()
{
    return (int)NativePtr;
}

bool Flood::IndexBuffer::IsBuilt::get()
{
    return ((::IndexBuffer*)NativePtr)->isBuilt;
}

void Flood::IndexBuffer::IsBuilt::set(bool value)
{
    ((::IndexBuffer*)NativePtr)->isBuilt = value;
}

