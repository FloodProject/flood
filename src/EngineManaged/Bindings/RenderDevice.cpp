/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderDevice.h"
#include "Material.h"
#include "RenderBackend.h"
#include "RenderBatch.h"
#include "RenderBuffer.h"
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
    return __Instance == obj->__Instance;
}

int Flood::RenderDevice::GetHashCode()
{
    return (int)NativePtr;
}

Flood::RenderDevice^ Flood::RenderDevice::GetRenderDevice()
{
    auto __ret = ::GetRenderDevice();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderDevice((::RenderDevice*)__ret);
}

System::IntPtr Flood::RenderDevice::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderDevice::__Instance::set(System::IntPtr object)
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

Flood::RenderBackend^ Flood::RenderDevice::Backend::get()
{
    auto __ret = ((::RenderDevice*)NativePtr)->getBackend();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderBackend((::RenderBackend*)__ret);
}

void Flood::RenderDevice::Backend::set(Flood::RenderBackend^ value)
{
    auto v = value;
    auto arg0 = (::RenderBackend*)v->NativePtr;
    ((::RenderDevice*)NativePtr)->setBackend(arg0);
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

