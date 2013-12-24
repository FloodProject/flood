/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Subsystem.h>

namespace Flood
{
    ref class Subsystem;
}

namespace Flood
{
    /// <summary>
    /// Subsystems are managed by the engine. They are updated once in a while (the
    /// subsystem can control the update rate) and are used mostly to implement the
    /// main services provided by the engine.
    /// </summary>
    public ref class Subsystem : ICppInstance
    {
    public:

        property ::Subsystem* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Subsystem(::Subsystem* native);
        Subsystem(System::IntPtr native);
        virtual void Update();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
