/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Serialization.h"
#include "Reflection.h"
#include "Memory.h"
#include "Stream.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::ValueContext::ValueContext(::ValueContext* native)
{
    NativePtr = native;
}

Flood::ValueContext::ValueContext(System::IntPtr native)
{
    NativePtr = (::ValueContext*)native.ToPointer();
}

Flood::ReflectionHandleContext::ReflectionHandleContext(::ReflectionHandleContext* native)
{
    NativePtr = native;
}

Flood::ReflectionHandleContext::ReflectionHandleContext(System::IntPtr native)
{
    NativePtr = (::ReflectionHandleContext*)native.ToPointer();
}

Flood::ReflectionContext::ReflectionContext(::ReflectionContext* native)
{
    NativePtr = native;
}

Flood::ReflectionContext::ReflectionContext(System::IntPtr native)
{
    NativePtr = (::ReflectionContext*)native.ToPointer();
}

Flood::ReflectionContext::ReflectionContext()
{
    NativePtr = new ::ReflectionContext();
}

Flood::Serializer::Serializer(::Serializer* native)
{
    NativePtr = native;
}

Flood::Serializer::Serializer(System::IntPtr native)
{
    NativePtr = (::Serializer*)native.ToPointer();
}

Flood::Serializer::Serializer()
{
    NativePtr = new ::Serializer();
}

void Flood::Serializer::Destroy()
{
    auto arg0 = NativePtr;
    ::SerializerDestroy(arg0);
}

void Flood::FloodSerialization::ReflectionWalkComposite(Flood::ReflectionContext^ _112)
{
    auto arg0 = _112->NativePtr;
    ::ReflectionWalkComposite(arg0);
}

void Flood::FloodSerialization::ReflectionWalkCompositeField(Flood::ReflectionContext^ _113)
{
    auto arg0 = _113->NativePtr;
    ::ReflectionWalkCompositeField(arg0);
}

