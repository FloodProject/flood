/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "BoundingBox.h"
#include "Matrix4x3.h"
#include "Ray.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::BoundingBox::BoundingBox(::BoundingBox* native)
{
    Min = Flood::Vector3((::Vector3*)&native->min);
    Max = Flood::Vector3((::Vector3*)&native->max);
}

Flood::BoundingBox::BoundingBox(System::IntPtr native)
{
    auto __native = (::BoundingBox*)native.ToPointer();
    Min = Flood::Vector3((::Vector3*)&__native->min);
    Max = Flood::Vector3((::Vector3*)&__native->max);
}

Flood::BoundingBox::BoundingBox(Flood::Vector3 min, Flood::Vector3 max)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = min.X;
    _marshal0.y = min.Y;
    _marshal0.z = min.Z;
    auto _marshal1 = ::Vector3();
    _marshal1.x = max.X;
    _marshal1.y = max.Y;
    _marshal1.z = max.Z;
    ::BoundingBox _native(_marshal0, _marshal1);
    this->Min = Flood::Vector3((::Vector3*)&_native.min);
    this->Max = Flood::Vector3((::Vector3*)&_native.max);
}

void Flood::BoundingBox::Add(Flood::Vector3 vertex)
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    auto _marshal0 = ::Vector3();
    _marshal0.x = vertex.X;
    _marshal0.y = vertex.Y;
    _marshal0.z = vertex.Z;
    auto arg0 = _marshal0;
    _this0.add(arg0);
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
}

void Flood::BoundingBox::Add(Flood::BoundingBox box)
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    auto _marshal0 = ::BoundingBox();
    auto _marshal1 = ::Vector3();
    _marshal1.x = box.Min.X;
    _marshal1.y = box.Min.Y;
    _marshal1.z = box.Min.Z;
    _marshal0.min = _marshal1;
    auto _marshal2 = ::Vector3();
    _marshal2.x = box.Max.X;
    _marshal2.y = box.Max.Y;
    _marshal2.z = box.Max.Z;
    _marshal0.max = _marshal2;
    auto arg0 = _marshal0;
    _this0.add(arg0);
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
}

void Flood::BoundingBox::Reset()
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    _this0.reset();
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
}

void Flood::BoundingBox::SetZero()
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    _this0.setZero();
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
}

bool Flood::BoundingBox::IsInfinite()
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    auto __ret = _this0.isInfinite();
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
    return __ret;
}

Flood::Vector3 Flood::BoundingBox::GetCorner(int index)
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    auto __ret = _this0.getCorner(index);
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
    return Flood::Vector3((::Vector3*)&__ret);
}

bool Flood::BoundingBox::Intersects(Flood::Ray ray, [System::Runtime::InteropServices::Out] float% distance)
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    auto _marshal0 = ::Ray();
    auto _marshal1 = ::Vector3();
    _marshal1.x = ray.Origin.X;
    _marshal1.y = ray.Origin.Y;
    _marshal1.z = ray.Origin.Z;
    _marshal0.origin = _marshal1;
    auto _marshal2 = ::Vector3();
    _marshal2.x = ray.Direction.X;
    _marshal2.y = ray.Direction.Y;
    _marshal2.z = ray.Direction.Z;
    _marshal0.direction = _marshal2;
    auto arg0 = _marshal0;
    float arg1;
    auto __ret = _this0.intersects(arg0, arg1);
    distance = arg1;
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
    return __ret;
}

Flood::BoundingBox Flood::BoundingBox::Transform(Flood::Matrix4x3 _0)
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = _0.M11;
    _marshal0.m12 = _0.M12;
    _marshal0.m13 = _0.M13;
    _marshal0.m21 = _0.M21;
    _marshal0.m22 = _0.M22;
    _marshal0.m23 = _0.M23;
    _marshal0.m31 = _0.M31;
    _marshal0.m32 = _0.M32;
    _marshal0.m33 = _0.M33;
    _marshal0.tx = _0.Tx;
    _marshal0.ty = _0.Ty;
    _marshal0.tz = _0.Tz;
    auto arg0 = _marshal0;
    auto __ret = _this0.transform(arg0);
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
    return Flood::BoundingBox((::BoundingBox*)&__ret);
}

Flood::Vector3 Flood::BoundingBox::Center::get()
{
    auto _this0 = ::BoundingBox();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Min.X;
    _this0_marshal0.y = (*this).Min.Y;
    _this0_marshal0.z = (*this).Min.Z;
    _this0.min = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Max.X;
    _this0_marshal1.y = (*this).Max.Y;
    _this0_marshal1.z = (*this).Max.Z;
    _this0.max = _this0_marshal1;
    auto __ret = _this0.getCenter();
    Min = Flood::Vector3((::Vector3*)&_this0.min);
    Max = Flood::Vector3((::Vector3*)&_this0.max);
    return Flood::Vector3((::Vector3*)&__ret);
}

