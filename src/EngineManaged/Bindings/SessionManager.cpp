/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "SessionManager.h"
#include "Session.h"

using namespace System;
using namespace System::Runtime::InteropServices;

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

bool Flood::SessionManager::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<SessionManager^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::SessionManager::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::SessionManager::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::SessionManager::__Instance::set(System::IntPtr object)
{
    NativePtr = (::SessionManager*)object.ToPointer();
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

void Flood::SessionManager::SessionAdded::raise(Flood::Session^ _0)
{
    _SessionAdded(_0);
}

void Flood::SessionManager::_SessionAddedRaise(const ::SessionPtr& _0)
{
    SessionAdded::raise(gcnew Flood::Session((::Session*)_0.get()));
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

void Flood::SessionManager::SessionRemoved::raise(Flood::Session^ _0)
{
    _SessionRemoved(_0);
}

void Flood::SessionManager::_SessionRemovedRaise(const ::SessionPtr& _0)
{
    SessionRemoved::raise(gcnew Flood::Session((::Session*)_0.get()));
}

