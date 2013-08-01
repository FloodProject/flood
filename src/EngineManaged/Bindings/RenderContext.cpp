/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderContext.h"
#include "Color.h"
#include "RenderTarget.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::RenderContextSettings::RenderContextSettings(::RenderContextSettings* native)
{
    BitsPerPixel = native->bitsPerPixel;
    DepthBits = native->depthBits;
    StencilBits = native->stencilBits;
    AntialiasLevel = native->antialiasLevel;
}

Flood::RenderContextSettings::RenderContextSettings(System::IntPtr native)
{
    auto __native = (::RenderContextSettings*)native.ToPointer();
    BitsPerPixel = __native->bitsPerPixel;
    DepthBits = __native->depthBits;
    StencilBits = __native->stencilBits;
    AntialiasLevel = __native->antialiasLevel;
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
    auto arg0 = (uint16)x;
    auto arg1 = (uint16)y;
    auto ret = ((::RenderContext*)NativePtr)->getPixel(arg0, arg1);
    return Flood::Color((::Color*)&ret);
}

bool Flood::RenderContext::Equals(System::Object^ object)
{
    if (!object) return false;
    return Instance == safe_cast<ICppInstance^>(object)->Instance;
}

int Flood::RenderContext::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderContext::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderContext::Instance::set(System::IntPtr object)
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

