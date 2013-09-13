/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Ray.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Ray::Ray(::Ray* native)
{
    Origin = Flood::Vector3((::Vector3*)&native->origin);
    Direction = Flood::Vector3((::Vector3*)&native->direction);
}

Flood::Ray::Ray(System::IntPtr native)
{
    auto __native = (::Ray*)native.ToPointer();
    Origin = Flood::Vector3((::Vector3*)&__native->origin);
    Direction = Flood::Vector3((::Vector3*)&__native->direction);
}

Flood::Ray::Ray(Flood::Vector3 origin, Flood::Vector3 direction)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = origin.X;
    _marshal0.y = origin.Y;
    _marshal0.z = origin.Z;
    auto _marshal1 = ::Vector3();
    _marshal1.x = direction.X;
    _marshal1.y = direction.Y;
    _marshal1.z = direction.Z;
    ::Ray _native(_marshal0, _marshal1);
    this->Origin = Flood::Vector3((::Vector3*)&_native.origin);
    this->Direction = Flood::Vector3((::Vector3*)&_native.direction);
}

Flood::Vector3 Flood::Ray::GetPoint(float distance)
{
    auto _this0 = ::Ray();
    auto _this0_marshal0 = ::Vector3();
    _this0_marshal0.x = (*this).Origin.X;
    _this0_marshal0.y = (*this).Origin.Y;
    _this0_marshal0.z = (*this).Origin.Z;
    _this0.origin = _this0_marshal0;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).Direction.X;
    _this0_marshal1.y = (*this).Direction.Y;
    _this0_marshal1.z = (*this).Direction.Z;
    _this0.direction = _this0_marshal1;
    auto __ret = _this0.getPoint(distance);
    Origin = Flood::Vector3((::Vector3*)&_this0.origin);
    Direction = Flood::Vector3((::Vector3*)&_this0.direction);
    return Flood::Vector3((::Vector3*)&__ret);
}

