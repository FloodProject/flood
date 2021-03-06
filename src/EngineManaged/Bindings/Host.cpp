/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Host.h"
#include "Packet.h"
#include "Peer.h"
#include "Session.h"
#include "SessionManager.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Host::Host(::Host* native)
{
    NativePtr = native;
}

Flood::Host::Host(System::IntPtr native)
{
    auto __native = (::Host*)native.ToPointer();
    NativePtr = __native;
}

bool Flood::Host::DestroySocket()
{
    auto __ret = ((::Host*)NativePtr)->destroySocket();
    return __ret;
}

void Flood::Host::BroadcastPacket(Flood::Packet^ _0, unsigned char channel)
{
    auto arg0 = (::Packet*)_0->NativePtr;
    auto arg1 = (::uint8)(::uint8_t)channel;
    ((::Host*)NativePtr)->broadcastPacket(arg0, arg1);
}

void Flood::Host::ProcessEvents(unsigned int timeout)
{
    auto arg0 = (::uint32)(::uint32_t)timeout;
    ((::Host*)NativePtr)->processEvents(arg0);
}

bool Flood::Host::HasContext()
{
    auto __ret = ((::Host*)NativePtr)->hasContext();
    return __ret;
}

bool Flood::Host::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Host^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Host::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Host::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Host::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Host*)object.ToPointer();
}
void Flood::Host::SessionPacket::add(System::Action<Flood::Session^, Flood::Packet^, int>^ evt)
{
    if (!_SessionPacketDelegateInstance)
    {
        _SessionPacketDelegateInstance = gcnew _SessionPacketDelegate(this, &Flood::Host::_SessionPacketRaise);
        auto _fptr = (void (*)(::Session*, const ::PacketPtr&, int))Marshal::GetFunctionPointerForDelegate(_SessionPacketDelegateInstance).ToPointer();
        ((::Host*)NativePtr)->onSessionPacket.Connect(_fptr);
    }
    _SessionPacket = static_cast<System::Action<Flood::Session^, Flood::Packet^, int>^>(System::Delegate::Combine(_SessionPacket, evt));
}

void Flood::Host::SessionPacket::remove(System::Action<Flood::Session^, Flood::Packet^, int>^ evt)
{
    _SessionPacket = static_cast<System::Action<Flood::Session^, Flood::Packet^, int>^>(System::Delegate::Remove(_SessionPacket, evt));
}

void Flood::Host::SessionPacket::raise(Flood::Session^ _0, Flood::Packet^ _1, int _2)
{
    _SessionPacket(_0, _1, _2);
}

void Flood::Host::_SessionPacketRaise(::Session* _0, const ::PacketPtr& _1, int _2)
{
    SessionPacket::raise((_0 == nullptr) ? nullptr : gcnew Flood::Session((::Session*)_0), (_1.get() == nullptr) ? nullptr : gcnew Flood::Packet((::Packet*)_1.get()), _2);
}

Flood::HostConnectionDetails::HostConnectionDetails(::HostConnectionDetails* native)
{
    __Address = StringMarshaller::marshalString(native->address);
    __Port = native->port;
    __ChannelCount = native->channelCount;
}

Flood::HostConnectionDetails::HostConnectionDetails(System::IntPtr native)
{
    auto __native = (::HostConnectionDetails*)native.ToPointer();
    __Address = StringMarshaller::marshalString(__native->address);
    __Port = __native->port;
    __ChannelCount = __native->channelCount;
}

Flood::HostConnectionDetails::HostConnectionDetails(System::String^ address, unsigned short port, unsigned char channelCount)
{
    ::HostConnectionDetails _native(StringMarshaller::marshalString(address), (::uint16)(::uint16_t)port, (::uint8)(::uint8_t)channelCount);
    this->Address = StringMarshaller::marshalString(_native.address);
    this->Port = _native.port;
    this->ChannelCount = _native.channelCount;
}

System::String^ Flood::HostConnectionDetails::Address::get()
{
    return __Address;
}

void Flood::HostConnectionDetails::Address::set(System::String^ value)
{
    __Address = value;
}

unsigned short Flood::HostConnectionDetails::Port::get()
{
    return __Port;
}

void Flood::HostConnectionDetails::Port::set(unsigned short value)
{
    __Port = value;
}

unsigned char Flood::HostConnectionDetails::ChannelCount::get()
{
    return __ChannelCount;
}

void Flood::HostConnectionDetails::ChannelCount::set(unsigned char value)
{
    __ChannelCount = value;
}

Flood::HostClient::HostClient(::HostClient* native)
    : Flood::Host((::Host*)native)
{
}

Flood::HostClient::HostClient(System::IntPtr native)
    : Flood::Host(native)
{
    auto __native = (::HostClient*)native.ToPointer();
}

Flood::HostClient::HostClient()
    : Flood::Host((::Host*)nullptr)
{
    NativePtr = new ::HostClient();
}

bool Flood::HostClient::Connect(Flood::HostConnectionDetails _0)
{
    auto _marshal0 = ::HostConnectionDetails();
    _marshal0.address = StringMarshaller::marshalString(_0.Address);
    _marshal0.port = (::uint16)(::uint16_t)_0.Port;
    _marshal0.channelCount = (::uint8)(::uint8_t)_0.ChannelCount;
    auto arg0 = _marshal0;
    auto __ret = ((::HostClient*)NativePtr)->connect(arg0);
    return __ret;
}

bool Flood::HostClient::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<HostClient^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::HostClient::GetHashCode()
{
    return (int)NativePtr;
}

Flood::Peer^ Flood::HostClient::Peer::get()
{
    auto __ret = ((::HostClient*)NativePtr)->getPeer();
    return (__ret.get() == nullptr) ? nullptr : gcnew Flood::Peer((::Peer*)__ret.get());
}

Flood::Session^ Flood::HostClient::Session::get()
{
    auto __ret = ((::HostClient*)NativePtr)->getSession();
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::Session((::Session*)__ret);
}

Flood::HostServer::HostServer(::HostServer* native)
    : Flood::Host((::Host*)native)
{
}

Flood::HostServer::HostServer(System::IntPtr native)
    : Flood::Host(native)
{
    auto __native = (::HostServer*)native.ToPointer();
}

bool Flood::HostServer::CreateSocket(Flood::HostConnectionDetails _0)
{
    auto _marshal0 = ::HostConnectionDetails();
    _marshal0.address = StringMarshaller::marshalString(_0.Address);
    _marshal0.port = (::uint16)(::uint16_t)_0.Port;
    _marshal0.channelCount = (::uint8)(::uint8_t)_0.ChannelCount;
    auto arg0 = _marshal0;
    auto __ret = ((::HostServer*)NativePtr)->createSocket(arg0);
    return __ret;
}

Flood::HostServer::HostServer()
    : Flood::Host((::Host*)nullptr)
{
    NativePtr = new ::HostServer();
}

bool Flood::HostServer::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<HostServer^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::HostServer::GetHashCode()
{
    return (int)NativePtr;
}

System::Collections::Generic::List<Flood::Peer^>^ Flood::HostServer::Peers::get()
{
    auto &__ret = ((::HostServer*)NativePtr)->getPeers();
    auto _tmp__ret = gcnew System::Collections::Generic::List<Flood::Peer^>();
    for(auto _element : __ret)
    {
        auto _marshalElement = (_element.get() == nullptr) ? nullptr : gcnew Flood::Peer((::Peer*)_element.get());
        _tmp__ret->Add(_marshalElement);
    }
    return (System::Collections::Generic::List<Flood::Peer^>^)(_tmp__ret);
}

Flood::SessionManager^ Flood::HostServer::SessionManager::get()
{
    auto &__ret = ((::HostServer*)NativePtr)->getSessionManager();
    return (Flood::SessionManager^)((&__ret == nullptr) ? nullptr : gcnew Flood::SessionManager((::SessionManager*)&__ret));
}

