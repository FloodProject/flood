/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "SessionManager.h"
#include "ResourceHandle.h"
#include "Session.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::SessionManager::SessionManager(::SessionManager* native)
{
    NativePtr = native;
}

Flood::SessionManager::SessionManager(System::IntPtr native)
{
    auto __native = (::SessionManager*)native.ToPointer();
    NativePtr = __native;
}

Flood::SessionManager::SessionManager()
{
    NativePtr = new ::SessionManager();
}

void Flood::SessionManager::AddSession(Flood::Session^ session)
{
    auto arg0 = (::Session*)session->NativePtr;
    ((::SessionManager*)NativePtr)->addSession(arg0);
}

void Flood::SessionManager::RemoveSession(Flood::Session^ session)
{
    auto arg0 = (::Session*)session->NativePtr;
    ((::SessionManager*)NativePtr)->removeSession(arg0);
}

void Flood::SessionManager::SessionAdded::add(System::Action<Flood::Session^>^ evt)
{
    if (!_SessionAddedDelegateInstance)
    {
        _SessionAddedDelegateInstance = gcnew _SessionAddedDelegate(this, &Flood::SessionManager::_SessionAddedRaise);
        auto _fptr = (void (*)(const ::SessionPtr&))Marshal::GetFunctionPointerForDelegate(_SessionAddedDelegateInstance).ToPointer();
        ((::SessionManager*)NativePtr)->onSessionAdded.Connect(_fptr);
    }
    _SessionAdded = static_cast<System::Action<Flood::Session^>^>(System::Delegate::Combine(_SessionAdded, evt));
}

void Flood::SessionManager::SessionAdded::remove(System::Action<Flood::Session^>^ evt)
{
    _SessionAdded = static_cast<System::Action<Flood::Session^>^>(System::Delegate::Remove(_SessionAdded, evt));
}

void Flood::SessionManager::SessionAdded::raise(Flood::Session^ _3)
{
    _SessionAdded(_3);
}

void Flood::SessionManager::_SessionAddedRaise(const ::SessionPtr& _3)
{
    SessionAdded::raise(gcnew Flood::Session((::Session*)_3.get()));
}

void Flood::SessionManager::SessionRemoved::add(System::Action<Flood::Session^>^ evt)
{
    if (!_SessionRemovedDelegateInstance)
    {
        _SessionRemovedDelegateInstance = gcnew _SessionRemovedDelegate(this, &Flood::SessionManager::_SessionRemovedRaise);
        auto _fptr = (void (*)(const ::SessionPtr&))Marshal::GetFunctionPointerForDelegate(_SessionRemovedDelegateInstance).ToPointer();
        ((::SessionManager*)NativePtr)->onSessionRemoved.Connect(_fptr);
    }
    _SessionRemoved = static_cast<System::Action<Flood::Session^>^>(System::Delegate::Combine(_SessionRemoved, evt));
}

void Flood::SessionManager::SessionRemoved::remove(System::Action<Flood::Session^>^ evt)
{
    _SessionRemoved = static_cast<System::Action<Flood::Session^>^>(System::Delegate::Remove(_SessionRemoved, evt));
}

void Flood::SessionManager::SessionRemoved::raise(Flood::Session^ _4)
{
    _SessionRemoved(_4);
}

void Flood::SessionManager::_SessionRemovedRaise(const ::SessionPtr& _4)
{
    SessionRemoved::raise(gcnew Flood::Session((::Session*)_4.get()));
}

