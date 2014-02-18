/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Frustum.h"
#include "BoundingBox.h"
#include "Matrix4x3.h"
#include "Plane.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Frustum::Frustum(::Frustum* native)
{
    __Projection = (Flood::FrustumProjection)native->projection;
    MatProjection = Flood::Matrix4x4((::Matrix4x4*)&native->matProjection);
    __FieldOfView = native->fieldOfView;
    __NearPlane = native->nearPlane;
    __FarPlane = native->farPlane;
    __AspectRatio = native->aspectRatio;
    OrthoSize = Flood::Vector3((::Vector3*)&native->orthoSize);
}

Flood::Frustum::Frustum(System::IntPtr native)
{
    auto __native = (::Frustum*)native.ToPointer();
    __Projection = (Flood::FrustumProjection)__native->projection;
    MatProjection = Flood::Matrix4x4((::Matrix4x4*)&__native->matProjection);
    __FieldOfView = __native->fieldOfView;
    __NearPlane = __native->nearPlane;
    __FarPlane = __native->farPlane;
    __AspectRatio = __native->aspectRatio;
    OrthoSize = Flood::Vector3((::Vector3*)&__native->orthoSize);
}

void Flood::Frustum::UpdateProjection()
{
    auto _this0 = ::Frustum();
    _this0.projection = (::FrustumProjection)(*this).Projection;
    auto _this0_marshal0 = ::Matrix4x4();
    _this0_marshal0.m11 = (*this).MatProjection.M11;
    _this0_marshal0.m12 = (*this).MatProjection.M12;
    _this0_marshal0.m13 = (*this).MatProjection.M13;
    _this0_marshal0.m14 = (*this).MatProjection.M14;
    _this0_marshal0.m21 = (*this).MatProjection.M21;
    _this0_marshal0.m22 = (*this).MatProjection.M22;
    _this0_marshal0.m23 = (*this).MatProjection.M23;
    _this0_marshal0.m24 = (*this).MatProjection.M24;
    _this0_marshal0.m31 = (*this).MatProjection.M31;
    _this0_marshal0.m32 = (*this).MatProjection.M32;
    _this0_marshal0.m33 = (*this).MatProjection.M33;
    _this0_marshal0.m34 = (*this).MatProjection.M34;
    _this0_marshal0.tx = (*this).MatProjection.Tx;
    _this0_marshal0.ty = (*this).MatProjection.Ty;
    _this0_marshal0.tz = (*this).MatProjection.Tz;
    _this0_marshal0.tw = (*this).MatProjection.Tw;
    _this0.matProjection = _this0_marshal0;
    _this0.fieldOfView = (*this).FieldOfView;
    _this0.nearPlane = (*this).NearPlane;
    _this0.farPlane = (*this).FarPlane;
    _this0.aspectRatio = (*this).AspectRatio;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).OrthoSize.X;
    _this0_marshal1.y = (*this).OrthoSize.Y;
    _this0_marshal1.z = (*this).OrthoSize.Z;
    _this0.orthoSize = _this0_marshal1;
    _this0.updateProjection();
    __Projection = (Flood::FrustumProjection)_this0.projection;
    MatProjection = Flood::Matrix4x4((::Matrix4x4*)&_this0.matProjection);
    __FieldOfView = _this0.fieldOfView;
    __NearPlane = _this0.nearPlane;
    __FarPlane = _this0.farPlane;
    __AspectRatio = _this0.aspectRatio;
    OrthoSize = Flood::Vector3((::Vector3*)&_this0.orthoSize);
}

void Flood::Frustum::UpdatePlanes(Flood::Matrix4x3 matView)
{
    auto _this0 = ::Frustum();
    _this0.projection = (::FrustumProjection)(*this).Projection;
    auto _this0_marshal0 = ::Matrix4x4();
    _this0_marshal0.m11 = (*this).MatProjection.M11;
    _this0_marshal0.m12 = (*this).MatProjection.M12;
    _this0_marshal0.m13 = (*this).MatProjection.M13;
    _this0_marshal0.m14 = (*this).MatProjection.M14;
    _this0_marshal0.m21 = (*this).MatProjection.M21;
    _this0_marshal0.m22 = (*this).MatProjection.M22;
    _this0_marshal0.m23 = (*this).MatProjection.M23;
    _this0_marshal0.m24 = (*this).MatProjection.M24;
    _this0_marshal0.m31 = (*this).MatProjection.M31;
    _this0_marshal0.m32 = (*this).MatProjection.M32;
    _this0_marshal0.m33 = (*this).MatProjection.M33;
    _this0_marshal0.m34 = (*this).MatProjection.M34;
    _this0_marshal0.tx = (*this).MatProjection.Tx;
    _this0_marshal0.ty = (*this).MatProjection.Ty;
    _this0_marshal0.tz = (*this).MatProjection.Tz;
    _this0_marshal0.tw = (*this).MatProjection.Tw;
    _this0.matProjection = _this0_marshal0;
    _this0.fieldOfView = (*this).FieldOfView;
    _this0.nearPlane = (*this).NearPlane;
    _this0.farPlane = (*this).FarPlane;
    _this0.aspectRatio = (*this).AspectRatio;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).OrthoSize.X;
    _this0_marshal1.y = (*this).OrthoSize.Y;
    _this0_marshal1.z = (*this).OrthoSize.Z;
    _this0.orthoSize = _this0_marshal1;
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = matView.M11;
    _marshal0.m12 = matView.M12;
    _marshal0.m13 = matView.M13;
    _marshal0.m21 = matView.M21;
    _marshal0.m22 = matView.M22;
    _marshal0.m23 = matView.M23;
    _marshal0.m31 = matView.M31;
    _marshal0.m32 = matView.M32;
    _marshal0.m33 = matView.M33;
    _marshal0.tx = matView.Tx;
    _marshal0.ty = matView.Ty;
    _marshal0.tz = matView.Tz;
    auto arg0 = _marshal0;
    _this0.updatePlanes(arg0);
    __Projection = (Flood::FrustumProjection)_this0.projection;
    MatProjection = Flood::Matrix4x4((::Matrix4x4*)&_this0.matProjection);
    __FieldOfView = _this0.fieldOfView;
    __NearPlane = _this0.nearPlane;
    __FarPlane = _this0.farPlane;
    __AspectRatio = _this0.aspectRatio;
    OrthoSize = Flood::Vector3((::Vector3*)&_this0.orthoSize);
}

void Flood::Frustum::UpdateCorners(Flood::Matrix4x3 matView)
{
    auto _this0 = ::Frustum();
    _this0.projection = (::FrustumProjection)(*this).Projection;
    auto _this0_marshal0 = ::Matrix4x4();
    _this0_marshal0.m11 = (*this).MatProjection.M11;
    _this0_marshal0.m12 = (*this).MatProjection.M12;
    _this0_marshal0.m13 = (*this).MatProjection.M13;
    _this0_marshal0.m14 = (*this).MatProjection.M14;
    _this0_marshal0.m21 = (*this).MatProjection.M21;
    _this0_marshal0.m22 = (*this).MatProjection.M22;
    _this0_marshal0.m23 = (*this).MatProjection.M23;
    _this0_marshal0.m24 = (*this).MatProjection.M24;
    _this0_marshal0.m31 = (*this).MatProjection.M31;
    _this0_marshal0.m32 = (*this).MatProjection.M32;
    _this0_marshal0.m33 = (*this).MatProjection.M33;
    _this0_marshal0.m34 = (*this).MatProjection.M34;
    _this0_marshal0.tx = (*this).MatProjection.Tx;
    _this0_marshal0.ty = (*this).MatProjection.Ty;
    _this0_marshal0.tz = (*this).MatProjection.Tz;
    _this0_marshal0.tw = (*this).MatProjection.Tw;
    _this0.matProjection = _this0_marshal0;
    _this0.fieldOfView = (*this).FieldOfView;
    _this0.nearPlane = (*this).NearPlane;
    _this0.farPlane = (*this).FarPlane;
    _this0.aspectRatio = (*this).AspectRatio;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).OrthoSize.X;
    _this0_marshal1.y = (*this).OrthoSize.Y;
    _this0_marshal1.z = (*this).OrthoSize.Z;
    _this0.orthoSize = _this0_marshal1;
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = matView.M11;
    _marshal0.m12 = matView.M12;
    _marshal0.m13 = matView.M13;
    _marshal0.m21 = matView.M21;
    _marshal0.m22 = matView.M22;
    _marshal0.m23 = matView.M23;
    _marshal0.m31 = matView.M31;
    _marshal0.m32 = matView.M32;
    _marshal0.m33 = matView.M33;
    _marshal0.tx = matView.Tx;
    _marshal0.ty = matView.Ty;
    _marshal0.tz = matView.Tz;
    auto arg0 = _marshal0;
    _this0.updateCorners(arg0);
    __Projection = (Flood::FrustumProjection)_this0.projection;
    MatProjection = Flood::Matrix4x4((::Matrix4x4*)&_this0.matProjection);
    __FieldOfView = _this0.fieldOfView;
    __NearPlane = _this0.nearPlane;
    __FarPlane = _this0.farPlane;
    __AspectRatio = _this0.aspectRatio;
    OrthoSize = Flood::Vector3((::Vector3*)&_this0.orthoSize);
}

bool Flood::Frustum::Intersects(Flood::BoundingBox box)
{
    auto _this0 = ::Frustum();
    _this0.projection = (::FrustumProjection)(*this).Projection;
    auto _this0_marshal0 = ::Matrix4x4();
    _this0_marshal0.m11 = (*this).MatProjection.M11;
    _this0_marshal0.m12 = (*this).MatProjection.M12;
    _this0_marshal0.m13 = (*this).MatProjection.M13;
    _this0_marshal0.m14 = (*this).MatProjection.M14;
    _this0_marshal0.m21 = (*this).MatProjection.M21;
    _this0_marshal0.m22 = (*this).MatProjection.M22;
    _this0_marshal0.m23 = (*this).MatProjection.M23;
    _this0_marshal0.m24 = (*this).MatProjection.M24;
    _this0_marshal0.m31 = (*this).MatProjection.M31;
    _this0_marshal0.m32 = (*this).MatProjection.M32;
    _this0_marshal0.m33 = (*this).MatProjection.M33;
    _this0_marshal0.m34 = (*this).MatProjection.M34;
    _this0_marshal0.tx = (*this).MatProjection.Tx;
    _this0_marshal0.ty = (*this).MatProjection.Ty;
    _this0_marshal0.tz = (*this).MatProjection.Tz;
    _this0_marshal0.tw = (*this).MatProjection.Tw;
    _this0.matProjection = _this0_marshal0;
    _this0.fieldOfView = (*this).FieldOfView;
    _this0.nearPlane = (*this).NearPlane;
    _this0.farPlane = (*this).FarPlane;
    _this0.aspectRatio = (*this).AspectRatio;
    auto _this0_marshal1 = ::Vector3();
    _this0_marshal1.x = (*this).OrthoSize.X;
    _this0_marshal1.y = (*this).OrthoSize.Y;
    _this0_marshal1.z = (*this).OrthoSize.Z;
    _this0.orthoSize = _this0_marshal1;
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
    auto __ret = _this0.intersects(arg0);
    __Projection = (Flood::FrustumProjection)_this0.projection;
    MatProjection = Flood::Matrix4x4((::Matrix4x4*)&_this0.matProjection);
    __FieldOfView = _this0.fieldOfView;
    __NearPlane = _this0.nearPlane;
    __FarPlane = _this0.farPlane;
    __AspectRatio = _this0.aspectRatio;
    OrthoSize = Flood::Vector3((::Vector3*)&_this0.orthoSize);
    return __ret;
}

Flood::FrustumProjection Flood::Frustum::Projection::get()
{
    return __Projection;
}

void Flood::Frustum::Projection::set(Flood::FrustumProjection value)
{
    __Projection = value;
}

float Flood::Frustum::FieldOfView::get()
{
    return __FieldOfView;
}

void Flood::Frustum::FieldOfView::set(float value)
{
    __FieldOfView = value;
}

float Flood::Frustum::NearPlane::get()
{
    return __NearPlane;
}

void Flood::Frustum::NearPlane::set(float value)
{
    __NearPlane = value;
}

float Flood::Frustum::FarPlane::get()
{
    return __FarPlane;
}

void Flood::Frustum::FarPlane::set(float value)
{
    __FarPlane = value;
}

float Flood::Frustum::AspectRatio::get()
{
    return __AspectRatio;
}

void Flood::Frustum::AspectRatio::set(float value)
{
    __AspectRatio = value;
}

