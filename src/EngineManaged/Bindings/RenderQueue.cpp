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
#include "Transform.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RenderState::RenderState(::RenderState* native)
{
    __Renderable = (native->renderable == nullptr) ? nullptr : gcnew Flood::RenderBatch((::RenderBatch*)native->renderable);
    __Material = (native->material == nullptr) ? nullptr : gcnew Flood::Material((::Material*)native->material);
    ModelMatrix = Flood::Matrix4x3((::Matrix4x3*)&native->modelMatrix);
    __Priority = native->priority;
}

Flood::RenderState::RenderState(System::IntPtr native)
{
    auto __native = (::RenderState*)native.ToPointer();
    __Renderable = (__native->renderable == nullptr) ? nullptr : gcnew Flood::RenderBatch((::RenderBatch*)__native->renderable);
    __Material = (__native->material == nullptr) ? nullptr : gcnew Flood::Material((::Material*)__native->material);
    ModelMatrix = Flood::Matrix4x3((::Matrix4x3*)&__native->modelMatrix);
    __Priority = __native->priority;
}

Flood::RenderState::RenderState(Flood::RenderBatch^ renderable)
{
    ::RenderState _native((::RenderBatch*)renderable->NativePtr);
    this->Renderable = (_native.renderable == nullptr) ? nullptr : gcnew Flood::RenderBatch((::RenderBatch*)_native.renderable);
    this->Material = (_native.material == nullptr) ? nullptr : gcnew Flood::Material((::Material*)_native.material);
    this->ModelMatrix = Flood::Matrix4x3((::Matrix4x3*)&_native.modelMatrix);
    this->Priority = _native.priority;
}

Flood::RenderBatch^ Flood::RenderState::Renderable::get()
{
    return __Renderable;
}

void Flood::RenderState::Renderable::set(Flood::RenderBatch^ value)
{
    __Renderable = value;
}

Flood::Material^ Flood::RenderState::Material::get()
{
    return __Material;
}

void Flood::RenderState::Material::set(Flood::Material^ value)
{
    __Material = value;
}

int Flood::RenderState::Priority::get()
{
    return __Priority;
}

void Flood::RenderState::Priority::set(int value)
{
    __Priority = value;
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
    _marshal0.priority = (::int32)(::int32_t)renderState.Priority;
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
    return __Instance == obj->__Instance;
}

int Flood::RenderBlock::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderBlock::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderBlock::__Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderBlock*)object.ToPointer();
}

System::Collections::Generic::List<Flood::RenderState>^ Flood::RenderBlock::Renderables::get()
{
    auto _tmp__Renderables = gcnew System::Collections::Generic::List<Flood::RenderState>();
    for(auto _element : ((::RenderBlock*)NativePtr)->renderables)
    {
        auto _marshalElement = Flood::RenderState((::RenderState*)&_element);
        _tmp__Renderables->Add(_marshalElement);
    }
    return _tmp__Renderables;
}

void Flood::RenderBlock::Renderables::set(System::Collections::Generic::List<Flood::RenderState>^ value)
{
    auto _tmpvalue = Vector<::RenderState>();
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
        _marshal0.priority = (::int32)(::int32_t)_element.Priority;
        auto _marshalElement = _marshal0;
        _tmpvalue.Push(_marshalElement);
    }
    ((::RenderBlock*)NativePtr)->renderables = _tmpvalue;
}

