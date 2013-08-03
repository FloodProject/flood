/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "WindowManager.h"
#include "Window.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::WindowManager::WindowManager(::WindowManager* native)
{
    NativePtr = native;
}

Flood::WindowManager::WindowManager(System::IntPtr native)
{
    auto __native = (::WindowManager*)native.ToPointer();
    NativePtr = __native;
}

Flood::WindowManager::WindowManager()
{
}

Flood::Window^ Flood::WindowManager::CreateWindow(Flood::WindowSettings settings)
{
    auto _marshal0 = ::WindowSettings();
    _marshal0.width = (uint16)settings.Width;
    _marshal0.height = (uint16)settings.Height;
    _marshal0.title = clix::marshalString<clix::E_UTF8>(settings.Title);
    _marshal0.fullScreen = settings.FullScreen;
    _marshal0.handle = settings.Handle.ToPointer();
    _marshal0.styles = (::WindowStyles)settings.Styles;
    auto arg0 = _marshal0;
    auto ret = ((::WindowManager*)NativePtr)->createWindow(arg0);
    return gcnew Flood::Window((::Window*)ret);
}

void Flood::WindowManager::DestroyWindows()
{
    ((::WindowManager*)NativePtr)->destroyWindows();
}

bool Flood::WindowManager::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<WindowManager^>(object);
    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::WindowManager::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::WindowManager::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::WindowManager::Instance::set(System::IntPtr object)
{
    NativePtr = (::WindowManager*)object.ToPointer();
}

System::Collections::Generic::List<Flood::Window^>^ Flood::WindowManager::Windows::get()
{
    auto _tmpWindows = gcnew System::Collections::Generic::List<Flood::Window^>();
    for(auto _element : ((::WindowManager*)NativePtr)->windows)
    {
        auto _marshalElement = gcnew Flood::Window((::Window*)_element);
        _tmpWindows->Add(_marshalElement);
    }
    return _tmpWindows;
}

void Flood::WindowManager::Windows::set(System::Collections::Generic::List<Flood::Window^>^ value)
{
    auto _tmpvalue = std::vector<::Window*>();
    for each(Flood::Window^ _element in value)
    {
        auto _marshalElement = (::Window*)_element->NativePtr;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::WindowManager*)NativePtr)->windows = _tmpvalue;
}

