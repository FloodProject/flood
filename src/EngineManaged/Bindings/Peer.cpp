/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Peer.h"
#include "Host.h"
#include "Packet.h"
#include "ResourceHandle.h"
#include "Session.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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

System::String^ Flood::Peer::GetHostName()
{
    auto ret = ((::Peer*)NativePtr)->getHostName();
    return clix::marshalString<clix::E_UTF8>(ret);
}

System::String^ Flood::Peer::GetHostIP()
{
    auto ret = ((::Peer*)NativePtr)->getHostIP();
    return clix::marshalString<clix::E_UTF8>(ret);
}

void Flood::Peer::QueuePacket(Flood::Packet^ packet, unsigned char channel)
{
    auto arg0 = (::Packet*)packet->NativePtr;
    auto arg1 = (uint8)channel;
    ((::Peer*)NativePtr)->queuePacket(arg0, arg1);
}

Flood::Session^ Flood::Peer::GetSession()
{
    auto ret = ((::Peer*)NativePtr)->getSession();
    return gcnew Flood::Session((::Session*)ret);
}

