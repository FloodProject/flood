/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Window.h"
#include "Vector.h"
#include "InputManager.h"
#include "RenderTarget.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::WindowSettings::WindowSettings(::WindowSettings* native)
{
    Title = marshalString<E_UTF8>(native->title);
    FullScreen = native->fullScreen;
    BitsPerPixel = native->bitsPerPixel;
    DepthBits = native->depthBits;
    StencilBits = native->stencilBits;
    AntialiasLevel = native->antialiasLevel;
    Handle = IntPtr(native->handle);
}

Flood::WindowSettings::WindowSettings(System::IntPtr native)
{
    auto __native = (::WindowSettings*)native.ToPointer();
    Title = marshalString<E_UTF8>(__native->title);
    FullScreen = __native->fullScreen;
    BitsPerPixel = __native->bitsPerPixel;
    DepthBits = __native->depthBits;
    StencilBits = __native->stencilBits;
    AntialiasLevel = __native->antialiasLevel;
    Handle = IntPtr(__native->handle);
}

Flood::WindowSettings::WindowSettings(unsigned short width, unsigned short height, System::String^ title, bool fullscreen)
{
}

Flood::Window::Window(::Window* native)
    : RenderTarget(native)
{
}

Flood::Window::Window(System::IntPtr native)
    : RenderTarget(native)
{
    auto __native = (::Window*)native.ToPointer();
}

Flood::Window::Window(Flood::WindowSettings settings)
    : RenderTarget(nullptr)
{
    auto arg0 = ::WindowSettings();
    arg0.title = marshalString<E_UTF8>(settings.Title);
    arg0.fullScreen = settings.FullScreen;
    arg0.bitsPerPixel = (uint16)settings.BitsPerPixel;
    arg0.depthBits = (uint16)settings.DepthBits;
    arg0.stencilBits = (uint16)settings.StencilBits;
    arg0.antialiasLevel = (uint16)settings.AntialiasLevel;
    arg0.handle = settings.Handle.ToPointer();

    NativePtr = new ::Window(arg0);
}

void Flood::Window::Update()
{
    NativePtr->update();
}

void Flood::Window::MakeCurrent()
{
    NativePtr->makeCurrent();
}

void Flood::Window::Show(bool visible)
{
    NativePtr->show(visible);
}

bool Flood::Window::PumpEvents()
{
    auto ret = NativePtr->pumpEvents();
    return ret;
}

void Flood::Window::SetTitle(System::String^ title)
{
    auto arg0 = marshalString<E_UTF8>(title);
    NativePtr->setTitle(arg0);
}

void Flood::Window::SetCursorVisible(bool state)
{
    NativePtr->setCursorVisible(state);
}

bool Flood::Window::IsCursorVisible()
{
    auto ret = NativePtr->isCursorVisible();
    return ret;
}

void Flood::Window::SetCursorCapture(bool state)
{
    NativePtr->setCursorCapture(state);
}

Flood::Vector2i Flood::Window::GetCursorPosition()
{
    auto ret = NativePtr->getCursorPosition();
    return Flood::Vector2i((::Vector2i*)&ret);
}

void Flood::Window::SetCursorPosition(int x, int y)
{
    NativePtr->setCursorPosition(x, y);
}

void Flood::Window::SetCursorPosition(Flood::Vector2i pos)
{
    auto arg0 = ::Vector2i();
    arg0.x = (int32)pos.X;
    arg0.y = (int32)pos.Y;

    NativePtr->setCursorPosition(arg0);
}

bool Flood::Window::HasFocus()
{
    auto ret = NativePtr->hasFocus();
    return ret;
}

Flood::InputManager^ Flood::Window::GetInput()
{
    auto ret = NativePtr->getInput();
    return gcnew Flood::InputManager((::InputManager*)ret);
}

Flood::Settings Flood::Window::GetSettings()
{
    auto ret = &NativePtr->getSettings();
    return Flood::Settings((::Settings*)&ret);
}

