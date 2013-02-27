/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderTarget.h"
#include "Vector.h"
#include "RenderContext.h"
#include "Color.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Settings::Settings(::Settings* native)
{
    Width = native->width;
    Height = native->height;
}

Flood::Settings::Settings(System::IntPtr native)
{
    auto __native = (::Settings*)native.ToPointer();
    Width = __native->width;
    Height = __native->height;
}

Flood::Settings::Settings(unsigned short width, unsigned short height)
{
}

Flood::Vector2i Flood::Settings::GetSize()
{
    auto this0 = (::Settings*) 0;
    auto ret = this0->getSize();
    return Flood::Vector2i((::Vector2i*)&ret);
}

Flood::RenderTarget::RenderTarget(::RenderTarget* native)
{
    NativePtr = native;
}

Flood::RenderTarget::RenderTarget(System::IntPtr native)
{
    auto __native = (::RenderTarget*)native.ToPointer();
    NativePtr = __native;
}

Flood::RenderTarget::RenderTarget()
{
}

void Flood::RenderTarget::RemoveViews()
{
    NativePtr->removeViews();
}

void Flood::RenderTarget::MakeCurrent()
{
    NativePtr->makeCurrent();
}

void Flood::RenderTarget::Update()
{
    NativePtr->update();
}

Flood::Settings Flood::RenderTarget::GetSettings()
{
    auto ret = &NativePtr->getSettings();
    return Flood::Settings((::Settings*)&ret);
}

Flood::RenderContext^ Flood::RenderTarget::GetContext()
{
    auto ret = NativePtr->getContext();
    return gcnew Flood::RenderContext((::RenderContext*)ret);
}

void Flood::RenderTarget::SetContext(Flood::RenderContext^ context)
{
    auto arg0 = context->NativePtr;
    NativePtr->setContext(arg0);
}

