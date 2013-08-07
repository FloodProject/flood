/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Network/Peer.h>

namespace Flood
{
    enum struct PeerState;
    ref class _ENetPeer;
    ref class Host;
    ref class Packet;
    ref class PacketProcessor;
    ref class PacketProcessors;
    ref class Peer;
    ref class Session;

    public enum struct PeerState
    {
        Disconnected = 0,
        Connected = 1,
        Disconnecting = 2
    };

    public ref class Peer : ICppInstance
    {
    public:
        property ::Peer* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Peer(::Peer* native);
        Peer(System::IntPtr native);
        Peer();
        property System::String^ HostName
        {
            System::String^ get();
        }
        property System::String^ HostIP
        {
            System::String^ get();
        }
        property Flood::PeerState State
        {
            Flood::PeerState get();
        }
        property Flood::Session^ Session
        {
            Flood::Session^ get();
        }
    private:
        delegate void _StateChangedDelegate(::PeerState _0);
        _StateChangedDelegate^ _StateChangedDelegateInstance;
        void _StateChangedRaise(::PeerState _0);
        System::Action<Flood::PeerState>^ _StateChanged;
    public:
        event System::Action<Flood::PeerState>^ StateChanged
        {
            void add(System::Action<Flood::PeerState>^ evt);
            void remove(System::Action<Flood::PeerState>^ evt);
            void raise(Flood::PeerState _0);
        }
        void Disconnect();
        void ForceDisconnect();
        void QueuePacket(Flood::Packet^ packet, unsigned char channel);
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
