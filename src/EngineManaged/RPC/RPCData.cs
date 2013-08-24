using Flood.RPC.Serialization;
using System;

namespace Flood.RPC
{
    public enum RPCFlags
    {
        None = 0,
        Encrypted = 1,
        Compressed = 2,
        Signed = 4,
    }

    public enum RPCDataType
    {
        Call = 1,
        Reply = 2,
        Exception = 3,
        Oneway = 4,
        /// Send by Proxy to add a Delegate to an Implementation event.
        EventSubscribe = 5,
        /// Send by Proxy to remove a Delegate from an Implementation event.
        EventUnsubscribe = 6,
        /// Send by Implementation to invoke Proxy's Delegate.
        DelegateCall = 7,
        /// Send by Proxy to Implementation's delegate with a call result.
        DelegateReply = 8,
        /// Send by Implementation when Proxy's Delegate is disposed.
        /// This allows Proxy to remove delegates that will never be invoked.
        DelegateDispose = 9,
    }

    public struct RPCData
    {
        public struct RPCDataHeader
        {
            Serializer serializer;

            public int ImplId;
            public int ProxyId;
            public RPCDataType CallType;

            public RPCDataHeader(Serializer serializer)
                : this()
            {
                this.serializer = serializer;
            }

            public void Read()
            {
                ImplId = serializer.ReadI32();
                ProxyId = serializer.ReadI32();
                CallType = (RPCDataType)serializer.ReadI32();
            }

            public void Write()
            {
                serializer.WriteI32(ImplId);
                serializer.WriteI32(ProxyId);
                serializer.WriteI32((int)CallType);
            }
        }

        public Serializer Serializer;
        public RPCFlags Flags;
        public RPCPeer Peer;

        public RPCDataHeader Header;

        public RPCData(RPCPeer peer)
            : this(peer, peer.CreateSerializer())
        {
        }

        public RPCData(RPCPeer peer, Serializer serializer)
            : this()
        {
            Serializer = serializer;
            Peer = peer;
            Header = new RPCDataHeader(Serializer);
        }

        public void Dispatch()
        {
            Peer.Dispatch(this);
        }

        public static RPCData Create(RPCPeer peer, int implId, int proxyId, RPCDataType type, RPCFlags flags = RPCFlags.None)
        {
            var data = new RPCData(peer);
            data.Peer = peer;
            data.Flags = flags;
            data.Header.CallType = type;
            data.Header.ImplId = implId;
            data.Header.ProxyId = proxyId;
            data.Header.Write();
            
            return data;
        }

        public static RPCData Create(RPCData call, RPCDataType type, RPCFlags flags = RPCFlags.None)
        {
            return Create(call.Peer, call.Header.ImplId, call.Header.ProxyId, type, flags);
        }

        public static RPCData Create(RPCProxy proxy, RPCDataType type, RPCFlags flags = RPCFlags.None)
        {
            return Create(proxy.Peer, proxy.ImplId, proxy.ProxyId, type, flags);
        }
    }
}
