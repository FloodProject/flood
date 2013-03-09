/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "WindowManager.h"
#include "ResourceHandle.h"
#include "Window.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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
    auto arg0 = ::WindowSettings();
    arg0.title = marshalString<E_UTF8>(settings.Title);
    arg0.fullScreen = settings.FullScreen;
    arg0.handle = settings.Handle.ToPointer();
    arg0.styles = (::WindowStyles)settings.Styles;

    auto ret = ((::WindowManager*)NativePtr)->createWindow(arg0);
    return gcnew Flood::Window((::Window*)ret);
}

void Flood::WindowManager::DestroyWindows()
{
    ((::WindowManager*)NativePtr)->destroyWindows();
}

