/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Plane.h"
#include "Ray.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Plane::Plane(::Plane* native)
{
    Normal = Flood::Vector3((::Vector3*)&native->normal);
    Offset = native->offset;
}

Flood::Plane::Plane(System::IntPtr native)
{
    auto __native = (::Plane*)native.ToPointer();
    Normal = Flood::Vector3((::Vector3*)&__native->normal);
    Offset = __native->offset;
}

Flood::Plane::Plane(Flood::Vector3 normal, Flood::Vector3 point)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = normal.X;
    _marshal0.y = normal.Y;
    _marshal0.z = normal.Z;
    auto _marshal1 = ::Vector3();
    _marshal1.x = point.X;
    _marshal1.y = point.Y;
    _marshal1.z = point.Z;
    ::Plane _native(_marshal0, _marshal1);
    this->Normal = Flood::Vector3((::Vector3*)&_native.normal);
    this->Offset = _native.offset;
}

Flood::Plane::Plane(Flood::Vector3 normal, float distance)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = normal.X;
    _marshal0.y = normal.Y;
    _marshal0.z = normal.Z;
    ::Plane _native(_marshal0, distance);
    this->Normal = Flood::Vector3((::Vector3*)&_native.normal);
    this->Offset = _native.offset;
}

void Flood::Plane::Normalize()
{
    auto _this0 = ::Plane();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Normal.X;
    _this0_marshal0.y = (*this).Normal.Y;
    _this0_marshal0.z = (*this).Normal.Z;
    _this0.normal = _this0_marshal0;
    _this0.offset = (*this).Offset;
    _this0.normalize();
    Normal = Flood::Vector3((::Vector3*)&_this0.normal);
    Offset = _this0.offset;
}

float Flood::Plane::Distance(Flood::Vector3 point)
{
    auto _this0 = ::Plane();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Normal.X;
    _this0_marshal0.y = (*this).Normal.Y;
    _this0_marshal0.z = (*this).Normal.Z;
    _this0.normal = _this0_marshal0;
    _this0.offset = (*this).Offset;
    auto _marshal0 = ::Vector3();
    _marshal0.x = point.X;
    _marshal0.y = point.Y;
    _marshal0.z = point.Z;
    auto arg0 = _marshal0;
    auto __ret = _this0.distance(arg0);
    Normal = Flood::Vector3((::Vector3*)&_this0.normal);
    Offset = _this0.offset;
    return __ret;
}

float Flood::Plane::Distance(float x, float y, float z)
{
    auto _this0 = ::Plane();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Normal.X;
    _this0_marshal0.y = (*this).Normal.Y;
    _this0_marshal0.z = (*this).Normal.Z;
    _this0.normal = _this0_marshal0;
    _this0.offset = (*this).Offset;
    auto __ret = _this0.distance(x, y, z);
    Normal = Flood::Vector3((::Vector3*)&_this0.normal);
    Offset = _this0.offset;
    return __ret;
}

Flood::Vector3 Flood::Plane::Project(Flood::Vector3 vec)
{
    auto _this0 = ::Plane();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Normal.X;
    _this0_marshal0.y = (*this).Normal.Y;
    _this0_marshal0.z = (*this).Normal.Z;
    _this0.normal = _this0_marshal0;
    _this0.offset = (*this).Offset;
    auto _marshal0 = ::Vector3();
    _marshal0.x = vec.X;
    _marshal0.y = vec.Y;
    _marshal0.z = vec.Z;
    auto arg0 = _marshal0;
    auto __ret = _this0.project(arg0);
    Normal = Flood::Vector3((::Vector3*)&_this0.normal);
    Offset = _this0.offset;
    return Flood::Vector3((::Vector3*)&__ret);
}

bool Flood::Plane::Intersects(Flood::Ray ray, [System::Runtime::InteropServices::Out] float% distance)
{
    auto _this0 = ::Plane();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Normal.X;
    _this0_marshal0.y = (*this).Normal.Y;
    _this0_marshal0.z = (*this).Normal.Z;
    _this0.normal = _this0_marshal0;
    _this0.offset = (*this).Offset;
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
    Normal = Flood::Vector3((::Vector3*)&_this0.normal);
    Offset = _this0.offset;
    return __ret;
}

