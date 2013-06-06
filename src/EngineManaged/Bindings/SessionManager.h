/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Network/SessionManager.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class Session;
    ref class SessionManager;

    public ref class SessionManager
    {
    public:
        property ::SessionManager* NativePtr;

        SessionManager(::SessionManager* native);
        SessionManager(System::IntPtr native);
        SessionManager();
    private:
        delegate void _SessionAddedDelegate(const ::SessionPtr& _3);
        _SessionAddedDelegate^ _SessionAddedDelegateInstance;
        void _SessionAddedRaise(const ::SessionPtr& _3);
        System::Action<Flood::Session^>^ _SessionAdded;
    public:
        event System::Action<Flood::Session^>^ SessionAdded
        {
            void add(System::Action<Flood::Session^>^ evt);
            void remove(System::Action<Flood::Session^>^ evt);
            void raise(Flood::Session^ _3);
        }
    private:
        delegate void _SessionRemovedDelegate(const ::SessionPtr& _4);
        _SessionRemovedDelegate^ _SessionRemovedDelegateInstance;
        void _SessionRemovedRaise(const ::SessionPtr& _4);
        System::Action<Flood::Session^>^ _SessionRemoved;
    public:
        event System::Action<Flood::Session^>^ SessionRemoved
        {
            void add(System::Action<Flood::Session^>^ evt);
            void remove(System::Action<Flood::Session^>^ evt);
            void raise(Flood::Session^ _4);
        }
        void AddSession(Flood::Session^ session);
        void RemoveSession(Flood::Session^ session);
    };
}
