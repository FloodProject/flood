/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "DebugGeometry.h"
#include "BoundingBox.h"
#include "Frustum.h"
#include "GeometryBuffer.h"
#include "Ray.h"
#include "RenderBatch.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::DebugDrawer::DebugDrawer(::DebugDrawer* native)
{
    NativePtr = native;
}

Flood::DebugDrawer::DebugDrawer(System::IntPtr native)
{
    auto __native = (::DebugDrawer*)native.ToPointer();
    NativePtr = __native;
}

Flood::DebugDrawer::DebugDrawer()
{
    NativePtr = new ::DebugDrawer();
}

void Flood::DebugDrawer::DrawBox(Flood::BoundingBox box)
{
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
    ((::DebugDrawer*)NativePtr)->drawBox(arg0);
}

void Flood::DebugDrawer::DrawRay(Flood::Ray ray, float length)
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
    ((::DebugDrawer*)NativePtr)->drawRay(arg0, length);
}

void Flood::DebugDrawer::DrawFrustum(Flood::Frustum frustum)
{
    auto _marshal0 = ::Frustum();
    _marshal0.projection = (::FrustumProjection)frustum.Projection;
    auto _marshal1 = ::Matrix4x4();
    _marshal1.m11 = frustum.MatProjection.M11;
    _marshal1.m12 = frustum.MatProjection.M12;
    _marshal1.m13 = frustum.MatProjection.M13;
    _marshal1.m14 = frustum.MatProjection.M14;
    _marshal1.m21 = frustum.MatProjection.M21;
    _marshal1.m22 = frustum.MatProjection.M22;
    _marshal1.m23 = frustum.MatProjection.M23;
    _marshal1.m24 = frustum.MatProjection.M24;
    _marshal1.m31 = frustum.MatProjection.M31;
    _marshal1.m32 = frustum.MatProjection.M32;
    _marshal1.m33 = frustum.MatProjection.M33;
    _marshal1.m34 = frustum.MatProjection.M34;
    _marshal1.tx = frustum.MatProjection.Tx;
    _marshal1.ty = frustum.MatProjection.Ty;
    _marshal1.tz = frustum.MatProjection.Tz;
    _marshal1.tw = frustum.MatProjection.Tw;
    _marshal0.matProjection = _marshal1;
    _marshal0.fieldOfView = frustum.FieldOfView;
    _marshal0.nearPlane = frustum.NearPlane;
    _marshal0.farPlane = frustum.FarPlane;
    _marshal0.aspectRatio = frustum.AspectRatio;
    auto _marshal2 = ::Vector3();
    _marshal2.x = frustum.OrthoSize.X;
    _marshal2.y = frustum.OrthoSize.Y;
    _marshal2.z = frustum.OrthoSize.Z;
    _marshal0.orthoSize = _marshal2;
    auto arg0 = _marshal0;
    ((::DebugDrawer*)NativePtr)->drawFrustum(arg0);
}

void Flood::DebugDrawer::DrawIcon(Flood::Vector3 pos)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = pos.X;
    _marshal0.y = pos.Y;
    _marshal0.z = pos.Z;
    auto arg0 = _marshal0;
    ((::DebugDrawer*)NativePtr)->drawIcon(arg0);
}

void Flood::DebugDrawer::Reset()
{
    ((::DebugDrawer*)NativePtr)->reset();
}

bool Flood::DebugDrawer::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<DebugDrawer^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::DebugDrawer::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::DebugDrawer::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::DebugDrawer::__Instance::set(System::IntPtr object)
{
    NativePtr = (::DebugDrawer*)object.ToPointer();
}

void Flood::DebugDrawer::Color::set(Flood::Color color)
{
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)color.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)color.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)color.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)color.A;
    auto arg0 = _marshal0;
    ((::DebugDrawer*)NativePtr)->setColor(arg0);
}

Flood::Color Flood::DebugDrawer::CurrentColor::get()
{
    return Flood::Color((::Color*)&((::DebugDrawer*)NativePtr)->currentColor);
}

void Flood::DebugDrawer::CurrentColor::set(Flood::Color value)
{
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)value.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)value.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)value.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)value.A;
    ((::DebugDrawer*)NativePtr)->currentColor = _marshal0;
}

Flood::RenderBatch^ Flood::DebugDrawer::Lines::get()
{
    return (((::DebugDrawer*)NativePtr)->lines.get() == nullptr) ? nullptr : gcnew Flood::RenderBatch((::RenderBatch*)((::DebugDrawer*)NativePtr)->lines.get());
}

void Flood::DebugDrawer::Lines::set(Flood::RenderBatch^ value)
{
    ((::DebugDrawer*)NativePtr)->lines = (::RenderBatch*)value->NativePtr;
}

Flood::RenderBatch^ Flood::DebugDrawer::Triangles::get()
{
    return (((::DebugDrawer*)NativePtr)->triangles.get() == nullptr) ? nullptr : gcnew Flood::RenderBatch((::RenderBatch*)((::DebugDrawer*)NativePtr)->triangles.get());
}

void Flood::DebugDrawer::Triangles::set(Flood::RenderBatch^ value)
{
    ((::DebugDrawer*)NativePtr)->triangles = (::RenderBatch*)value->NativePtr;
}

Flood::RenderBatch^ Flood::DebugDrawer::Quads::get()
{
    return (((::DebugDrawer*)NativePtr)->quads.get() == nullptr) ? nullptr : gcnew Flood::RenderBatch((::RenderBatch*)((::DebugDrawer*)NativePtr)->quads.get());
}

void Flood::DebugDrawer::Quads::set(Flood::RenderBatch^ value)
{
    ((::DebugDrawer*)NativePtr)->quads = (::RenderBatch*)value->NativePtr;
}

System::Collections::Generic::List<Flood::RenderBatch^>^ Flood::DebugDrawer::Renderables::get()
{
    auto _tmp__Renderables = gcnew System::Collections::Generic::List<Flood::RenderBatch^>();
    for(auto _element : ((::DebugDrawer*)NativePtr)->renderables)
    {
        auto _marshalElement = (_element == nullptr) ? nullptr : gcnew Flood::RenderBatch((::RenderBatch*)_element);
        _tmp__Renderables->Add(_marshalElement);
    }
    return _tmp__Renderables;
}

void Flood::DebugDrawer::Renderables::set(System::Collections::Generic::List<Flood::RenderBatch^>^ value)
{
    auto _tmpvalue = Vector<::RenderBatch*>();
    for each(Flood::RenderBatch^ _element in value)
    {
        auto _marshalElement = (::RenderBatch*)_element->NativePtr;
        _tmpvalue.Push(_marshalElement);
    }
    ((::DebugDrawer*)NativePtr)->renderables = _tmpvalue;
}

