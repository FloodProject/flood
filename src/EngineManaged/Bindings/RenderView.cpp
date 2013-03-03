/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderView.h"
#include "RenderTarget.h"
#include "Vector.h"
#include "Color.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::RenderView::RenderView(::RenderView* native)
{
    NativePtr = native;
}

Flood::RenderView::RenderView(System::IntPtr native)
{
    auto __native = (::RenderView*)native.ToPointer();
    NativePtr = __native;
}

Flood::RenderView::RenderView()
{
    NativePtr = new ::RenderView();
}

Flood::RenderTarget^ Flood::RenderView::GetRenderTarget()
{
    auto ret = ((::RenderView*)NativePtr)->getRenderTarget();
    return gcnew Flood::RenderTarget((::RenderTarget*)ret);
}

void Flood::RenderView::SetRenderTarget(Flood::RenderTarget^ v)
{
    auto arg0 = (::RenderTarget*)v->NativePtr;
    ((::RenderView*)NativePtr)->setRenderTarget(arg0);
}

Flood::Vector2i Flood::RenderView::GetOrigin()
{
    auto ret = ((::RenderView*)NativePtr)->getOrigin();
    return Flood::Vector2i((::Vector2i*)&ret);
}

void Flood::RenderView::SetOrigin(Flood::Vector2i v)
{
    auto arg0 = ::Vector2i();
    arg0.x = (int32)v.X;
    arg0.y = (int32)v.Y;

    ((::RenderView*)NativePtr)->setOrigin(arg0);
}

Flood::Vector2i Flood::RenderView::GetSize()
{
    auto ret = ((::RenderView*)NativePtr)->getSize();
    return Flood::Vector2i((::Vector2i*)&ret);
}

void Flood::RenderView::SetSize(Flood::Vector2i v)
{
    auto arg0 = ::Vector2i();
    arg0.x = (int32)v.X;
    arg0.y = (int32)v.Y;

    ((::RenderView*)NativePtr)->setSize(arg0);
}

Flood::Color Flood::RenderView::GetClearColor()
{
    auto ret = &((::RenderView*)NativePtr)->getClearColor();
    return Flood::Color((::Color*)&ret);
}

void Flood::RenderView::SetClearColor(Flood::Color v)
{
    auto arg0 = ::Color();
    ((::RenderView*)NativePtr)->setClearColor(arg0);
}

int Flood::RenderView::GetDepthPriority()
{
    auto ret = ((::RenderView*)NativePtr)->getDepthPriority();
    return ret;
}

void Flood::RenderView::SetDepthPriority(int v)
{
    ((::RenderView*)NativePtr)->setDepthPriority(v);
}

float Flood::RenderView::GetAspectRatio()
{
    auto ret = ((::RenderView*)NativePtr)->getAspectRatio();
    return ret;
}

bool Flood::RenderView::operator<(Flood::RenderView^ view)
{
    auto &arg0 = *(::RenderView*)view->NativePtr;
    auto ret = ((::RenderView*)NativePtr)->operator<(arg0);
    return ret;
}

void Flood::RenderView::HandleRenderTargetResize()
{
    ((::RenderView*)NativePtr)->handleRenderTargetResize();
}

