/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderContext.h"
#include "Color.h"
#include "RenderTarget.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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

void Flood::RenderContext::SetTarget(Flood::RenderTarget^ target)
{
    auto arg0 = (::RenderTarget*)target->NativePtr;
    ((::RenderContext*)NativePtr)->setTarget(arg0);
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

