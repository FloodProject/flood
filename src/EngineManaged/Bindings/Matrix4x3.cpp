/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Matrix4x3.h"
#include "EulerAngles.h"
#include "Quaternion.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Matrix4x3::Matrix4x3(::Matrix4x3* native)
{
    NativePtr = native;
}

Flood::Matrix4x3::Matrix4x3(System::IntPtr native)
{
    auto __native = (::Matrix4x3*)native.ToPointer();
    NativePtr = __native;
}

Flood::Matrix4x3::Matrix4x3()
{
    NativePtr = new ::Matrix4x3();
}

void Flood::Matrix4x3::Identity()
{
    ((::Matrix4x3*)NativePtr)->identity();
}

float Flood::Matrix4x3::Determinant()
{
    auto ret = ((::Matrix4x3*)NativePtr)->determinant();
    return ret;
}

Flood::Matrix4x3^ Flood::Matrix4x3::Inverse()
{
    auto ret = ((::Matrix4x3*)NativePtr)->inverse();
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::Transpose()
{
    auto ret = ((::Matrix4x3*)NativePtr)->transpose();
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Vector3 Flood::Matrix4x3::operator*(Flood::Vector3 p)
{
    auto arg0 = ::Vector3();
    auto ret = ((::Matrix4x3*)NativePtr)->operator*(arg0);
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::operator*(Flood::Matrix4x3^ m)
{
    auto &arg0 = *(::Matrix4x3*)m->NativePtr;
    auto ret = ((::Matrix4x3*)NativePtr)->operator*(arg0);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::Lerp(Flood::Matrix4x3^ a, Flood::Matrix4x3^ b, float t)
{
    auto &arg0 = *(::Matrix4x3*)a->NativePtr;
    auto &arg1 = *(::Matrix4x3*)b->NativePtr;
    auto ret = ((::Matrix4x3*)NativePtr)->lerp(arg0, arg1, t);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::CreateTranslation(Flood::Vector3 translate)
{
    auto arg0 = ::Vector3();
    auto ret = ((::Matrix4x3*)NativePtr)->createTranslation(arg0);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::CreateRotation(Flood::EulerAngles angles)
{
    auto arg0 = ::EulerAngles();
    arg0.x = angles.X;
    arg0.y = angles.Y;
    arg0.z = angles.Z;

    auto ret = ((::Matrix4x3*)NativePtr)->createRotation(arg0);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::CreateScale(Flood::Vector3 scale)
{
    auto arg0 = ::Vector3();
    auto ret = ((::Matrix4x3*)NativePtr)->createScale(arg0);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::CreateFromQuaternion(Flood::Quaternion q)
{
    auto arg0 = ::Quaternion();
    auto ret = ((::Matrix4x3*)NativePtr)->createFromQuaternion(arg0);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::RotateX(float ang)
{
    auto ret = ((::Matrix4x3*)NativePtr)->rotateX(ang);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::RotateY(float ang)
{
    auto ret = ((::Matrix4x3*)NativePtr)->rotateY(ang);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3^ Flood::Matrix4x3::RotateZ(float ang)
{
    auto ret = ((::Matrix4x3*)NativePtr)->rotateZ(ang);
    return gcnew Flood::Matrix4x3((::Matrix4x3*)&ret);
}

