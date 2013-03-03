/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "UniformBuffer.h"
#include "Vector.h"
#include "Matrix4x3.h"
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
    auto ret = ((::UniformBuffer*)NativePtr)->getElement(arg0, arg1);
    return gcnew Flood::UniformBufferElement((::UniformBufferElement*)ret);
}

void Flood::UniformBuffer::RemoveUniform(System::String^ slot)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    ((::UniformBuffer*)NativePtr)->removeUniform(arg0);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, int data)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto arg1 = (int32)data;
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, float value)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, value);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, Flood::Vector3 vec)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto arg1 = ::Vector3();
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, Flood::Matrix4x3^ _198)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto &arg1 = *(::Matrix4x3*)_198->NativePtr;
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, Flood::Matrix4x4 _199)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto arg1 = ::Matrix4x4();
    arg1.m11 = _199.M11;
    arg1.m12 = _199.M12;
    arg1.m13 = _199.M13;
    arg1.m14 = _199.M14;
    arg1.m21 = _199.M21;
    arg1.m22 = _199.M22;
    arg1.m23 = _199.M23;
    arg1.m24 = _199.M24;
    arg1.m31 = _199.M31;
    arg1.m32 = _199.M32;
    arg1.m33 = _199.M33;
    arg1.m34 = _199.M34;
    arg1.tx = _199.Tx;
    arg1.ty = _199.Ty;
    arg1.tz = _199.Tz;
    arg1.tw = _199.Tw;

    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

