/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Camera.h"
#include "DebugGeometry.h"
#include "Entity.h"
#include "Ray.h"
#include "RenderDevice.h"
#include "RenderQueue.h"
#include "RenderView.h"
#include "Transform.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Camera::Camera(::Camera* native)
    : Flood::Component((::Component*)native)
{
}

Flood::Camera::Camera(System::IntPtr native)
    : Flood::Component(native)
{
    auto __native = (::Camera*)native.ToPointer();
}

Flood::Camera::Camera()
    : Flood::Component((::Component*)nullptr)
{
    NativePtr = new ::Camera();
}

void Flood::Camera::Render(Flood::RenderBlock^ block, bool clearView)
{
    auto &arg0 = *(::RenderBlock*)block->NativePtr;
    ((::Camera*)NativePtr)->render(arg0, clearView);
}

void Flood::Camera::Cull(Flood::RenderBlock^ queue, Flood::Entity^ entity)
{
    auto &arg0 = *(::RenderBlock*)queue->NativePtr;
    auto arg1 = (::Entity*)entity->NativePtr;
    ((::Camera*)NativePtr)->cull(arg0, arg1);
}

Flood::Ray Flood::Camera::GetRay(float x, float y, Flood::Vector3 outFar)
{
    auto _marshal2 = ::Vector3();
    _marshal2.x = outFar.X;
    _marshal2.y = outFar.Y;
    _marshal2.z = outFar.Z;
    auto arg2 = _marshal2;
    auto __ret = ((::Camera*)NativePtr)->getRay(x, y, &arg2);
    return Flood::Ray((::Ray*)&__ret);
}

Flood::Frustum Flood::Camera::GetVolume(float screenLeft, float screenRight, float screenTop, float screenBottom)
{
    auto __ret = ((::Camera*)NativePtr)->getVolume(screenLeft, screenRight, screenTop, screenBottom);
    return Flood::Frustum((::Frustum*)&__ret);
}

void Flood::Camera::Update(float delta)
{
    ((::Camera*)NativePtr)->update(delta);
}

void Flood::Camera::UpdateFrustum()
{
    ((::Camera*)NativePtr)->updateFrustum();
}

bool Flood::Camera::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Camera^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Camera::GetHashCode()
{
    return (int)NativePtr;
}

Flood::Vector3 Flood::Camera::LookAtVector::get()
{
    auto &__ret = ((::Camera*)NativePtr)->getLookAtVector();
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Matrix4x3 Flood::Camera::ViewMatrix::get()
{
    auto &__ret = ((::Camera*)NativePtr)->getViewMatrix();
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::RenderView^ Flood::Camera::View::get()
{
    auto __ret = ((::Camera*)NativePtr)->getView();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderView((::RenderView*)__ret);
}

void Flood::Camera::View::set(Flood::RenderView^ value)
{
    auto view = value;
    auto arg0 = (::RenderView*)view->NativePtr;
    ((::Camera*)NativePtr)->setView(arg0);
}

Flood::Frustum Flood::Camera::Frustum::get()
{
    auto &__ret = ((::Camera*)NativePtr)->getFrustum();
    return Flood::Frustum((::Frustum*)&__ret);
}

Flood::Frustum Flood::Camera::Frustum1::get()
{
    auto &__ret = ((::Camera*)NativePtr)->getFrustum();
    return Flood::Frustum((::Frustum*)&__ret);
}

Flood::DebugDrawer^ Flood::Camera::Drawer::get()
{
    return gcnew Flood::DebugDrawer((::DebugDrawer*)&((::Camera*)NativePtr)->drawer);
}

void Flood::Camera::Drawer::set(Flood::DebugDrawer^ value)
{
    ((::Camera*)NativePtr)->drawer = *(::DebugDrawer*)value->NativePtr;
}

