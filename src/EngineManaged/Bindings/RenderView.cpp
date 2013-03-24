/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderView.h"
#include "Color.h"
#include "Matrix4x3.h"
#include "Matrix4x4.h"
#include "RenderTarget.h"
#include "ResourceHandle.h"
#include "Vector.h"

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
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)v.X;
    _marshal0.y = (int32)v.Y;
    auto arg0 = _marshal0;
    ((::RenderView*)NativePtr)->setOrigin(arg0);
}

Flood::Vector2i Flood::RenderView::GetSize()
{
    auto ret = ((::RenderView*)NativePtr)->getSize();
    return Flood::Vector2i((::Vector2i*)&ret);
}

void Flood::RenderView::SetSize(Flood::Vector2i v)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)v.X;
    _marshal0.y = (int32)v.Y;
    auto arg0 = _marshal0;
    ((::RenderView*)NativePtr)->setSize(arg0);
}

Flood::Color Flood::RenderView::GetClearColor()
{
    auto ret = ((::RenderView*)NativePtr)->getClearColor();
    return Flood::Color((::Color*)&ret);
}

void Flood::RenderView::SetClearColor(Flood::Color v)
{
    auto _marshal0 = ::Color();
    auto arg0 = _marshal0;
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

Flood::Vector2i Flood::RenderView::Origin::get()
{
    return Flood::Vector2i((::Vector2i*)&((::RenderView*)NativePtr)->origin);
}

void Flood::RenderView::Origin::set(Flood::Vector2i value)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)value.X;
    _marshal0.y = (int32)value.Y;
    ((::RenderView*)NativePtr)->origin = _marshal0;
}

Flood::Vector2i Flood::RenderView::Size::get()
{
    return Flood::Vector2i((::Vector2i*)&((::RenderView*)NativePtr)->size);
}

void Flood::RenderView::Size::set(Flood::Vector2i value)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)value.X;
    _marshal0.y = (int32)value.Y;
    ((::RenderView*)NativePtr)->size = _marshal0;
}

Flood::RenderTarget^ Flood::RenderView::Target::get()
{
    return gcnew Flood::RenderTarget((::RenderTarget*)((::RenderView*)NativePtr)->target);
}

void Flood::RenderView::Target::set(Flood::RenderTarget^ value)
{
    ((::RenderView*)NativePtr)->target = (::RenderTarget*)value->NativePtr;
}

Flood::Color Flood::RenderView::ClearColor::get()
{
    return Flood::Color((::Color*)&((::RenderView*)NativePtr)->clearColor);
}

void Flood::RenderView::ClearColor::set(Flood::Color value)
{
    auto _marshal0 = ::Color();
    ((::RenderView*)NativePtr)->clearColor = _marshal0;
}

int Flood::RenderView::DepthPriority::get()
{
    return ((::RenderView*)NativePtr)->depthPriority;
}

void Flood::RenderView::DepthPriority::set(int value)
{
    ((::RenderView*)NativePtr)->depthPriority = value;
}

Flood::Matrix4x4 Flood::RenderView::ProjectionMatrix::get()
{
    return Flood::Matrix4x4((::Matrix4x4*)&((::RenderView*)NativePtr)->projectionMatrix);
}

void Flood::RenderView::ProjectionMatrix::set(Flood::Matrix4x4 value)
{
    auto _marshal0 = ::Matrix4x4();
    _marshal0.m11 = value.M11;
    _marshal0.m12 = value.M12;
    _marshal0.m13 = value.M13;
    _marshal0.m14 = value.M14;
    _marshal0.m21 = value.M21;
    _marshal0.m22 = value.M22;
    _marshal0.m23 = value.M23;
    _marshal0.m24 = value.M24;
    _marshal0.m31 = value.M31;
    _marshal0.m32 = value.M32;
    _marshal0.m33 = value.M33;
    _marshal0.m34 = value.M34;
    _marshal0.tx = value.Tx;
    _marshal0.ty = value.Ty;
    _marshal0.tz = value.Tz;
    _marshal0.tw = value.Tw;
    ((::RenderView*)NativePtr)->projectionMatrix = _marshal0;
}

Flood::Matrix4x3 Flood::RenderView::ViewMatrix::get()
{
    return Flood::Matrix4x3((::Matrix4x3*)&((::RenderView*)NativePtr)->viewMatrix);
}

void Flood::RenderView::ViewMatrix::set(Flood::Matrix4x3 value)
{
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = value.M11;
    _marshal0.m12 = value.M12;
    _marshal0.m13 = value.M13;
    _marshal0.m21 = value.M21;
    _marshal0.m22 = value.M22;
    _marshal0.m23 = value.M23;
    _marshal0.m31 = value.M31;
    _marshal0.m32 = value.M32;
    _marshal0.m33 = value.M33;
    _marshal0.tx = value.Tx;
    _marshal0.ty = value.Ty;
    _marshal0.tz = value.Tz;
    ((::RenderView*)NativePtr)->viewMatrix = _marshal0;
}

