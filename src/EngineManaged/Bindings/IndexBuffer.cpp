/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "IndexBuffer.h"
#include "Buffer.h"
#include "GeometryBuffer.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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

bool Flood::IndexBuffer::IsBuilt::get()
{
    return ((::IndexBuffer*)NativePtr)->isBuilt;
}

void Flood::IndexBuffer::IsBuilt::set(bool value)
{
    ((::IndexBuffer*)NativePtr)->isBuilt = value;
}

