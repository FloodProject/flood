/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "IndexBuffer.h"
#include "GeometryBuffer.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::IndexBuffer::IndexBuffer(::IndexBuffer* native)
    : Buffer(native)
{
}

Flood::IndexBuffer::IndexBuffer(System::IntPtr native)
    : Buffer(native)
{
    auto __native = (::IndexBuffer*)native.ToPointer();
}

Flood::IndexBuffer::IndexBuffer()
    : Buffer(nullptr)
{
    NativePtr = new ::IndexBuffer();
}

