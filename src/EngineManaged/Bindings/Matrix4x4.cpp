/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Matrix4x4.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Matrix4x4::Matrix4x4(::Matrix4x4* native)
{
    // TODO: Struct marshaling
}

Flood::Matrix4x4::Matrix4x4(System::IntPtr native)
{
    // TODO: Struct marshaling
}

void Flood::Matrix4x4::Identity()
{
    auto this0 = (::Matrix4x4*) 0;
    this0->identity();
}

Flood::Matrix4x4 Flood::Matrix4x4::Inverse()
{
    auto this0 = (::Matrix4x4*) 0;
    auto ret = this0->inverse();
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::Transpose()
{
    auto this0 = (::Matrix4x4*) 0;
    auto ret = this0->transpose();
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::operator*(Flood::Matrix4x4 m)
{
    auto this0 = (::Matrix4x4*) 0;
    auto _arg0 = (::Matrix4x4*)&m;
    auto arg0 = *_arg0;
    auto ret = this0->operator*(arg0);
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Vector3 Flood::Matrix4x4::operator*(Flood::Vector3 v)
{
    auto this0 = (::Matrix4x4*) 0;
    auto _arg0 = (::Vector3*)&v;
    auto arg0 = *_arg0;
    auto ret = this0->operator*(arg0);
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector4 Flood::Matrix4x4::operator*(Flood::Vector4 v)
{
    auto this0 = (::Matrix4x4*) 0;
    auto _arg0 = (::Vector4*)&v;
    auto arg0 = *_arg0;
    auto ret = this0->operator*(arg0);
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::CreatePerspective(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
    auto this0 = (::Matrix4x4*) 0;
    auto ret = this0->createPerspective(FOV, aspectRatio, nearPlane, farPlane);
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::CreateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    auto this0 = (::Matrix4x4*) 0;
    auto ret = this0->createOrthographic(left, right, bottom, top, nearPlane, farPlane);
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

