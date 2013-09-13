/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderDevice.h"
#include "Material.h"
#include "RenderBatch.h"
#include "RenderContext.h"
#include "RenderQueue.h"
#include "RenderTarget.h"
#include "RenderView.h"
#include "Texture.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RenderDevice::RenderDevice(::RenderDevice* native)
{
    NativePtr = native;
}

Flood::RenderDevice::RenderDevice(System::IntPtr native)
{
    auto __native = (::RenderDevice*)native.ToPointer();
    NativePtr = __native;
}

Flood::RenderDevice::RenderDevice()
{
    NativePtr = new ::RenderDevice();
}

void Flood::RenderDevice::Render(Flood::RenderBlock^ queue)
{
    auto &arg0 = *(::RenderBlock*)queue->NativePtr;
    ((::RenderDevice*)NativePtr)->render(arg0);
}

void Flood::RenderDevice::UpdateRenderTargets()
{
    ((::RenderDevice*)NativePtr)->updateRenderTargets();
}

void Flood::RenderDevice::ClearView()
{
    ((::RenderDevice*)NativePtr)->clearView();
}

bool Flood::RenderDevice::IsFixedPipeline()
{
    auto __ret = ((::RenderDevice*)NativePtr)->isFixedPipeline();
    return __ret;
}

bool Flood::RenderDevice::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RenderDevice^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::RenderDevice::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderDevice::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderDevice::Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderDevice*)object.ToPointer();
}

Flood::RenderPipeline Flood::RenderDevice::Pipeline::get()
{
    auto __ret = ((::RenderDevice*)NativePtr)->getPipeline();
    return (Flood::RenderPipeline)__ret;
}

void Flood::RenderDevice::Pipeline::set(Flood::RenderPipeline value)
{
    auto v = value;
    auto arg0 = (::RenderPipeline)v;
    ((::RenderDevice*)NativePtr)->setPipeline(arg0);
}

Flood::RenderTarget^ Flood::RenderDevice::RenderTarget::get()
{
    auto __ret = ((::RenderDevice*)NativePtr)->getRenderTarget();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderTarget((::RenderTarget*)__ret);
}

void Flood::RenderDevice::RenderTarget::set(Flood::RenderTarget^ value)
{
    auto target = value;
    auto arg0 = (::RenderTarget*)target->NativePtr;
    ((::RenderDevice*)NativePtr)->setRenderTarget(arg0);
}

Flood::RenderContext^ Flood::RenderDevice::ActiveContext::get()
{
    auto __ret = ((::RenderDevice*)NativePtr)->getActiveContext();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderContext((::RenderContext*)__ret);
}

void Flood::RenderDevice::ActiveContext::set(Flood::RenderContext^ value)
{
    auto v = value;
    auto arg0 = (::RenderContext*)v->NativePtr;
    ((::RenderDevice*)NativePtr)->setActiveContext(arg0);
}

Flood::RenderView^ Flood::RenderDevice::ActiveView::get()
{
    auto __ret = ((::RenderDevice*)NativePtr)->getActiveView();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderView((::RenderView*)__ret);
}

void Flood::RenderDevice::ActiveView::set(Flood::RenderView^ value)
{
    auto view = value;
    auto arg0 = (::RenderView*)view->NativePtr;
    ((::RenderDevice*)NativePtr)->setActiveView(arg0);
}

Flood::RenderDevice^ Flood::FloodRenderDevice::GetRenderDevice()
{
    auto __ret = ::GetRenderDevice();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderDevice((::RenderDevice*)__ret);
}

