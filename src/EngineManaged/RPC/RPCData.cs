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
        Oneway = 4
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

        public static RPCData CreateReply(RPCData call, RPCFlags flags = RPCFlags.None)
        {
            var serializer = new BinarySerializer();
            var reply = new RPCData(serializer);
            reply.Peer = call.Peer;
            reply.Flags = flags;
            reply.Header.SequenceNumber = call.Header.SequenceNumber;
            reply.Header.CallType = RPCDataType.Reply;
            reply.Header.ServiceId = call.Header.ServiceId;
            reply.Header.Write();

            return reply;
        }
    }
}
