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
    __Width = native->width;
    __Height = native->height;
}

Flood::Settings::Settings(System::IntPtr native)
{
    auto __native = (::Settings*)native.ToPointer();
    __Width = __native->width;
    __Height = __native->height;
}

Flood::Settings::Settings(unsigned short width, unsigned short height)
{
    ::Settings _native((::uint16)(::uint16_t)width, (::uint16)(::uint16_t)height);
    this->Width = _native.width;
    this->Height = _native.height;
}

Flood::Vector2i Flood::Settings::Size::get()
{
    auto _this0 = ::Settings();
    _this0.width = (::uint16)(::uint16_t)(*this).Width;
    _this0.height = (::uint16)(::uint16_t)(*this).Height;
    auto __ret = _this0.getSize();
    __Width = _this0.width;
    __Height = _this0.height;
    return Flood::Vector2i((::Vector2i*)&__ret);
}

unsigned short Flood::Settings::Width::get()
{
    return __Width;
}

void Flood::Settings::Width::set(unsigned short value)
{
    __Width = value;
}

unsigned short Flood::Settings::Height::get()
{
    return __Height;
}

void Flood::Settings::Height::set(unsigned short value)
{
    __Height = value;
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
    auto __ret = ((::RenderTarget*)NativePtr)->createView();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderView((::RenderView*)__ret);
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
    return __Instance == obj->__Instance;
}

int Flood::RenderTarget::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::RenderTarget::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::RenderTarget::__Instance::set(System::IntPtr object)
{
    NativePtr = (::RenderTarget*)object.ToPointer();
}

Flood::Settings Flood::RenderTarget::Settings::get()
{
    auto &__ret = ((::RenderTarget*)NativePtr)->getSettings();
    return Flood::Settings((::Settings*)&__ret);
}

Flood::RenderContext^ Flood::RenderTarget::Context::get()
{
    auto __ret = ((::RenderTarget*)NativePtr)->getContext();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderContext((::RenderContext*)__ret);
}

void Flood::RenderTarget::Context::set(Flood::RenderContext^ value)
{
    auto context = value;
    auto arg0 = (::RenderContext*)context->NativePtr;
    ((::RenderTarget*)NativePtr)->setContext(arg0);
}

System::Collections::Generic::List<Flood::RenderView^>^ Flood::RenderTarget::Views::get()
{
    auto &__ret = ((::RenderTarget*)NativePtr)->getViews();
    auto _tmp__ret = gcnew System::Collections::Generic::List<Flood::RenderView^>();
    for(auto _element : __ret)
    {
        auto _marshalElement = gcnew Flood::RenderView((::RenderView*)_element);
        _tmp__ret->Add(_marshalElement);
    }
    return _tmp__ret;
}

void* Flood::RenderTarget::UserData::get()
{
    auto __ret = ((::RenderTarget*)NativePtr)->getUserData();
    return __ret;
}

void Flood::RenderTarget::UserData::set(void* value)
{
    auto v = value;
    auto arg0 = (void*)v;
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

