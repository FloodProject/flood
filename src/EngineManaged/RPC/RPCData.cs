﻿using Flood.RPC.Serialization;
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

    public struct RPCData
    {
        public struct RPCDataHeader
        {
            Serializer serializer;

            public int ServiceId;
            public int SequenceNumber;
            public ProcedureCallType CallType;

            public RPCDataHeader(Serializer serializer)
                : this()
            {
                this.serializer = serializer;
            }

            public void Read()
            {
                ServiceId = serializer.ReadI32();
                SequenceNumber = serializer.ReadI32();
                CallType = (ProcedureCallType)serializer.ReadI32();
            }

            public void Write()
            {
                serializer.WriteI32(ServiceId);
                serializer.WriteI32(SequenceNumber);
                serializer.WriteI32((int)CallType);
            }
        }

        public Serializer Serializer;
        public RPCFlags Flags;
        public RPCPeer Peer;

        public RPCDataHeader Header;

        public RPCData(Serializer serializer)
            : this()
        {
            Serializer = serializer;
            Header = new RPCDataHeader(Serializer);
        }

        public static RPCData CreateReply(RPCData call, RPCFlags flags = RPCFlags.None)
        {
            var serializer = new BinarySerializer();
            var reply = new RPCData(serializer);
            reply.Peer = call.Peer;
            reply.Flags = flags;
            reply.Header.SequenceNumber = call.Header.SequenceNumber;
            reply.Header.CallType = ProcedureCallType.Reply;
            reply.Header.ServiceId = call.Header.ServiceId;
            reply.Header.Write();

            return reply;
        }
    }
}
