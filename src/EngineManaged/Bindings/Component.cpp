/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Component.h"
#include "Camera.h"
#include "DebugGeometry.h"
#include "Entity.h"
#include "RenderBatch.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Component::Component(::Component* native)
{
    NativePtr = native;
}

Flood::Component::Component(System::IntPtr native)
{
    auto __native = (::Component*)native.ToPointer();
    NativePtr = __native;
}

Flood::Component::Component()
{
}

void Flood::Component::Update(float delta)
{
    ((::Component*)NativePtr)->update(delta);
}

void Flood::Component::OnPreRender(Flood::Camera^ camera)
{
    auto &arg0 = *(::Camera*)camera->NativePtr;
    ((::Component*)NativePtr)->onPreRender(arg0);
}

bool Flood::Component::IsDebugRenderableVisible()
{
    auto __ret = ((::Component*)NativePtr)->isDebugRenderableVisible();
    return __ret;
}

void Flood::Component::OnDebugDraw(Flood::DebugDrawer^ _0, Flood::DebugDrawFlags _1)
{
    auto &arg0 = *(::DebugDrawer*)_0->NativePtr;
    auto arg1 = (::DebugDrawFlags)_1;
    ((::Component*)NativePtr)->onDebugDraw(arg0, arg1);
}

bool Flood::Component::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Component^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Component::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Component::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Component::Instance::set(System::IntPtr object)
{
    NativePtr = (::Component*)object.ToPointer();
}

Flood::Entity^ Flood::Component::Entity::get()
{
    auto __ret = ((::Component*)NativePtr)->getEntity();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Entity((::Entity*)__ret);
}

void Flood::Component::Entity::set(Flood::Entity^ value)
{
    auto entity = value;
    auto arg0 = (::Entity*)entity->NativePtr;
    ((::Component*)NativePtr)->setEntity(arg0);
}

void Flood::Component::DebugRenderableVisible::set(bool value)
{
    auto visible = value;
    ((::Component*)NativePtr)->setDebugRenderableVisible(visible);
}

bool Flood::Component::DebugInheritsTransform::get()
{
    auto __ret = ((::Component*)NativePtr)->getDebugInheritsTransform();
    return __ret;
}

void Flood::Component::DebugInheritsTransform::set(bool value)
{
    auto v = value;
    ((::Component*)NativePtr)->setDebugInheritsTransform(v);
}

