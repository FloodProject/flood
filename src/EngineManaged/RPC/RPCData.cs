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

            public int RemoteId;
            public int LocalId;
            public RPCDataType CallType;

            public RPCDataHeader(Serializer serializer)
                : this()
            {
                this.serializer = serializer;
            }

            public void Read()
            {
                RemoteId = serializer.ReadI32();
                LocalId = serializer.ReadI32();
                CallType = (RPCDataType)serializer.ReadI32();
            }

            public void Write()
            {
                serializer.WriteI32(LocalId);
                serializer.WriteI32(RemoteId);
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

        public static RPCData Create(RPCPeer peer, int localId, int remoteId, RPCDataType type, RPCFlags flags = RPCFlags.None)
        {
            var data = new RPCData(peer);
            data.Peer = peer;
            data.Flags = flags;
            data.Header.CallType = type;
            data.Header.RemoteId = remoteId;
            data.Header.LocalId = localId;
            data.Header.Write();
            
            return data;
        }

        public static RPCData Create(RPCData call, RPCDataType type, RPCFlags flags = RPCFlags.None)
        {
            return Create(call.Peer, call.Header.LocalId, call.Header.RemoteId, type, flags);
        }

        public static RPCData Create(RPCProxy proxy, RPCDataType type, RPCFlags flags = RPCFlags.None)
        {
            return Create(proxy.Peer, proxy.Id, proxy.RemoteId, type, flags);
        }

        public struct Call
        {
            public RPCData Data;
            public int Id;
            public int MethodId;

            internal Call(int id, int methodId, RPCPeer peer, int localId, int remoteId)
            {
                Id = id;
                MethodId = methodId;
                Data = RPCData.Create(peer, localId, remoteId, RPCDataType.Call);

                Data.Serializer.WriteI32(id);
                Data.Serializer.WriteI32(methodId);
            }

            internal static Call Create(RPCData data)
            {
                var call = new Call();
                call.Data = data;
                
                call.Id = data.Serializer.ReadI32();
                call.MethodId = data.Serializer.ReadI32();

                return call;
            }
        }

        public struct Reply
        {
            public RPCData Data;
            public int Id;
            public int MethodId;

            public Reply(Call call)
            {
                Id = call.Id;
                MethodId = call.MethodId;
                Data = RPCData.Create(call.Data, RPCDataType.Reply);
                
                Data.Serializer.WriteI32(Id);
                Data.Serializer.WriteI32(MethodId);
            }

            internal static Reply Create(RPCData data)
            {
                var reply = new Reply();
                reply.Data = data;
                
                reply.Id = data.Serializer.ReadI32();
                reply.MethodId = data.Serializer.ReadI32();

                return reply;
            }
        }

        public struct DelegateCall
        {
            public RPCData Data;
            public int Id;
            public int LocalDelegateId;
            public int RemoteDelegateId;

            internal DelegateCall(int id, int localDelegateId, int remoteDelegateId, RPCPeer peer, int localId, int remoteId)
            {
                Id = id;
                LocalDelegateId = localDelegateId;
                RemoteDelegateId = remoteDelegateId;
                Data = RPCData.Create(peer, localId, remoteId, RPCDataType.DelegateCall);
                
                Data.Serializer.WriteI32(Id);
                Data.Serializer.WriteI32(LocalDelegateId);
                Data.Serializer.WriteI32(RemoteDelegateId);
            }

            internal static DelegateCall Create(RPCData data)
            {
                var call = new DelegateCall();
                call.Data = data;
                
                call.Id = data.Serializer.ReadI32();
                call.RemoteDelegateId = data.Serializer.ReadI32();
                call.LocalDelegateId = data.Serializer.ReadI32();

                return call;
            }
        }

        public struct DelegateReply
        {
            public RPCData Data;
            public int Id;
            public int LocalDelegateId;
            public int RemoteDelegateId;

            public DelegateReply(DelegateCall call)
            {
                Id = call.Id;
                LocalDelegateId = call.LocalDelegateId;
                RemoteDelegateId = call.RemoteDelegateId;
                Data = RPCData.Create(call.Data, RPCDataType.DelegateReply);
                
                Data.Serializer.WriteI32(Id);
                Data.Serializer.WriteI32(LocalDelegateId);
                Data.Serializer.WriteI32(RemoteDelegateId);
            }

            internal static DelegateReply Create(RPCData data)
            {
                var reply = new DelegateReply();
                reply.Data = data;
                
                reply.Id = data.Serializer.ReadI32();
                reply.RemoteDelegateId = data.Serializer.ReadI32();
                reply.LocalDelegateId = data.Serializer.ReadI32();

                return reply;
            }
        }
    }
}
