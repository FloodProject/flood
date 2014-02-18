/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderContext.h"
#include "Color.h"
#include "RenderBackend.h"
#include "RenderBuffer.h"
#include "RenderDevice.h"
#include "RenderTarget.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RenderContextSettings::RenderContextSettings(::RenderContextSettings* native)
{
    __BitsPerPixel = native->bitsPerPixel;
    __DepthBits = native->depthBits;
    __StencilBits = native->stencilBits;
    __AntialiasLevel = native->antialiasLevel;
}

Flood::RenderContextSettings::RenderContextSettings(System::IntPtr native)
{
    auto __native = (::RenderContextSettings*)native.ToPointer();
    __BitsPerPixel = __native->bitsPerPixel;
    __DepthBits = __native->depthBits;
    __StencilBits = __native->stencilBits;
    __AntialiasLevel = __native->antialiasLevel;
}

unsigned short Flood::RenderContextSettings::BitsPerPixel::get()
{
    return __BitsPerPixel;
}

void Flood::RenderContextSettings::BitsPerPixel::set(unsigned short value)
{
    __BitsPerPixel = value;
}

unsigned short Flood::RenderContextSettings::DepthBits::get()
{
    return __DepthBits;
}

void Flood::RenderContextSettings::DepthBits::set(unsigned short value)
{
    __DepthBits = value;
}

unsigned short Flood::RenderContextSettings::StencilBits::get()
{
    return __StencilBits;
}

void Flood::RenderContextSettings::StencilBits::set(unsigned short value)
{
    __StencilBits = value;
}

unsigned short Flood::RenderContextSettings::AntialiasLevel::get()
{
    return __AntialiasLevel;
}

void Flood::RenderContextSettings::AntialiasLevel::set(unsigned short value)
{
    __AntialiasLevel = value;
}

Flood::RenderContext::RenderContext(::RenderContext* native)
{
    NativePtr = native;
}

Flood::RenderContext::RenderContext(System::IntPtr native)
{
    auto __native = (::RenderContext*)native.ToPointer();
    NativePtr = __native;
}

Flood::RenderContext::RenderContext()
{
}

void Flood::RenderContext::MakeCurrent(Flood::RenderTarget^ target)
{
    auto arg0 = (::RenderTarget*)target->NativePtr;
    ((::RenderContext*)NativePtr)->makeCurrent(arg0);
}

void Flood::RenderContext::Init()
{
    ((::RenderContext*)NativePtr)->init();
}

void Flood::RenderContext::ResetState()
{
    ((::RenderContext*)NativePtr)->resetState();
}

Flood::Color Flood::RenderContext::GetPixel(unsigned short x, unsigned short y)
{
    auto arg0 = (::uint16)(::uint16_t)x;
    auto arg1 = (::uint16)(::uint16_t)y;
    auto __ret = ((::RenderContext*)NativePtr)->getPixel(arg0, arg1);
    return Flood::Color((::Color*)&__ret);
}

Flood::RenderBuffer^ Flood::RenderContext::CreateRenderBuffer(Flood::Settings _0)
{
    auto _marshal0 = ::Settings();
    _marshal0.width = (::uint16)(::uint16_t)_0.Width;
    _marshal0.height = (::uint16)(::uint16_t)_0.Height;
    auto arg0 = _marshal0;
    auto __ret = ((::RenderContext*)NativePtr)->createRenderBuffer(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderBuffer((::RenderBuffer*)__ret);
}

bool Flood::RenderContext::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RenderContext^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::RenderContext::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderContext::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderContext::__Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderContext*)object.ToPointer();
}

void Flood::RenderContext::Target::set(Flood::RenderTarget^ value)
{
    auto target = value;
    auto arg0 = (::RenderTarget*)target->NativePtr;
    ((::RenderContext*)NativePtr)->setTarget(arg0);
}

bool Flood::RenderContext::InitDone::get()
{
    return ((::RenderContext*)NativePtr)->initDone;
}

void Flood::RenderContext::InitDone::set(bool value)
{
    ((::RenderContext*)NativePtr)->initDone = value;
}

Flood::RenderTarget^ Flood::RenderContext::CurrentTarget::get()
{
    return gcnew Flood::RenderTarget((::RenderTarget*)((::RenderContext*)NativePtr)->currentTarget);
}

void Flood::RenderContext::CurrentTarget::set(Flood::RenderTarget^ value)
{
    ((::RenderContext*)NativePtr)->currentTarget = (::RenderTarget*)value->NativePtr;
}

Flood::RenderBackend^ Flood::RenderContext::Backend::get()
{
    return gcnew Flood::RenderBackend((::RenderBackend*)((::RenderContext*)NativePtr)->backend);
}

void Flood::RenderContext::Backend::set(Flood::RenderBackend^ value)
{
    ((::RenderContext*)NativePtr)->backend = (::RenderBackend*)value->NativePtr;
}

