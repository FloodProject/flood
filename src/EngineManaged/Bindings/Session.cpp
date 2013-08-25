/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Session.h"
#include "Packet.h"
#include "Peer.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Session::Session(::Session* native)
{
    NativePtr = native;
}

Flood::Session::Session(System::IntPtr native)
{
    auto __native = (::Session*)native.ToPointer();
    NativePtr = __native;
}

Flood::Session::Session()
{
    NativePtr = new ::Session();
}

bool Flood::Session::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Session^>(object);

    if (!obj) return false;
    auto param = (::Session*)obj->NativePtr;
    return ((::Session*)NativePtr)->isEquals(param);
}

int Flood::Session::GetHashCode()
{
    return ((::Session*)NativePtr)->hashCode();
}

System::IntPtr Flood::Session::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Session::Instance::set(System::IntPtr object)
{
    NativePtr = (::Session*)object.ToPointer();
}

Flood::SessionState Flood::Session::State::get()
{
    auto __ret = ((::Session*)NativePtr)->getState();
    return (Flood::SessionState)__ret;
}

Flood::Peer^ Flood::Session::Peer::get()
{
    auto __ret = ((::Session*)NativePtr)->getPeer();
    return gcnew Flood::Peer((::Peer*)__ret);
}

void Flood::Session::StateChange::add(System::Action<Flood::SessionState>^ evt)
{
    if (!_StateChangeDelegateInstance)
    {
        _StateChangeDelegateInstance = gcnew _StateChangeDelegate(this, &Flood::Session::_StateChangeRaise);
        auto _fptr = (void (*)(::SessionState))Marshal::GetFunctionPointerForDelegate(_StateChangeDelegateInstance).ToPointer();
        ((::Session*)NativePtr)->onStateChange.Connect(_fptr);
    }
    _StateChange = static_cast<System::Action<Flood::SessionState>^>(System::Delegate::Combine(_StateChange, evt));
}

void Flood::Session::StateChange::remove(System::Action<Flood::SessionState>^ evt)
{
    _StateChange = static_cast<System::Action<Flood::SessionState>^>(System::Delegate::Remove(_StateChange, evt));
}

void Flood::Session::StateChange::raise(Flood::SessionState _0)
{
    _StateChange(_0);
}

void Flood::Session::_StateChangeRaise(::SessionState _0)
{
    StateChange::raise((Flood::SessionState)_0);
}

void Flood::Session::Packet::add(System::Action<Flood::Packet^, int>^ evt)
{
    if (!_PacketDelegateInstance)
    {
        _PacketDelegateInstance = gcnew _PacketDelegate(this, &Flood::Session::_PacketRaise);
        auto _fptr = (void (*)(const ::PacketPtr&, int))Marshal::GetFunctionPointerForDelegate(_PacketDelegateInstance).ToPointer();
        ((::Session*)NativePtr)->onPacket.Connect(_fptr);
    }
    _Packet = static_cast<System::Action<Flood::Packet^, int>^>(System::Delegate::Combine(_Packet, evt));
}

void Flood::Session::Packet::remove(System::Action<Flood::Packet^, int>^ evt)
{
    _Packet = static_cast<System::Action<Flood::Packet^, int>^>(System::Delegate::Remove(_Packet, evt));
}

void Flood::Session::Packet::raise(Flood::Packet^ _1, int _0)
{
    _Packet(_1, _0);
}

void Flood::Session::_PacketRaise(const ::PacketPtr& _1, int _0)
{
    Packet::raise(gcnew Flood::Packet((::Packet*)_1.get()), _0);
}

