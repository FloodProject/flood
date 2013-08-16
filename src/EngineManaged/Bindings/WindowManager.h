/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Window/WindowManager.h>

namespace Flood
{
    ref class Window;
    ref class WindowManager;
    value struct WindowSettings;

    /// <summary>
    /// Window manager manages windows (Windows baby!).
    /// </summary>
    public ref class WindowManager : ICppInstance
    {
    public:

        property ::WindowManager* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        WindowManager(::WindowManager* native);
        WindowManager(System::IntPtr native);
        WindowManager();

        property System::Collections::Generic::List<Flood::Window^>^ Windows
        {
            System::Collections::Generic::List<Flood::Window^>^ get();
            void set(System::Collections::Generic::List<Flood::Window^>^);
        }
        virtual Flood::Window^ CreateWindow(Flood::WindowSettings settings);

        void DestroyWindows();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
