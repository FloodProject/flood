/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "IndexBuffer.h"
#include "GeometryBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::IndexBuffer::IndexBuffer(::IndexBuffer* native)
{
    NativePtr = native;
}

Flood::IndexBuffer::IndexBuffer(System::IntPtr native)
{
    NativePtr = (::IndexBuffer*)native.ToPointer();
}

Flood::IndexBuffer::IndexBuffer()
{
    NativePtr = new ::IndexBuffer();
}

