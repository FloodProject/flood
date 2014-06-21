/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderBatch.h"
#include "GeometryBuffer.h"
#include "Memory.h"
#include "RenderQueue.h"
#include "RenderView.h"
#include "UniformBuffer.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RenderBatchRange::RenderBatchRange(::RenderBatchRange* native)
{
    __Start = native->start;
    __End = native->end;
}

Flood::RenderBatchRange::RenderBatchRange(System::IntPtr native)
{
    auto __native = (::RenderBatchRange*)native.ToPointer();
    __Start = __native->start;
    __End = __native->end;
}

unsigned short Flood::RenderBatchRange::Start::get()
{
    return __Start;
}

void Flood::RenderBatchRange::Start::set(unsigned short value)
{
    __Start = value;
}

unsigned short Flood::RenderBatchRange::End::get()
{
    return __End;
}

void Flood::RenderBatchRange::End::set(unsigned short value)
{
    __End = value;
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

bool Flood::RenderBatch::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RenderBatch^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::RenderBatch::GetHashCode()
{
    return (int)NativePtr;
}

Flood::RenderBatch^ Flood::RenderBatch::RenderBatchCreate(Flood::Allocator^ _0)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto __ret = ::RenderBatchCreate(arg0);
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::RenderBatch((::RenderBatch*)__ret);
}

System::IntPtr Flood::RenderBatch::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderBatch::__Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderBatch*)object.ToPointer();
}

Flood::RenderLayer Flood::RenderBatch::RenderLayer::get()
{
    auto __ret = ((::RenderBatch*)NativePtr)->getRenderLayer();
    return (Flood::RenderLayer)__ret;
}

void Flood::RenderBatch::RenderLayer::set(Flood::RenderLayer v)
{
    auto arg0 = (::RenderLayer)v;
    ((::RenderBatch*)NativePtr)->setRenderLayer(arg0);
}

int Flood::RenderBatch::RenderPriority::get()
{
    auto __ret = ((::RenderBatch*)NativePtr)->getRenderPriority();
    return __ret;
}

void Flood::RenderBatch::RenderPriority::set(int v)
{
    auto arg0 = (::int32)(::int32_t)v;
    ((::RenderBatch*)NativePtr)->setRenderPriority(arg0);
}

Flood::GeometryBuffer^ Flood::RenderBatch::GeometryBuffer::get()
{
    auto &__ret = ((::RenderBatch*)NativePtr)->getGeometryBuffer();
    return (Flood::GeometryBuffer^)((__ret.get() == nullptr) ? nullptr : gcnew Flood::GeometryBuffer((::GeometryBuffer*)__ret.get()));
}

void Flood::RenderBatch::GeometryBuffer::set(Flood::GeometryBuffer^ v)
{
    auto arg0 = (::GeometryBuffer*)v->NativePtr;
    ((::RenderBatch*)NativePtr)->setGeometryBuffer(arg0);
}

Flood::UniformBuffer^ Flood::RenderBatch::UniformBuffer::get()
{
    auto &__ret = ((::RenderBatch*)NativePtr)->getUniformBuffer();
    return (Flood::UniformBuffer^)((__ret.get() == nullptr) ? nullptr : gcnew Flood::UniformBuffer((::UniformBuffer*)__ret.get()));
}

void Flood::RenderBatch::UniformBuffer::set(Flood::UniformBuffer^ v)
{
    auto arg0 = (::UniformBuffer*)v->NativePtr;
    ((::RenderBatch*)NativePtr)->setUniformBuffer(arg0);
}

Flood::ResourceHandle<Flood::Material^> Flood::RenderBatch::Material::get()
{
    auto &__ret = ((::RenderBatch*)NativePtr)->getMaterial();
    return (Flood::ResourceHandle<Flood::Material^>)(Flood::ResourceHandle<Flood::Material^>(__ret.id));
}

void Flood::RenderBatch::Material::set(Flood::ResourceHandle<Flood::Material^> v)
{
    auto arg0 = (HandleId)v.Id;
    ((::RenderBatch*)NativePtr)->setMaterial(arg0);
}

Flood::PrimitiveRasterMode Flood::RenderBatch::PrimitiveRasterMode::get()
{
    auto __ret = ((::RenderBatch*)NativePtr)->getPrimitiveRasterMode();
    return (Flood::PrimitiveRasterMode)__ret;
}

void Flood::RenderBatch::PrimitiveRasterMode::set(Flood::PrimitiveRasterMode v)
{
    auto arg0 = (::PrimitiveRasterMode)v;
    ((::RenderBatch*)NativePtr)->setPrimitiveRasterMode(arg0);
}

Flood::PrimitiveType Flood::RenderBatch::PrimitiveType::get()
{
    auto __ret = ((::RenderBatch*)NativePtr)->getPrimitiveType();
    return (Flood::PrimitiveType)__ret;
}

void Flood::RenderBatch::PrimitiveType::set(Flood::PrimitiveType v)
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
    _marshal0.start = (::RenderIndexOffset)(::uint16)(::uint16_t)value.Start;
    _marshal0.end = (::RenderIndexOffset)(::uint16)(::uint16_t)value.End;
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
    ((::RenderBatch*)NativePtr)->priority = (::int32)(::int32_t)value;
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
    return (((::RenderBatch*)NativePtr)->gb.get() == nullptr) ? nullptr : gcnew Flood::GeometryBuffer((::GeometryBuffer*)((::RenderBatch*)NativePtr)->gb.get());
}

void Flood::RenderBatch::Gb::set(Flood::GeometryBuffer^ value)
{
    ((::RenderBatch*)NativePtr)->gb = (::GeometryBuffer*)value->NativePtr;
}

Flood::UniformBuffer^ Flood::RenderBatch::Ub::get()
{
    return (((::RenderBatch*)NativePtr)->ub.get() == nullptr) ? nullptr : gcnew Flood::UniformBuffer((::UniformBuffer*)((::RenderBatch*)NativePtr)->ub.get());
}

void Flood::RenderBatch::Ub::set(Flood::UniformBuffer^ value)
{
    ((::RenderBatch*)NativePtr)->ub = (::UniformBuffer*)value->NativePtr;
}

Flood::ResourceHandle<Flood::Material^> Flood::RenderBatch::Material1::get()
{
    return Flood::ResourceHandle<Flood::Material^>(((::RenderBatch*)NativePtr)->material.id);
}

void Flood::RenderBatch::Material1::set(Flood::ResourceHandle<Flood::Material^> value)
{
    ((::RenderBatch*)NativePtr)->material = (HandleId)value.Id;
}

