/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/RenderTarget.h>

namespace Flood
{
    ref class RenderContext;
    ref class RenderTarget;
    ref class RenderView;
    value struct Settings;
    value struct Vector2i;
}

namespace Flood
{
    public value struct Settings
    {
    public:

        Settings(::Settings* native);
        Settings(System::IntPtr native);
        Settings(unsigned short width, unsigned short height);

        property Flood::Vector2i Size
        {
            Flood::Vector2i get();
        }

        property unsigned short Width
        {
            unsigned short get();
            void set(unsigned short);
        }

        property unsigned short Height
        {
            unsigned short get();
            void set(unsigned short);
        }

        private:
        unsigned short __Width;
        unsigned short __Height;
    };

    public ref class RenderTarget : ICppInstance
    {
    public:

        property ::RenderTarget* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        RenderTarget(::RenderTarget* native);
        RenderTarget(System::IntPtr native);
        RenderTarget();

        property Flood::Settings Settings
        {
            Flood::Settings get();
        }

        property Flood::RenderContext^ Context
        {
            Flood::RenderContext^ get();
            void set(Flood::RenderContext^);
        }

        property System::Collections::Generic::List<Flood::RenderView^>^ Views
        {
            System::Collections::Generic::List<Flood::RenderView^>^ get();
        }

        property System::IntPtr UserData
        {
            System::IntPtr get();
            void set(System::IntPtr);
        }

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
            void raise(Flood::Settings _0);
        }
        Flood::RenderView^ CreateView();

        void RemoveViews();

        virtual void MakeCurrent();

        virtual void Update();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
