/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderContext.h"
#include "RenderTarget.h"
#include "Color.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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
    NativePtr = new ::RenderContext();
}

void Flood::RenderContext::MakeCurrent()
{
    NativePtr->makeCurrent();
}

void Flood::RenderContext::Init()
{
    NativePtr->init();
}

void Flood::RenderContext::ResetState()
{
    NativePtr->resetState();
}

Flood::Color Flood::RenderContext::GetPixel(unsigned short x, unsigned short y)
{
    auto arg0 = (uint16)x;
    auto arg1 = (uint16)y;
    auto ret = NativePtr->getPixel(arg0, arg1);
    return Flood::Color((::Color*)&ret);
}

