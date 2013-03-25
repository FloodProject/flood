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

void Flood::UniformBuffer::SetUniform(System::String^ slot, System::Collections::Generic::List<Flood::Vector3>^ vec)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto _tmpvec = std::vector<::Vector3>();
    for each(Flood::Vector3 _element in vec)
    {
        auto _marshal0 = ::Vector3();
        _marshal0.x = _element.X;
        _marshal0.y = _element.Y;
        _marshal0.z = _element.Z;
        auto _marshalElement = _marshal0;
        _tmpvec.push_back(_marshalElement);
    }
    auto arg1 = _tmpvec;
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, System::Collections::Generic::List<Flood::Color>^ vec)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto _tmpvec = std::vector<::Color>();
    for each(Flood::Color _element in vec)
    {
        auto _marshal0 = ::Color();
        _marshal0.r = _element.R;
        _marshal0.g = _element.G;
        _marshal0.b = _element.B;
        _marshal0.a = _element.A;
        auto _marshalElement = _marshal0;
        _tmpvec.push_back(_marshalElement);
    }
    auto arg1 = _tmpvec;
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

void Flood::UniformBuffer::SetUniform(System::String^ slot, Flood::Vector3 vec)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto _marshal1 = ::Vector3();
    _marshal1.x = vec.X;
    _marshal1.y = vec.Y;
    _marshal1.z = vec.Z;
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

void Flood::UniformBuffer::SetUniform(System::String^ slot, System::Collections::Generic::List<Flood::Matrix4x4>^ vec)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(slot);
    auto arg0 = _arg0.c_str();
    auto _tmpvec = std::vector<::Matrix4x4>();
    for each(Flood::Matrix4x4 _element in vec)
    {
        auto _marshal0 = ::Matrix4x4();
        _marshal0.m11 = _element.M11;
        _marshal0.m12 = _element.M12;
        _marshal0.m13 = _element.M13;
        _marshal0.m14 = _element.M14;
        _marshal0.m21 = _element.M21;
        _marshal0.m22 = _element.M22;
        _marshal0.m23 = _element.M23;
        _marshal0.m24 = _element.M24;
        _marshal0.m31 = _element.M31;
        _marshal0.m32 = _element.M32;
        _marshal0.m33 = _element.M33;
        _marshal0.m34 = _element.M34;
        _marshal0.tx = _element.Tx;
        _marshal0.ty = _element.Ty;
        _marshal0.tz = _element.Tz;
        _marshal0.tw = _element.Tw;
        auto _marshalElement = _marshal0;
        _tmpvec.push_back(_marshalElement);
    }
    auto arg1 = _tmpvec;
    ((::UniformBuffer*)NativePtr)->setUniform(arg0, arg1);
}

