/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "UniformBuffer.h"
#include "Vector.h"
#include "Matrix4x4.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::UniformBufferElement::UniformBufferElement(::UniformBufferElement* native)
{
    NativePtr = native;
}

Flood::UniformBufferElement::UniformBufferElement(System::IntPtr native)
{
    auto __native = (::UniformBufferElement*)native.ToPointer();
    NativePtr = __native;
}

Flood::UniformBuffer::UniformBuffer(::UniformBuffer* native)
{
    NativePtr = native;
}

Flood::UniformBuffer::UniformBuffer(System::IntPtr native)
{
    auto __native = (::UniformBuffer*)native.ToPointer();
    NativePtr = __native;
}

Flood::UniformBufferElement^ Flood::UniformBuffer::GetElement(System::String^ name, unsigned int size)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(name);
    auto arg0 = _arg0.c_str();
    auto arg1 = (size_t)size;
    auto ret = NativePtr->getElement(arg0, arg1);
    return gcnew Flood::UniformBufferElement((::UniformBufferElement*)ret);
}

void Flood::UniformBuffer::RemoveUniform(System::String^ slot)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    NativePtr->removeUniform(arg0);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, int data)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto arg1 = (int32)data;
    NativePtr->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, float value)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    NativePtr->setUniform(arg0, value);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, Flood::Vector3 vec)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto _arg1 = (::Vector3*)&vec;
    auto arg1 = *_arg1;
    NativePtr->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, Flood::Matrix4x4 _196)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto _arg1 = (::Matrix4x4*)&_196;
    auto arg1 = *_arg1;
    NativePtr->setUniform(arg0, arg1);
}

