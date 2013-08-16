/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "RenderTarget.h"
#include "RenderContext.h"
#include "RenderView.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

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
    ::Settings _native((::uint16)width, (::uint16)height);
    this->Width = _native.width;
    this->Height = _native.height;
}

Flood::Vector2i Flood::Settings::Size::get()
{
    auto _this0 = ::Settings();
    _this0.width = (::uint16)(*this).Width;
    _this0.height = (::uint16)(*this).Height;
    auto ret = _this0.getSize();
    Width = _this0.width;
    Height = _this0.height;
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

bool Flood::RenderTarget::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<RenderTarget^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::RenderTarget::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderTarget::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderTarget::Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderTarget*)object.ToPointer();
}

Flood::Settings Flood::RenderTarget::Settings::get()
{
    auto &ret = ((::RenderTarget*)NativePtr)->getSettings();
    return Flood::Settings((::Settings*)&ret);
}

Flood::RenderContext^ Flood::RenderTarget::Context::get()
{
    auto ret = ((::RenderTarget*)NativePtr)->getContext();
    return gcnew Flood::RenderContext((::RenderContext*)ret);
}

void Flood::RenderTarget::Context::set(Flood::RenderContext^ value)
{
    auto context = value;
    auto arg0 = (::RenderContext*)context->NativePtr;
    ((::RenderTarget*)NativePtr)->setContext(arg0);
}

System::Collections::Generic::List<Flood::RenderView^>^ Flood::RenderTarget::Views::get()
{
    auto &ret = ((::RenderTarget*)NativePtr)->getViews();
    auto _tmpret = gcnew System::Collections::Generic::List<Flood::RenderView^>();
    for(auto _element : ret)
    {
        auto _marshalElement = gcnew Flood::RenderView((::RenderView*)_element);
        _tmpret->Add(_marshalElement);
    }
    return _tmpret;
}

System::IntPtr Flood::RenderTarget::UserData::get()
{
    auto ret = ((::RenderTarget*)NativePtr)->getUserData();
    return IntPtr(ret);
}

void Flood::RenderTarget::UserData::set(System::IntPtr value)
{
    auto v = value;
    auto arg0 = v.ToPointer();
    ((::RenderTarget*)NativePtr)->setUserData(arg0);
}

void Flood::RenderTarget::TargetResize::add(System::Action<Flood::Settings>^ evt)
{
    if (!_TargetResizeDelegateInstance)
    {
        _TargetResizeDelegateInstance = gcnew _TargetResizeDelegate(this, &Flood::RenderTarget::_TargetResizeRaise);
        auto _fptr = (void (*)(const ::Settings&))Marshal::GetFunctionPointerForDelegate(_TargetResizeDelegateInstance).ToPointer();
        ((::RenderTarget*)NativePtr)->onTargetResize.Connect(_fptr);
    }
    _TargetResize = static_cast<System::Action<Flood::Settings>^>(System::Delegate::Combine(_TargetResize, evt));
}

void Flood::RenderTarget::TargetResize::remove(System::Action<Flood::Settings>^ evt)
{
    _TargetResize = static_cast<System::Action<Flood::Settings>^>(System::Delegate::Remove(_TargetResize, evt));
}

void Flood::RenderTarget::TargetResize::raise(Flood::Settings _0)
{
    _TargetResize(_0);
}

void Flood::RenderTarget::_TargetResizeRaise(const ::Settings& _0)
{
    TargetResize::raise(Flood::Settings((::Settings*)&_0));
}

