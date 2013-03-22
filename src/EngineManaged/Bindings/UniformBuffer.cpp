/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "UniformBuffer.h"
#include "Color.h"
#include "Matrix4x3.h"
#include "Matrix4x4.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::UniformBufferElement::UniformBufferElement(::UniformBufferElement* native)
{
    Name = clix::marshalString<clix::E_UTF8>(native->name);
    Type = (Flood::UniformDataType)native->type;
    Count = native->count;
    Data = nullptr;
}

Flood::UniformBufferElement::UniformBufferElement(System::IntPtr native)
{
    auto __native = (::UniformBufferElement*)native.ToPointer();
    Name = clix::marshalString<clix::E_UTF8>(__native->name);
    Type = (Flood::UniformDataType)__native->type;
    Count = __native->count;
    Data = nullptr;
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

Flood::UniformBufferElement Flood::UniformBuffer::GetElement(System::String^ name, unsigned int size)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(name);
    auto arg0 = _arg0.c_str();
    auto arg1 = (size_t)size;
    auto ret = ((::UniformBuffer*)NativePtr)->getElement(arg0, arg1);
    return Flood::UniformBufferElement((::UniformBufferElement*)ret);
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
    auto _marshal1 = ::Vector3();

    auto arg1 = _marshal1;
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, Flood::Matrix4x3 _0)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto _marshal1 = ::Matrix4x3();

    auto arg1 = _marshal1;
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, Flood::Matrix4x4 _0)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto _marshal1 = ::Matrix4x4();
    _marshal1.m11 = _0.M11;
    _marshal1.m12 = _0.M12;
    _marshal1.m13 = _0.M13;
    _marshal1.m14 = _0.M14;
    _marshal1.m21 = _0.M21;
    _marshal1.m22 = _0.M22;
    _marshal1.m23 = _0.M23;
    _marshal1.m24 = _0.M24;
    _marshal1.m31 = _0.M31;
    _marshal1.m32 = _0.M32;
    _marshal1.m33 = _0.M33;
    _marshal1.m34 = _0.M34;
    _marshal1.tx = _0.Tx;
    _marshal1.ty = _0.Ty;
    _marshal1.tz = _0.Tz;
    _marshal1.tw = _0.Tw;

    auto arg1 = _marshal1;
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

