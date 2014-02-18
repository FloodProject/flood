/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Network/Session.h>

namespace Flood
{
    enum struct SessionState;
    ref class Packet;
    ref class Peer;
    ref class Session;
}

namespace Flood
{
    /// <summary>
    /// Enumerates all the states a network session can be.
    /// </summary>
    public enum struct SessionState
    {
        Closed = 0,
        Open = 1
    };

    /// <summary>
    /// Sessions are higher-level than peers. They allow for example for peers to
    /// reconnect to their game even if their connection drops. Sessions will be
    /// usually terminated when a timeout expires or if the peer explicitly
    /// terminates their connection.
    /// </summary>
    public ref class Session : ICppInstance
    {
    public:

        property ::Session* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Session(::Session* native);
        Session(System::IntPtr native);
        Session();

        property Flood::SessionState State
        {
            Flood::SessionState get();
        }

        property Flood::Peer^ Peer
        {
            Flood::Peer^ get();
        }

    private:
        delegate void _StateChangeDelegate(::SessionState _0);
        _StateChangeDelegate^ _StateChangeDelegateInstance;
        void _StateChangeRaise(::SessionState _0);
        System::Action<Flood::SessionState>^ _StateChange;
    public:
        event System::Action<Flood::SessionState>^ StateChange
        {
            void add(System::Action<Flood::SessionState>^ evt);
            void remove(System::Action<Flood::SessionState>^ evt);
            void raise(Flood::SessionState _0);
        }
    private:
        delegate void _PacketDelegate(const ::PacketPtr& _0, int _1);
        _PacketDelegate^ _PacketDelegateInstance;
        void _PacketRaise(const ::PacketPtr& _0, int _1);
        System::Action<Flood::Packet^, int>^ _Packet;
    public:
        event System::Action<Flood::Packet^, int>^ Packet
        {
            void add(System::Action<Flood::Packet^, int>^ evt);
            void remove(System::Action<Flood::Packet^, int>^ evt);
            void raise(Flood::Packet^ _0, int _1);
        }
        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
