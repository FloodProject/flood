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
        EventSubscribe = 5,
        EventUnsubscribe = 6,
        EventInvoke = 7
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
        public static RPCData CreateReply(RPCData call, RPCFlags flags = RPCFlags.None)
        {
            return Create(call.Peer, call.Header.ImplId, call.Header.ProxyId, RPCDataType.Reply, flags);
        }

        public static RPCData CreateCall(RPCPeer peer, int implId, int proxyId, RPCFlags flags = RPCFlags.None)
        {
            return Create(peer, implId, proxyId, RPCDataType.Call, flags);
        }

        public static RPCData CreateEventSubscribe(RPCPeer peer, int implId, int proxyId)
        {
            return Create(peer, implId, proxyId, RPCDataType.EventSubscribe);
        }

        public static RPCData CreateException(RPCData call)
        {
            return Create(call.Peer, call.Header.ImplId, call.Header.ProxyId, RPCDataType.Exception);
        }
    }
}
