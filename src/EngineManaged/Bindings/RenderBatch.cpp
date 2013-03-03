/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderBatch.h"
#include "Memory.h"
#include "RenderView.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::RenderBatchRange::RenderBatchRange(::RenderBatchRange* native)
{
    NativePtr = native;
}

Flood::RenderBatchRange::RenderBatchRange(System::IntPtr native)
{
    auto __native = (::RenderBatchRange*)native.ToPointer();
    NativePtr = __native;
}

Flood::RenderBatchRange::RenderBatchRange()
{
    NativePtr = new ::RenderBatchRange();
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

Flood::RenderBatch^ Flood::FloodRenderBatch::RenderBatchCreate(Flood::Allocator^ _218)
{
    auto arg0 = (::Allocator*)_218->NativePtr;
    auto ret = ::RenderBatchCreate(arg0);
    return gcnew Flood::RenderBatch((::RenderBatch*)ret);
}

