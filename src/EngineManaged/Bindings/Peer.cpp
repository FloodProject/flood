/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Peer.h"
#include "Host.h"
#include "Packet.h"
#include "Session.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Peer::Peer(::Peer* native)
{
    NativePtr = native;
}

Flood::Peer::Peer(System::IntPtr native)
{
    auto __native = (::Peer*)native.ToPointer();
    NativePtr = __native;
}

Flood::Peer::Peer()
{
    NativePtr = new ::Peer();
}

void Flood::Peer::Disconnect()
{
    ((::Peer*)NativePtr)->disconnect();
}

void Flood::Peer::ForceDisconnect()
{
    ((::Peer*)NativePtr)->forceDisconnect();
}

void Flood::Peer::QueuePacket(Flood::Packet^ packet, unsigned char channel)
{
    auto arg0 = (::Packet*)packet->NativePtr;
    auto arg1 = (::uint8)channel;
    ((::Peer*)NativePtr)->queuePacket(arg0, arg1);
}

bool Flood::Peer::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Peer^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Peer::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Peer::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Peer::Instance::set(System::IntPtr object)
{
    NativePtr = (::Peer*)object.ToPointer();
}

System::String^ Flood::Peer::HostName::get()
{
    auto ret = ((::Peer*)NativePtr)->getHostName();
    return clix::marshalString<clix::E_UTF8>(ret);
}

System::String^ Flood::Peer::HostIP::get()
{
    auto ret = ((::Peer*)NativePtr)->getHostIP();
    return clix::marshalString<clix::E_UTF8>(ret);
}

Flood::PeerState Flood::Peer::State::get()
{
    auto ret = ((::Peer*)NativePtr)->getState();
    return (Flood::PeerState)ret;
}

Flood::Session^ Flood::Peer::Session::get()
{
    auto ret = ((::Peer*)NativePtr)->getSession();
    return gcnew Flood::Session((::Session*)ret);
}

void Flood::Peer::StateChanged::add(System::Action<Flood::PeerState>^ evt)
{
    if (!_StateChangedDelegateInstance)
    {
        _StateChangedDelegateInstance = gcnew _StateChangedDelegate(this, &Flood::Peer::_StateChangedRaise);
        auto _fptr = (void (*)(::PeerState))Marshal::GetFunctionPointerForDelegate(_StateChangedDelegateInstance).ToPointer();
        ((::Peer*)NativePtr)->onStateChanged.Connect(_fptr);
    }
    _StateChanged = static_cast<System::Action<Flood::PeerState>^>(System::Delegate::Combine(_StateChanged, evt));
}

void Flood::Peer::StateChanged::remove(System::Action<Flood::PeerState>^ evt)
{
    _StateChanged = static_cast<System::Action<Flood::PeerState>^>(System::Delegate::Remove(_StateChanged, evt));
}

void Flood::Peer::StateChanged::raise(Flood::PeerState _0)
{
    _StateChanged(_0);
}

void Flood::Peer::_StateChangedRaise(::PeerState _0)
{
    StateChanged::raise((Flood::PeerState)_0);
}

