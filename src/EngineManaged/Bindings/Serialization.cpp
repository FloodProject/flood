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
#include "Stream.h"

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

Flood::ReflectionDeserializeHandleFn^ Flood::ReflectionHandleContext::Deserialize::get()
{
    return safe_cast<Flood::ReflectionDeserializeHandleFn^>(System::Runtime::InteropServices::Marshal::GetDelegateForFunctionPointer(IntPtr(((::ReflectionHandleContext*)NativePtr)->deserialize), Flood::ReflectionDeserializeHandleFn::typeid));
}

void Flood::ReflectionHandleContext::Deserialize::set(Flood::ReflectionDeserializeHandleFn^ value)
{
    ((::ReflectionHandleContext*)NativePtr)->deserialize = static_cast<::ReflectionDeserializeHandleFn>(System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(value).ToPointer());
}

Flood::Serializer::Serializer(::Serializer* native)
{
    NativePtr = native;
}

Flood::Serializer::Serializer(System::IntPtr native)
{
    auto __native = (::Serializer*)native.ToPointer();
    NativePtr = __native;
}

Flood::Serializer::Serializer()
{
    NativePtr = new ::Serializer();
}

void Flood::Serializer::Destroy()
{
    auto arg0 = (::Serializer*)NativePtr;
    ::SerializerDestroy(arg0);
}

Flood::Allocator^ Flood::Serializer::Alloc::get()
{
    return gcnew Flood::Allocator((::Allocator*)((::Serializer*)NativePtr)->alloc);
}

void Flood::Serializer::Alloc::set(Flood::Allocator^ value)
{
    ((::Serializer*)NativePtr)->alloc = (::Allocator*)value->NativePtr;
}

Flood::Stream^ Flood::Serializer::Stream::get()
{
    return gcnew Flood::Stream((::Stream*)((::Serializer*)NativePtr)->stream);
}

void Flood::Serializer::Stream::set(Flood::Stream^ value)
{
    ((::Serializer*)NativePtr)->stream = (::Stream*)value->NativePtr;
}

