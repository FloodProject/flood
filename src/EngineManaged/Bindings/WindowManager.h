/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Window/WindowManager.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class Window;
    ref class WindowManager;
    value struct WindowSettings;

    /// <summary>
    /// Window manager manages windows (Windows baby!).
    /// </summary>
    public ref class WindowManager
    {
    public:
        property ::WindowManager* NativePtr;

        WindowManager(::WindowManager* native);
        WindowManager(System::IntPtr native);
        WindowManager();
        Flood::Window^ CreateWindow(Flood::WindowSettings settings);
        void DestroyWindows();
    };
}
