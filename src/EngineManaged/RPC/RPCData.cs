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

        ReferenceChanges = 10,
        ReferenceSubscribe = 11,
        ReferenceUnsubscribe = 12,

        ContextRequest = 13,
        ContextResponse = 14
    }

    public struct RPCData
    {
        public struct RPCDataHeader
        {
            Serializer serializer;

            public int RemoteId;

            /// When local id is zero extra data should be serialized so local instance can be retrieved or created.
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
        public RPCManager RPCManager;

        public RPCDataHeader Header;

        public RPCData(RPCPeer peer, RPCManager rpcManager, Serializer serializer)
        {
            Serializer = serializer;
            Peer = peer;
            RPCManager = rpcManager;
            Header = new RPCDataHeader(Serializer);
            Flags = RPCFlags.None;
        }

        private RPCData(RPCPeer peer, RPCManager rpcManager)
            : this(peer, rpcManager, peer.CreateSerializer())
        {
        }

        public RPCData(RPCPeer peer, RPCManager rpcManager, int localId, int remoteId, RPCDataType type, RPCFlags flags = RPCFlags.None)
            : this(peer, rpcManager)
        {
            Flags = flags;
            Header.CallType = type;
            Header.RemoteId = remoteId;
            Header.LocalId = localId;
            Header.Write();
        }

        public RPCData(RPCData call, RPCDataType type, RPCFlags flags = RPCFlags.None)
            : this(call.Peer, call.RPCManager ,call.Header.LocalId, call.Header.RemoteId, type, flags)
        {
        }

        public void Dispatch()
        {
            Peer.Dispatch(this);
        }

        public struct Call
        {
            public RPCData Data;
            public int Id;
            public int MethodId;
            internal GlobalServiceId GlobalServiceId;

            internal Call(int id, int methodId, RPCPeer peer, RPCManager rpcManager, int localId, int remoteId)
            {
                Id = id;
                MethodId = methodId;
                Data = new RPCData(peer, rpcManager, localId, remoteId, RPCDataType.Call);

                Data.Serializer.WriteI32(id);
                Data.Serializer.WriteI32(methodId);

                if (remoteId == 0)
                {
                    GlobalServiceId = rpcManager.ServiceManager.GetGlobalServiceId(localId);
                    GlobalServiceId.Write(Data);
                }
                else
                {
                    GlobalServiceId = default(GlobalServiceId);
                }
            }

            internal static Call Create(RPCData data)
            {
                var call = new Call();
                call.Data = data;
                
                call.Id = data.Serializer.ReadI32();
                call.MethodId = data.Serializer.ReadI32();

                if (data.Header.LocalId == 0)
                    call.GlobalServiceId = GlobalServiceId.Read(data);

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
                Data = new RPCData(call.Data, RPCDataType.Reply);
                
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

            internal DelegateCall(int id, RPCPeer peer, RPCManager rpcManager, int localId, int remoteId)
            {
                Id = id;
                Data = new RPCData(peer, rpcManager, localId, remoteId, RPCDataType.DelegateCall);
                
                Data.Serializer.WriteI32(id);
            }

            internal static DelegateCall Create(RPCData data)
            {
                var call = new DelegateCall();
                call.Data = data;
                
                call.Id = data.Serializer.ReadI32();

                return call;
            }
        }

        public struct DelegateReply
        {
            public RPCData Data;
            public int Id;

            public DelegateReply(DelegateCall call)
            {
                Id = call.Id;

                Data = new RPCData(call.Data, RPCDataType.DelegateReply);
                
                Data.Serializer.WriteI32(Id);
            }

            internal static DelegateReply Create(RPCData data)
            {
                var reply = new DelegateReply();
                reply.Data = data;
                
                reply.Id = data.Serializer.ReadI32();

                return reply;
            }
        }
    }
}
