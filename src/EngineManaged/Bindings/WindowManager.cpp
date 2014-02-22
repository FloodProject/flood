/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "WindowManager.h"
#include "Menu.h"
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

Flood::Window^ Flood::WindowManager::CreateWindow(Flood::WindowSettings settings)
{
    auto _marshal0 = ::WindowSettings();
    _marshal0.width = (::uint16)(::uint16_t)settings.Width;
    _marshal0.height = (::uint16)(::uint16_t)settings.Height;
    _marshal0.title = clix::marshalString<clix::E_UTF8>(settings.Title);
    _marshal0.handle = (void*)settings.Handle;
    _marshal0.styles = (::WindowStyles)settings.Styles;
    auto arg0 = _marshal0;
    auto __ret = ((::WindowManager*)NativePtr)->createWindow(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Window((::Window*)__ret);
}

Flood::MenuBar^ Flood::WindowManager::CreateMenuBar()
{
    auto __ret = ((::WindowManager*)NativePtr)->createMenuBar();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::MenuBar((::MenuBar*)__ret);
}

Flood::Menu^ Flood::WindowManager::CreateMenu()
{
    auto __ret = ((::WindowManager*)NativePtr)->createMenu();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Menu((::Menu*)__ret);
}

Flood::Window^ Flood::WindowManager::OpenFileDialog(System::String^ wildcard, Flood::FileDialogFlags flags)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(wildcard);
    auto arg1 = (::FileDialogFlags)flags;
    auto __ret = ((::WindowManager*)NativePtr)->openFileDialog(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Window((::Window*)__ret);
}

Flood::Window^ Flood::WindowManager::OpenDirectoryDialog(System::String^ wildcard, Flood::DirectoryDialogFlags flags)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(wildcard);
    auto arg1 = (::DirectoryDialogFlags)flags;
    auto __ret = ((::WindowManager*)NativePtr)->openDirectoryDialog(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Window((::Window*)__ret);
}

Flood::WindowManager::WindowManager()
{
}

bool Flood::WindowManager::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<WindowManager^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::WindowManager::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::WindowManager::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::WindowManager::__Instance::set(System::IntPtr object)
{
    NativePtr = (::WindowManager*)object.ToPointer();
}
