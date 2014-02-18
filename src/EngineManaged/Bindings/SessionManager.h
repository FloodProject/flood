/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Network/SessionManager.h>

namespace Flood
{
    ref class Session;
    ref class SessionManager;
}

namespace Flood
{
    public ref class SessionManager : ICppInstance
    {
    public:

        property ::SessionManager* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        SessionManager(::SessionManager* native);
        SessionManager(System::IntPtr native);
        SessionManager();

    private:
        delegate void _SessionAddedDelegate(const ::SessionPtr& _0);
        _SessionAddedDelegate^ _SessionAddedDelegateInstance;
        void _SessionAddedRaise(const ::SessionPtr& _0);
        System::Action<Flood::Session^>^ _SessionAdded;
    public:
        event System::Action<Flood::Session^>^ SessionAdded
        {
            void add(System::Action<Flood::Session^>^ evt);
            void remove(System::Action<Flood::Session^>^ evt);
            void raise(Flood::Session^ _0);
        }
    private:
        delegate void _SessionRemovedDelegate(const ::SessionPtr& _0);
        _SessionRemovedDelegate^ _SessionRemovedDelegateInstance;
        void _SessionRemovedRaise(const ::SessionPtr& _0);
        System::Action<Flood::Session^>^ _SessionRemoved;
    public:
        event System::Action<Flood::Session^>^ SessionRemoved
        {
            void add(System::Action<Flood::Session^>^ evt);
            void remove(System::Action<Flood::Session^>^ evt);
            void raise(Flood::Session^ _0);
        }
        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
