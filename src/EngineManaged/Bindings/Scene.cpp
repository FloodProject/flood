/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Scene.h"
#include "Entity.h"
#include "Frustum.h"
#include "Geometry.h"
#include "Group.h"
#include "Ray.h"
#include "RenderBatch.h"
#include "Resource.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RayQueryResult::RayQueryResult(::RayQueryResult* native)
{
    NativePtr = native;
}

Flood::RayQueryResult::RayQueryResult(System::IntPtr native)
{
    auto __native = (::RayQueryResult*)native.ToPointer();
    NativePtr = __native;
}

Flood::RayQueryResult::RayQueryResult()
{
    NativePtr = new ::RayQueryResult();
}

bool Flood::RayQueryResult::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RayQueryResult^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::RayQueryResult::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RayQueryResult::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RayQueryResult::Instance::set(System::IntPtr object)
{
    NativePtr = (::RayQueryResult*)object.ToPointer();
}

Flood::Entity^ Flood::RayQueryResult::Entity::get()
{
    return gcnew Flood::Entity((::Entity*)((::RayQueryResult*)NativePtr)->entity);
}

void Flood::RayQueryResult::Entity::set(Flood::Entity^ value)
{
    ((::RayQueryResult*)NativePtr)->entity = (::Entity*)value->NativePtr;
}

float Flood::RayQueryResult::Distance::get()
{
    return ((::RayQueryResult*)NativePtr)->distance;
}

void Flood::RayQueryResult::Distance::set(float value)
{
    ((::RayQueryResult*)NativePtr)->distance = value;
}

Flood::RayTriangleQueryResult::RayTriangleQueryResult(::RayTriangleQueryResult* native)
    : Flood::RayQueryResult((::RayQueryResult*)native)
{
}

Flood::RayTriangleQueryResult::RayTriangleQueryResult(System::IntPtr native)
    : Flood::RayQueryResult(native)
{
    auto __native = (::RayTriangleQueryResult*)native.ToPointer();
}

Flood::RayTriangleQueryResult::RayTriangleQueryResult()
    : Flood::RayQueryResult((::RayQueryResult*)nullptr)
{
    NativePtr = new ::RayTriangleQueryResult();
}

bool Flood::RayTriangleQueryResult::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RayTriangleQueryResult^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::RayTriangleQueryResult::GetHashCode()
{
    return (int)NativePtr;
}

Flood::Geometry^ Flood::RayTriangleQueryResult::Geometry::get()
{
    return gcnew Flood::Geometry((::Geometry*)((::RayTriangleQueryResult*)NativePtr)->geometry);
}

void Flood::RayTriangleQueryResult::Geometry::set(Flood::Geometry^ value)
{
    ((::RayTriangleQueryResult*)NativePtr)->geometry = (::Geometry*)value->NativePtr;
}

Flood::RenderBatch^ Flood::RayTriangleQueryResult::Renderable::get()
{
    return gcnew Flood::RenderBatch((::RenderBatch*)((::RayTriangleQueryResult*)NativePtr)->renderable);
}

void Flood::RayTriangleQueryResult::Renderable::set(Flood::RenderBatch^ value)
{
    ((::RayTriangleQueryResult*)NativePtr)->renderable = (::RenderBatch*)value->NativePtr;
}

Flood::Vector3 Flood::RayTriangleQueryResult::IntersectionLocal::get()
{
    return Flood::Vector3((::Vector3*)&((::RayTriangleQueryResult*)NativePtr)->intersectionLocal);
}

void Flood::RayTriangleQueryResult::IntersectionLocal::set(Flood::Vector3 value)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = value.X;
    _marshal0.y = value.Y;
    _marshal0.z = value.Z;
    ((::RayTriangleQueryResult*)NativePtr)->intersectionLocal = _marshal0;
}

Flood::Vector3 Flood::RayTriangleQueryResult::IntersectionWorld::get()
{
    return Flood::Vector3((::Vector3*)&((::RayTriangleQueryResult*)NativePtr)->intersectionWorld);
}

void Flood::RayTriangleQueryResult::IntersectionWorld::set(Flood::Vector3 value)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = value.X;
    _marshal0.y = value.Y;
    _marshal0.z = value.Z;
    ((::RayTriangleQueryResult*)NativePtr)->intersectionWorld = _marshal0;
}

Flood::Vector3 Flood::RayTriangleQueryResult::IntersectionUV::get()
{
    return Flood::Vector3((::Vector3*)&((::RayTriangleQueryResult*)NativePtr)->intersectionUV);
}

void Flood::RayTriangleQueryResult::IntersectionUV::set(Flood::Vector3 value)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = value.X;
    _marshal0.y = value.Y;
    _marshal0.z = value.Z;
    ((::RayTriangleQueryResult*)NativePtr)->intersectionUV = _marshal0;
}

Flood::Scene::Scene(::Scene* native)
    : Flood::Resource((::Resource*)native)
{
}

Flood::Scene::Scene(System::IntPtr native)
    : Flood::Resource(native)
{
    auto __native = (::Scene*)native.ToPointer();
}

Flood::Scene::Scene()
    : Flood::Resource((::Resource*)nullptr)
{
    NativePtr = new ::Scene();
}

void Flood::Scene::Update(float delta)
{
    ((::Scene*)NativePtr)->update(delta);
}

bool Flood::Scene::DoRayBoxQuery(Flood::Ray ray, Flood::RayQueryResult^ res)
{
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
    auto &arg1 = *(::RayQueryResult*)res->NativePtr;
    auto __ret = ((::Scene*)NativePtr)->doRayBoxQuery(arg0, arg1);
    return __ret;
}

bool Flood::Scene::DoRayBoxQuery(Flood::Ray ray, System::Collections::Generic::List<Flood::RayQueryResult^>^ list, bool all)
{
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
    auto _tmplist = Vector<::RayQueryResult>();
    for each(Flood::RayQueryResult^ _element in list)
    {
        auto _marshalElement = *(::RayQueryResult*)_element->NativePtr;
        _tmplist.Push(_marshalElement);
    }
    auto arg1 = _tmplist;
    auto __ret = ((::Scene*)NativePtr)->doRayBoxQuery(arg0, arg1, all);
    return __ret;
}

bool Flood::Scene::DoRayVolumeQuery(Flood::Frustum volume, System::Collections::Generic::List<Flood::RayQueryResult^>^ list, bool all)
{
    auto _marshal0 = ::Frustum();
    _marshal0.projection = (::FrustumProjection)volume.Projection;
    auto _marshal1 = ::Matrix4x4();
    _marshal1.m11 = volume.MatProjection.M11;
    _marshal1.m12 = volume.MatProjection.M12;
    _marshal1.m13 = volume.MatProjection.M13;
    _marshal1.m14 = volume.MatProjection.M14;
    _marshal1.m21 = volume.MatProjection.M21;
    _marshal1.m22 = volume.MatProjection.M22;
    _marshal1.m23 = volume.MatProjection.M23;
    _marshal1.m24 = volume.MatProjection.M24;
    _marshal1.m31 = volume.MatProjection.M31;
    _marshal1.m32 = volume.MatProjection.M32;
    _marshal1.m33 = volume.MatProjection.M33;
    _marshal1.m34 = volume.MatProjection.M34;
    _marshal1.tx = volume.MatProjection.Tx;
    _marshal1.ty = volume.MatProjection.Ty;
    _marshal1.tz = volume.MatProjection.Tz;
    _marshal1.tw = volume.MatProjection.Tw;
    _marshal0.matProjection = _marshal1;
    _marshal0.fieldOfView = volume.FieldOfView;
    _marshal0.nearPlane = volume.NearPlane;
    _marshal0.farPlane = volume.FarPlane;
    _marshal0.aspectRatio = volume.AspectRatio;
    auto _marshal2 = ::Vector3();
    _marshal2.x = volume.OrthoSize.X;
    _marshal2.y = volume.OrthoSize.Y;
    _marshal2.z = volume.OrthoSize.Z;
    _marshal0.orthoSize = _marshal2;
    auto arg0 = _marshal0;
    auto _tmplist = Vector<::RayQueryResult>();
    for each(Flood::RayQueryResult^ _element in list)
    {
        auto _marshalElement = *(::RayQueryResult*)_element->NativePtr;
        _tmplist.Push(_marshalElement);
    }
    auto arg1 = _tmplist;
    auto __ret = ((::Scene*)NativePtr)->doRayVolumeQuery(arg0, arg1, all);
    return __ret;
}

bool Flood::Scene::DoRayTriangleQuery(Flood::Ray ray, Flood::RayTriangleQueryResult^ res)
{
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
    auto &arg1 = *(::RayTriangleQueryResult*)res->NativePtr;
    auto __ret = ((::Scene*)NativePtr)->doRayTriangleQuery(arg0, arg1);
    return __ret;
}

bool Flood::Scene::DoRayTriangleQuery(Flood::Ray ray, Flood::RayTriangleQueryResult^ res, Flood::Entity^ _0)
{
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
    auto &arg1 = *(::RayTriangleQueryResult*)res->NativePtr;
    auto arg2 = (::Entity*)_0->NativePtr;
    auto __ret = ((::Scene*)NativePtr)->doRayTriangleQuery(arg0, arg1, arg2);
    return __ret;
}

bool Flood::Scene::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Scene^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Scene::GetHashCode()
{
    return (int)NativePtr;
}

Flood::ResourceGroup Flood::Scene::ResourceGroup::get()
{
    auto __ret = ((::Scene*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)__ret;
}

Flood::Group^ Flood::Scene::Entities::get()
{
    return gcnew Flood::Group((::Group*)&((::Scene*)NativePtr)->entities);
}

