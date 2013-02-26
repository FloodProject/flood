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
    NativePtr = native;
}

Flood::Settings::Settings(System::IntPtr native)
{
    NativePtr = (::Settings*)native.ToPointer();
}

Flood::Settings::Settings(unsigned short width, unsigned short height)
{
    auto arg0 = (uint16)width;
    auto arg1 = (uint16)height;
    NativePtr = new ::Settings(arg0, arg1);
}

Flood::Vector2i Flood::Settings::GetSize()
{
    auto ret = NativePtr->getSize();
    return Flood::Vector2i((::Vector2i*)&ret);
}

Flood::RenderTarget::RenderTarget(::RenderTarget* native)
{
    NativePtr = native;
}

Flood::RenderTarget::RenderTarget(System::IntPtr native)
{
    NativePtr = (::RenderTarget*)native.ToPointer();
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

Flood::Settings^ Flood::RenderTarget::GetSettings()
{
    auto ret = &NativePtr->getSettings();
    return gcnew Flood::Settings((::Settings*)ret);
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

