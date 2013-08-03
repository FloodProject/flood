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
    auto arg1 = (uint8)channel;
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

Flood::Session^ Flood::Peer::Session::get()
{
    auto ret = ((::Peer*)NativePtr)->getSession();
    return gcnew Flood::Session((::Session*)ret);
}

