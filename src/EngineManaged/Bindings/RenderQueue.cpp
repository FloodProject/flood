/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderQueue.h"
#include "Material.h"
#include "RenderBatch.h"
#include "Texture.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RenderState::RenderState(::RenderState* native)
{
    Renderable = gcnew Flood::RenderBatch((::RenderBatch*)native->renderable);
    Material = gcnew Flood::Material((::Material*)native->material);
    ModelMatrix = Flood::Matrix4x3((::Matrix4x3*)&native->modelMatrix);
    Priority = native->priority;
}

Flood::RenderState::RenderState(System::IntPtr native)
{
    auto __native = (::RenderState*)native.ToPointer();
    Renderable = gcnew Flood::RenderBatch((::RenderBatch*)__native->renderable);
    Material = gcnew Flood::Material((::Material*)__native->material);
    ModelMatrix = Flood::Matrix4x3((::Matrix4x3*)&__native->modelMatrix);
    Priority = __native->priority;
}

Flood::RenderState::RenderState(Flood::RenderBatch^ renderable)
{
    auto _native = ::RenderState((::RenderBatch*)renderable->NativePtr);
    this->Renderable = gcnew Flood::RenderBatch((::RenderBatch*)_native.renderable);
    this->Material = gcnew Flood::Material((::Material*)_native.material);
    this->ModelMatrix = Flood::Matrix4x3((::Matrix4x3*)&_native.modelMatrix);
    this->Priority = _native.priority;
}

Flood::LightState::LightState(::LightState* native)
{
    Depth = gcnew Flood::Texture((::Texture*)native->depth);
    Projection = Flood::Matrix4x4((::Matrix4x4*)&native->projection);
}

Flood::LightState::LightState(System::IntPtr native)
{
    auto __native = (::LightState*)native.ToPointer();
    Depth = gcnew Flood::Texture((::Texture*)__native->depth);
    Projection = Flood::Matrix4x4((::Matrix4x4*)&__native->projection);
}

Flood::RenderBlock::RenderBlock(::RenderBlock* native)
{
    NativePtr = native;
}

Flood::RenderBlock::RenderBlock(System::IntPtr native)
{
    auto __native = (::RenderBlock*)native.ToPointer();
    NativePtr = __native;
}

void Flood::RenderBlock::AddState(Flood::RenderState renderState)
{
    auto _marshal0 = ::RenderState();
    if (renderState.Renderable != nullptr)
        _marshal0.renderable = (::RenderBatch*)renderState.Renderable->NativePtr;
    if (renderState.Material != nullptr)
        _marshal0.material = (::Material*)renderState.Material->NativePtr;
    auto _marshal1 = ::Matrix4x3();
    _marshal1.m11 = renderState.ModelMatrix.M11;
    _marshal1.m12 = renderState.ModelMatrix.M12;
    _marshal1.m13 = renderState.ModelMatrix.M13;
    _marshal1.m21 = renderState.ModelMatrix.M21;
    _marshal1.m22 = renderState.ModelMatrix.M22;
    _marshal1.m23 = renderState.ModelMatrix.M23;
    _marshal1.m31 = renderState.ModelMatrix.M31;
    _marshal1.m32 = renderState.ModelMatrix.M32;
    _marshal1.m33 = renderState.ModelMatrix.M33;
    _marshal1.tx = renderState.ModelMatrix.Tx;
    _marshal1.ty = renderState.ModelMatrix.Ty;
    _marshal1.tz = renderState.ModelMatrix.Tz;
    _marshal0.modelMatrix = _marshal1;
    _marshal0.priority = (int32)renderState.Priority;
    auto arg0 = _marshal0;
    ((::RenderBlock*)NativePtr)->addState(arg0);
}

Flood::RenderBlock::RenderBlock()
{
    NativePtr = new ::RenderBlock();
}

bool Flood::RenderBlock::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RenderBlock^>(object);
    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::RenderBlock::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderBlock::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderBlock::Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderBlock*)object.ToPointer();
}

System::Collections::Generic::List<Flood::RenderState>^ Flood::RenderBlock::Renderables::get()
{
    auto _tmpRenderables = gcnew System::Collections::Generic::List<Flood::RenderState>();
    for(auto _element : ((::RenderBlock*)NativePtr)->renderables)
    {
        auto _marshalElement = Flood::RenderState((::RenderState*)&_element);
        _tmpRenderables->Add(_marshalElement);
    }
    return _tmpRenderables;
}

void Flood::RenderBlock::Renderables::set(System::Collections::Generic::List<Flood::RenderState>^ value)
{
    auto _tmpvalue = std::vector<::RenderState>();
    for each(Flood::RenderState _element in value)
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
        _marshal0.priority = (int32)_element.Priority;
        auto _marshalElement = _marshal0;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::RenderBlock*)NativePtr)->renderables = _tmpvalue;
}

System::Collections::Generic::List<Flood::LightState>^ Flood::RenderBlock::Lights::get()
{
    auto _tmpLights = gcnew System::Collections::Generic::List<Flood::LightState>();
    for(auto _element : ((::RenderBlock*)NativePtr)->lights)
    {
        auto _marshalElement = Flood::LightState((::LightState*)&_element);
        _tmpLights->Add(_marshalElement);
    }
    return _tmpLights;
}

void Flood::RenderBlock::Lights::set(System::Collections::Generic::List<Flood::LightState>^ value)
{
    auto _tmpvalue = std::vector<::LightState>();
    for each(Flood::LightState _element in value)
    {
        auto _marshal0 = ::LightState();
        if (_element.Depth != nullptr)
            _marshal0.depth = (::Texture*)_element.Depth->NativePtr;
        auto _marshal1 = ::Matrix4x4();
        _marshal1.m11 = _element.Projection.M11;
        _marshal1.m12 = _element.Projection.M12;
        _marshal1.m13 = _element.Projection.M13;
        _marshal1.m14 = _element.Projection.M14;
        _marshal1.m21 = _element.Projection.M21;
        _marshal1.m22 = _element.Projection.M22;
        _marshal1.m23 = _element.Projection.M23;
        _marshal1.m24 = _element.Projection.M24;
        _marshal1.m31 = _element.Projection.M31;
        _marshal1.m32 = _element.Projection.M32;
        _marshal1.m33 = _element.Projection.M33;
        _marshal1.m34 = _element.Projection.M34;
        _marshal1.tx = _element.Projection.Tx;
        _marshal1.ty = _element.Projection.Ty;
        _marshal1.tz = _element.Projection.Tz;
        _marshal1.tw = _element.Projection.Tw;
        _marshal0.projection = _marshal1;
        auto _marshalElement = _marshal0;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::RenderBlock*)NativePtr)->lights = _tmpvalue;
}

