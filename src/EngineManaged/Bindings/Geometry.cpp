/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Geometry.h"
#include "RenderBatch.h"
#include "RenderQueue.h"
#include "Transform.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Geometry::Geometry(::Geometry* native)
    : Flood::Component((::Component*)native)
{
}

Flood::Geometry::Geometry(System::IntPtr native)
    : Flood::Component(native)
{
    auto __native = (::Geometry*)native.ToPointer();
}

Flood::Geometry::Geometry()
    : Flood::Component((::Component*)nullptr)
{
    NativePtr = new ::Geometry();
}

void Flood::Geometry::AddRenderable(Flood::RenderBatch^ renderable)
{
    auto arg0 = (::RenderBatch*)renderable->NativePtr;
    ((::Geometry*)NativePtr)->addRenderable(arg0);
}

void Flood::Geometry::AppendRenderables(System::Collections::Generic::List<Flood::RenderState>^ queue, Flood::Transform^ transform)
{
    auto _tmpqueue = std::vector<::RenderState>();
    for each(Flood::RenderState _element in queue)
    {
        auto _marshal0 = ::RenderState();
        if (_element.Renderable != nullptr)
            _marshal0.renderable = (::RenderBatch*)_element.Renderable->NativePtr;
        if (_element.Material != nullptr)
            _marshal0.material = (::Material*)_element.Material->NativePtr;
        auto _marshal1 = ::Matrix4x3();
        _marshal1.m11 = _element.ModelMatrix.M11;
        _marshal1.m12 = _element.ModelMatrix.M12;
        _marshal1.m13 = _element.ModelMatrix.M13;
        _marshal1.m21 = _element.ModelMatrix.M21;
        _marshal1.m22 = _element.ModelMatrix.M22;
        _marshal1.m23 = _element.ModelMatrix.M23;
        _marshal1.m31 = _element.ModelMatrix.M31;
        _marshal1.m32 = _element.ModelMatrix.M32;
        _marshal1.m33 = _element.ModelMatrix.M33;
        _marshal1.tx = _element.ModelMatrix.Tx;
        _marshal1.ty = _element.ModelMatrix.Ty;
        _marshal1.tz = _element.ModelMatrix.Tz;
        _marshal0.modelMatrix = _marshal1;
        _marshal0.priority = (::int32)(::int32_t)_element.Priority;
        auto _marshalElement = _marshal0;
        _tmpqueue.push_back(_marshalElement);
    }
    auto arg0 = _tmpqueue;
    auto arg1 = (::Transform*)transform->NativePtr;
    ((::Geometry*)NativePtr)->appendRenderables(arg0, arg1);
}

void Flood::Geometry::UpdateBounds()
{
    ((::Geometry*)NativePtr)->updateBounds();
}

void Flood::Geometry::Update(float delta)
{
    ((::Geometry*)NativePtr)->update(delta);
}

void Flood::Geometry::RebuildBoundingBox()
{
    ((::Geometry*)NativePtr)->rebuildBoundingBox();
}

bool Flood::Geometry::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Geometry^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Geometry::GetHashCode()
{
    return (int)NativePtr;
}

System::Collections::Generic::List<Flood::RenderBatch^>^ Flood::Geometry::Renderables::get()
{
    auto __ret = ((::Geometry*)NativePtr)->getRenderables();
    auto _tmp__ret = gcnew System::Collections::Generic::List<Flood::RenderBatch^>();
    for(auto _element : __ret)
    {
        auto _marshalElement = gcnew Flood::RenderBatch((::RenderBatch*)_element.get());
        _tmp__ret->Add(_marshalElement);
    }
    return _tmp__ret;
}

Flood::BoundingBox Flood::Geometry::BoundingVolume::get()
{
    auto &__ret = ((::Geometry*)NativePtr)->getBoundingVolume();
    return Flood::BoundingBox((::BoundingBox*)&__ret);
}

Flood::BoundingBox Flood::Geometry::WorldBoundingVolume::get()
{
    auto __ret = ((::Geometry*)NativePtr)->getWorldBoundingVolume();
    return Flood::BoundingBox((::BoundingBox*)&__ret);
}

