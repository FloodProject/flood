/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/PlatformManager.h>
#include "Subsystem.h"

namespace Flood
{
    ref class InputManager;
    ref class PlatformManager;
    ref class WindowManager;

    /// <summary>
    /// This class is a platform abstraction layer responsible to provide instances
    /// for platform-dependent engine subsytems like the window and input managers.
    /// Each platform has one subclass that is provided provided to the engine.
    /// </summary>
    public ref class PlatformManager : Flood::Subsystem
    {
    public:

        PlatformManager(::PlatformManager* native);
        PlatformManager(System::IntPtr native);
        PlatformManager();

        property Flood::WindowManager^ WindowManager
        {
            Flood::WindowManager^ get();
        }
        property Flood::InputManager^ InputManager
        {
            Flood::InputManager^ get();
        }
        /// <summary>
        /// Initialization callback.
        /// </summary>
        virtual void Init();

        /// <summary>
        /// Update callback.
        /// </summary>
        virtual void Update() override;

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
