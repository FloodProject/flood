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
    auto _marshal0 = ::WindowSettings();
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

