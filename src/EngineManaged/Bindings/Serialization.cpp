/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Serialization.h"
#include "Memory.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::ReflectionHandleContext::ReflectionHandleContext(::ReflectionHandleContext* native)
{
    NativePtr = native;
}

Flood::ReflectionHandleContext::ReflectionHandleContext(System::IntPtr native)
{
    auto __native = (::ReflectionHandleContext*)native.ToPointer();
    NativePtr = __native;
}

Flood::ReflectionHandleContext::ReflectionHandleContext()
{
    NativePtr = new ::ReflectionHandleContext();
}

