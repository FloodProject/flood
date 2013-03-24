/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/RenderTarget.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class Event1;
    ref class RefPtr;
    ref class RenderContext;
    ref class RenderTarget;
    ref class RenderView;
    value struct Settings;
    value struct Vector2i;

    public value struct Settings
    {
    public:
        Settings(::Settings* native);
        Settings(System::IntPtr native);
        Settings(unsigned short width, unsigned short height);
        unsigned short Width;
        unsigned short Height;
        Flood::Vector2i GetSize();
    };

    public ref class RenderTarget
    {
    public:
        property ::RenderTarget* NativePtr;

        RenderTarget(::RenderTarget* native);
        RenderTarget(System::IntPtr native);
        RenderTarget();
    private:
        delegate void _TargetResizeDelegate(const ::Settings& _0);
        _TargetResizeDelegate^ _TargetResizeDelegateInstance;
        void _TargetResizeRaise(const ::Settings& _0);
        System::Action<Flood::Settings>^ _TargetResize;
    public:
        event System::Action<Flood::Settings>^ TargetResize
        {
            void add(System::Action<Flood::Settings>^ evt);
            void remove(System::Action<Flood::Settings>^ evt);
            void raise(Flood::Settings);
        }
        Flood::RenderView^ CreateView();
        void RemoveViews();
        void MakeCurrent();
        void Update();
        Flood::Settings GetSettings();
        Flood::RenderContext^ GetContext();
        void SetContext(Flood::RenderContext^ context);
        System::Collections::Generic::List<Flood::RenderView^>^ GetViews();
        System::IntPtr GetUserData();
        void SetUserData(System::IntPtr v);
    };
}
