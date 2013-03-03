/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderTarget.h"
#include "RenderView.h"
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

Flood::RenderView^ Flood::RenderTarget::CreateView()
{
    auto ret = ((::RenderTarget*)NativePtr)->createView();
    return gcnew Flood::RenderView((::RenderView*)ret);
}

void Flood::RenderTarget::RemoveViews()
{
    ((::RenderTarget*)NativePtr)->removeViews();
}

void Flood::RenderTarget::MakeCurrent()
{
    ((::RenderTarget*)NativePtr)->makeCurrent();
}

void Flood::RenderTarget::Update()
{
    ((::RenderTarget*)NativePtr)->update();
}

Flood::Settings Flood::RenderTarget::GetSettings()
{
    auto ret = &((::RenderTarget*)NativePtr)->getSettings();
    return Flood::Settings((::Settings*)&ret);
}

Flood::RenderContext^ Flood::RenderTarget::GetContext()
{
    auto ret = ((::RenderTarget*)NativePtr)->getContext();
    return gcnew Flood::RenderContext((::RenderContext*)ret);
}

void Flood::RenderTarget::SetContext(Flood::RenderContext^ context)
{
    auto arg0 = (::RenderContext*)context->NativePtr;
    ((::RenderTarget*)NativePtr)->setContext(arg0);
}

System::IntPtr Flood::RenderTarget::GetUserData()
{
    auto ret = ((::RenderTarget*)NativePtr)->getUserData();
    return IntPtr(ret);
}

void Flood::RenderTarget::SetUserData(System::IntPtr v)
{
    auto arg0 = v.ToPointer();
    ((::RenderTarget*)NativePtr)->setUserData(arg0);
}

