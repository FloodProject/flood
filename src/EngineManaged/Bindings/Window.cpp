/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Window.h"
#include "InputManager.h"
#include "RenderContext.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::WindowSettings::WindowSettings(::WindowSettings* native)
{
    __Width = native->width;
    __Height = native->height;
    __Title = clix::marshalString<clix::E_UTF8>(native->title);
    __Handle = IntPtr(native->handle);
    __Styles = (Flood::WindowStyles)native->styles;
}

Flood::WindowSettings::WindowSettings(System::IntPtr native)
{
    auto __native = (::WindowSettings*)native.ToPointer();
    __Width = __native->width;
    __Height = __native->height;
    __Title = clix::marshalString<clix::E_UTF8>(__native->title);
    __Handle = IntPtr(__native->handle);
    __Styles = (Flood::WindowStyles)__native->styles;
}

Flood::WindowSettings::WindowSettings(unsigned short width, unsigned short height, System::String^ title, Flood::WindowStyles styles)
{
    ::WindowSettings _native((::uint16)(::uint16_t)width, (::uint16)(::uint16_t)height, clix::marshalString<clix::E_UTF8>(title), (::WindowStyles)styles);
    this->Width = _native.width;
    this->Height = _native.height;
    this->Title = clix::marshalString<clix::E_UTF8>(_native.title);
    this->Handle = IntPtr(_native.handle);
    this->Styles = (Flood::WindowStyles)_native.styles;
}

Flood::Vector2i Flood::WindowSettings::Size::get()
{
    auto _this0 = ::WindowSettings();
    _this0.width = (::uint16)(::uint16_t)(*this).Width;
    _this0.height = (::uint16)(::uint16_t)(*this).Height;
    _this0.title = clix::marshalString<clix::E_UTF8>((*this).Title);
    _this0.handle = (void*)(*this).Handle.ToPointer();
    _this0.styles = (::WindowStyles)(*this).Styles;
    auto __ret = _this0.getSize();
    __Width = _this0.width;
    __Height = _this0.height;
    __Title = clix::marshalString<clix::E_UTF8>(_this0.title);
    __Handle = IntPtr(_this0.handle);
    __Styles = (Flood::WindowStyles)_this0.styles;
    return Flood::Vector2i((::Vector2i*)&__ret);
}

unsigned short Flood::WindowSettings::Width::get()
{
    return __Width;
}

void Flood::WindowSettings::Width::set(unsigned short value)
{
    __Width = value;
}

unsigned short Flood::WindowSettings::Height::get()
{
    return __Height;
}

void Flood::WindowSettings::Height::set(unsigned short value)
{
    __Height = value;
}

System::String^ Flood::WindowSettings::Title::get()
{
    return __Title;
}

void Flood::WindowSettings::Title::set(System::String^ value)
{
    __Title = value;
}

System::IntPtr Flood::WindowSettings::Handle::get()
{
    return __Handle;
}

void Flood::WindowSettings::Handle::set(System::IntPtr value)
{
    __Handle = value;
}

Flood::WindowStyles Flood::WindowSettings::Styles::get()
{
    return __Styles;
}

void Flood::WindowSettings::Styles::set(Flood::WindowStyles value)
{
    __Styles = value;
}

Flood::Window::Window(::Window* native)
    : Flood::RenderTarget((::RenderTarget*)native)
{
}

Flood::Window::Window(System::IntPtr native)
    : Flood::RenderTarget(native)
{
    auto __native = (::Window*)native.ToPointer();
}

Flood::Window::Window(Flood::WindowSettings settings)
    : Flood::RenderTarget((::RenderTarget*)nullptr)
{
}

Flood::RenderContext^ Flood::Window::CreateContext(Flood::RenderContextSettings _0)
{
    auto _marshal0 = ::RenderContextSettings();
    _marshal0.bitsPerPixel = (::uint16)(::uint16_t)_0.BitsPerPixel;
    _marshal0.depthBits = (::uint16)(::uint16_t)_0.DepthBits;
    _marshal0.stencilBits = (::uint16)(::uint16_t)_0.StencilBits;
    _marshal0.antialiasLevel = (::uint16)(::uint16_t)_0.AntialiasLevel;
    auto arg0 = _marshal0;
    auto __ret = ((::Window*)NativePtr)->createContext(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderContext((::RenderContext*)__ret);
}

void Flood::Window::Update()
{
    ((::Window*)NativePtr)->update();
}

void Flood::Window::MakeCurrent()
{
    ((::Window*)NativePtr)->makeCurrent();
}

void Flood::Window::Show(bool visible)
{
    ((::Window*)NativePtr)->show(visible);
}

bool Flood::Window::PumpEvents()
{
    auto __ret = ((::Window*)NativePtr)->pumpEvents();
    return __ret;
}

bool Flood::Window::IsCursorVisible()
{
    auto __ret = ((::Window*)NativePtr)->isCursorVisible();
    return __ret;
}

void Flood::Window::SetCursorPosition(int x, int y)
{
    ((::Window*)NativePtr)->setCursorPosition(x, y);
}

bool Flood::Window::HasFocus()
{
    auto __ret = ((::Window*)NativePtr)->hasFocus();
    return __ret;
}

bool Flood::Window::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Window^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Window::GetHashCode()
{
    return (int)NativePtr;
}

void Flood::Window::Title::set(System::String^ value)
{
    auto title = value;
    auto arg0 = clix::marshalString<clix::E_UTF8>(title);
    ((::Window*)NativePtr)->setTitle(arg0);
}

void Flood::Window::CursorVisible::set(bool value)
{
    auto state = value;
    ((::Window*)NativePtr)->setCursorVisible(state);
}

void Flood::Window::CursorCapture::set(bool value)
{
    auto state = value;
    ((::Window*)NativePtr)->setCursorCapture(state);
}

Flood::Vector2i Flood::Window::CursorPosition::get()
{
    auto __ret = ((::Window*)NativePtr)->getCursorPosition();
    return Flood::Vector2i((::Vector2i*)&__ret);
}

Flood::InputManager^ Flood::Window::Input::get()
{
    auto __ret = ((::Window*)NativePtr)->getInput();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::InputManager((::InputManager*)__ret);
}

Flood::Settings Flood::Window::Settings::get()
{
    auto &__ret = ((::Window*)NativePtr)->getSettings();
    return Flood::Settings((::Settings*)&__ret);
}

void Flood::Window::Idle::add(System::Action^ evt)
{
    if (!_IdleDelegateInstance)
    {
        _IdleDelegateInstance = gcnew _IdleDelegate(this, &Flood::Window::_IdleRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_IdleDelegateInstance).ToPointer();
        ((::Window*)NativePtr)->onIdle.Connect(_fptr);
    }
    _Idle = static_cast<System::Action^>(System::Delegate::Combine(_Idle, evt));
}

void Flood::Window::Idle::remove(System::Action^ evt)
{
    _Idle = static_cast<System::Action^>(System::Delegate::Remove(_Idle, evt));
}

void Flood::Window::Idle::raise()
{
    _Idle();
}

void Flood::Window::_IdleRaise()
{
    Idle::raise();
}

void Flood::Window::Render::add(System::Action^ evt)
{
    if (!_RenderDelegateInstance)
    {
        _RenderDelegateInstance = gcnew _RenderDelegate(this, &Flood::Window::_RenderRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_RenderDelegateInstance).ToPointer();
        ((::Window*)NativePtr)->onRender.Connect(_fptr);
    }
    _Render = static_cast<System::Action^>(System::Delegate::Combine(_Render, evt));
}

void Flood::Window::Render::remove(System::Action^ evt)
{
    _Render = static_cast<System::Action^>(System::Delegate::Remove(_Render, evt));
}

void Flood::Window::Render::raise()
{
    _Render();
}

void Flood::Window::_RenderRaise()
{
    Render::raise();
}

void Flood::Window::WindowClose::add(System::Action^ evt)
{
    if (!_WindowCloseDelegateInstance)
    {
        _WindowCloseDelegateInstance = gcnew _WindowCloseDelegate(this, &Flood::Window::_WindowCloseRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_WindowCloseDelegateInstance).ToPointer();
        ((::Window*)NativePtr)->onWindowClose.Connect(_fptr);
    }
    _WindowClose = static_cast<System::Action^>(System::Delegate::Combine(_WindowClose, evt));
}

void Flood::Window::WindowClose::remove(System::Action^ evt)
{
    _WindowClose = static_cast<System::Action^>(System::Delegate::Remove(_WindowClose, evt));
}

void Flood::Window::WindowClose::raise()
{
    _WindowClose();
}

void Flood::Window::_WindowCloseRaise()
{
    WindowClose::raise();
}

void Flood::Window::WindowFocusChange::add(System::Action<bool>^ evt)
{
    if (!_WindowFocusChangeDelegateInstance)
    {
        _WindowFocusChangeDelegateInstance = gcnew _WindowFocusChangeDelegate(this, &Flood::Window::_WindowFocusChangeRaise);
        auto _fptr = (void (*)(bool))Marshal::GetFunctionPointerForDelegate(_WindowFocusChangeDelegateInstance).ToPointer();
        ((::Window*)NativePtr)->onWindowFocusChange.Connect(_fptr);
    }
    _WindowFocusChange = static_cast<System::Action<bool>^>(System::Delegate::Combine(_WindowFocusChange, evt));
}

void Flood::Window::WindowFocusChange::remove(System::Action<bool>^ evt)
{
    _WindowFocusChange = static_cast<System::Action<bool>^>(System::Delegate::Remove(_WindowFocusChange, evt));
}

void Flood::Window::WindowFocusChange::raise(bool _0)
{
    _WindowFocusChange(_0);
}

void Flood::Window::_WindowFocusChangeRaise(bool _0)
{
    WindowFocusChange::raise(_0);
}

