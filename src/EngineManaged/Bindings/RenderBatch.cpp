/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderBatch.h"
#include "GeometryBuffer.h"
#include "Material.h"
#include "Memory.h"
#include "RenderQueue.h"
#include "RenderView.h"
#include "ResourceHandle.h"
#include "UniformBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::RenderBatchRange::RenderBatchRange(::RenderBatchRange* native)
{
    Start = native->start;
    End = native->end;
}

Flood::RenderBatchRange::RenderBatchRange(System::IntPtr native)
{
    auto __native = (::RenderBatchRange*)native.ToPointer();
    Start = __native->start;
    End = __native->end;
}

Flood::RenderBatch::RenderBatch(::RenderBatch* native)
{
    NativePtr = native;
}

Flood::RenderBatch::RenderBatch(System::IntPtr native)
{
    auto __native = (::RenderBatch*)native.ToPointer();
    NativePtr = __native;
}

Flood::RenderBatch::RenderBatch()
{
    NativePtr = new ::RenderBatch();
}

Flood::RenderLayer Flood::RenderBatch::GetRenderLayer()
{
    auto ret = ((::RenderBatch*)NativePtr)->getRenderLayer();
    return (Flood::RenderLayer)ret;
}

void Flood::RenderBatch::SetRenderLayer(Flood::RenderLayer v)
{
    auto arg0 = (::RenderLayer)v;
    ((::RenderBatch*)NativePtr)->setRenderLayer(arg0);
}

int Flood::RenderBatch::GetRenderPriority()
{
    auto ret = ((::RenderBatch*)NativePtr)->getRenderPriority();
    return ret;
}

void Flood::RenderBatch::SetRenderPriority(int v)
{
    auto arg0 = (int32)v;
    ((::RenderBatch*)NativePtr)->setRenderPriority(arg0);
}

Flood::GeometryBuffer^ Flood::RenderBatch::GetGeometryBuffer()
{
    auto &ret = ((::RenderBatch*)NativePtr)->getGeometryBuffer();
    return gcnew Flood::GeometryBuffer((::GeometryBuffer*)ret.get());
}

void Flood::RenderBatch::SetGeometryBuffer(Flood::GeometryBuffer^ v)
{
    auto arg0 = (::GeometryBuffer*)v->NativePtr;
    ((::RenderBatch*)NativePtr)->setGeometryBuffer(arg0);
}

Flood::UniformBuffer^ Flood::RenderBatch::GetUniformBuffer()
{
    auto &ret = ((::RenderBatch*)NativePtr)->getUniformBuffer();
    return gcnew Flood::UniformBuffer((::UniformBuffer*)ret.get());
}

void Flood::RenderBatch::SetUniformBuffer(Flood::UniformBuffer^ v)
{
    auto arg0 = (::UniformBuffer*)v->NativePtr;
    ((::RenderBatch*)NativePtr)->setUniformBuffer(arg0);
}

Flood::ResourceHandle<Flood::Material^> Flood::RenderBatch::GetMaterial()
{
    auto &ret = ((::RenderBatch*)NativePtr)->getMaterial();
    return Flood::ResourceHandle<Flood::Material^>(ret.id);
}

void Flood::RenderBatch::SetMaterial(Flood::ResourceHandle<Flood::Material^> v)
{
    auto arg0 = (HandleId)v.Id;
    ((::RenderBatch*)NativePtr)->setMaterial(arg0);
}

Flood::PrimitiveRasterMode Flood::RenderBatch::GetPrimitiveRasterMode()
{
    auto ret = ((::RenderBatch*)NativePtr)->getPrimitiveRasterMode();
    return (Flood::PrimitiveRasterMode)ret;
}

void Flood::RenderBatch::SetPrimitiveRasterMode(Flood::PrimitiveRasterMode v)
{
    auto arg0 = (::PrimitiveRasterMode)v;
    ((::RenderBatch*)NativePtr)->setPrimitiveRasterMode(arg0);
}

Flood::PrimitiveType Flood::RenderBatch::GetPrimitiveType()
{
    auto ret = ((::RenderBatch*)NativePtr)->getPrimitiveType();
    return (Flood::PrimitiveType)ret;
}

void Flood::RenderBatch::SetPrimitiveType(Flood::PrimitiveType v)
{
    auto arg0 = (::PrimitiveType)v;
    ((::RenderBatch*)NativePtr)->setPrimitiveType(arg0);
}

Flood::RenderBatchRange Flood::RenderBatch::Range::get()
{
    return Flood::RenderBatchRange((::RenderBatchRange*)&((::RenderBatch*)NativePtr)->range);
}

void Flood::RenderBatch::Range::set(Flood::RenderBatchRange value)
{
    auto _marshal0 = ::RenderBatchRange();
    _marshal0.start = (RenderIndexOffset)(uint16)value.Start;
    _marshal0.end = (RenderIndexOffset)(uint16)value.End;
    ((::RenderBatch*)NativePtr)->range = _marshal0;
}

Flood::RenderLayer Flood::RenderBatch::Stage::get()
{
    return (Flood::RenderLayer)((::RenderBatch*)NativePtr)->stage;
}

void Flood::RenderBatch::Stage::set(Flood::RenderLayer value)
{
    ((::RenderBatch*)NativePtr)->stage = (::RenderLayer)value;
}

int Flood::RenderBatch::Priority::get()
{
    return ((::RenderBatch*)NativePtr)->priority;
}

void Flood::RenderBatch::Priority::set(int value)
{
    ((::RenderBatch*)NativePtr)->priority = (int32)value;
}

Flood::PrimitiveType Flood::RenderBatch::Type::get()
{
    return (Flood::PrimitiveType)((::RenderBatch*)NativePtr)->type;
}

void Flood::RenderBatch::Type::set(Flood::PrimitiveType value)
{
    ((::RenderBatch*)NativePtr)->type = (::PrimitiveType)value;
}

Flood::PrimitiveRasterMode Flood::RenderBatch::Mode::get()
{
    return (Flood::PrimitiveRasterMode)((::RenderBatch*)NativePtr)->mode;
}

void Flood::RenderBatch::Mode::set(Flood::PrimitiveRasterMode value)
{
    ((::RenderBatch*)NativePtr)->mode = (::PrimitiveRasterMode)value;
}

Flood::GeometryBuffer^ Flood::RenderBatch::Gb::get()
{
    return gcnew Flood::GeometryBuffer((::GeometryBuffer*)((::RenderBatch*)NativePtr)->gb.get());
}

void Flood::RenderBatch::Gb::set(Flood::GeometryBuffer^ value)
{
    ((::RenderBatch*)NativePtr)->gb = (::GeometryBuffer*)value->NativePtr;
}

Flood::UniformBuffer^ Flood::RenderBatch::Ub::get()
{
    return gcnew Flood::UniformBuffer((::UniformBuffer*)((::RenderBatch*)NativePtr)->ub.get());
}

void Flood::RenderBatch::Ub::set(Flood::UniformBuffer^ value)
{
    ((::RenderBatch*)NativePtr)->ub = (::UniformBuffer*)value->NativePtr;
}

Flood::ResourceHandle<Flood::Material^> Flood::RenderBatch::Material::get()
{
    return Flood::ResourceHandle<Flood::Material^>(((::RenderBatch*)NativePtr)->material.id);
}

void Flood::RenderBatch::Material::set(Flood::ResourceHandle<Flood::Material^> value)
{
    ((::RenderBatch*)NativePtr)->material = (HandleId)value.Id;
}

Flood::RenderBatch^ Flood::FloodRenderBatch::RenderBatchCreate(Flood::Allocator^ _0)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto ret = ::RenderBatchCreate(arg0);
    return gcnew Flood::RenderBatch((::RenderBatch*)ret);
}

