using Flood.Remoting.Serialization;

namespace Flood.Remoting
{
    public enum MessageFlags
    {
        None = 0,
        Encrypted = 1,
        Compressed = 2,
        Signed = 4,
    }

    public enum MessageType
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

    public struct Message
    {
        public struct DataHeader
        {
            Serializer serializer;

            public int RemoteId;

            /// When local id is zero extra data should be serialized so local instance can be retrieved or created.
            public int LocalId;
            public MessageType CallType;

            public DataHeader(Serializer serializer)
                : this()
            {
                this.serializer = serializer;
            }

            public void Read()
            {
                RemoteId = serializer.ReadI32();
                LocalId = serializer.ReadI32();
                CallType = (MessageType)serializer.ReadI32();
            }

            public void Write()
            {
                serializer.WriteI32(LocalId);
                serializer.WriteI32(RemoteId);
                serializer.WriteI32((int)CallType);
            }
        }

        public Serializer Serializer;
        public MessageFlags Flags;
        public RemotingPeer Peer;
        public MessageProcessor MessageProcessor;

        public DataHeader Header;

        public Message(RemotingPeer peer, MessageProcessor messageProcessor, Serializer serializer)
        {
            Serializer = serializer;
            Peer = peer;
            MessageProcessor = messageProcessor;
            Header = new DataHeader(Serializer);
            Flags = MessageFlags.None;
        }

        private Message(RemotingPeer peer, MessageProcessor messageProcessor)
            : this(peer, messageProcessor, peer.CreateSerializer())
        {
        }

        public Message(RemotingPeer peer, MessageProcessor messageProcessor, int localId, int remoteId, MessageType type, MessageFlags flags = MessageFlags.None)
            : this(peer, messageProcessor)
        {
            Flags = flags;
            Header.CallType = type;
            Header.RemoteId = remoteId;
            Header.LocalId = localId;
            Header.Write();
        }

        public Message(Message call, MessageType type, MessageFlags flags = MessageFlags.None)
            : this(call.Peer, call.MessageProcessor ,call.Header.LocalId, call.Header.RemoteId, type, flags)
        {
        }

        public void Dispatch()
        {
            Peer.Dispatch(this);
        }

        public struct Call
        {
            public Message Data;
            public int Id;
            public int MethodId;
            internal GlobalServiceId GlobalServiceId;

            internal Call(int id, int methodId, RemotingPeer peer, MessageProcessor messageProcessor, int localId, int remoteId)
            {
                Id = id;
                MethodId = methodId;
                Data = new Message(peer, messageProcessor, localId, remoteId, MessageType.Call);

                Data.Serializer.WriteI32(id);
                Data.Serializer.WriteI32(methodId);

                if (remoteId == 0)
                {
                    GlobalServiceId = messageProcessor.ServiceManager.GetGlobalServiceId(localId);
                    GlobalServiceId.Write(Data);
                }
                else
                {
                    GlobalServiceId = default(GlobalServiceId);
                }
            }

            internal static Call Create(Message data)
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
            public Message Data;
            public int Id;
            public int MethodId;

            public Reply(Call call)
            {
                Id = call.Id;
                MethodId = call.MethodId;
                Data = new Message(call.Data, MessageType.Reply);
                
                Data.Serializer.WriteI32(Id);
                Data.Serializer.WriteI32(MethodId);
            }

            internal static Reply Create(Message data)
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
            public Message Data;
            public int Id;

            internal DelegateCall(int id, RemotingPeer peer, MessageProcessor messageProcessor, int localId, int remoteId)
            {
                Id = id;
                Data = new Message(peer, messageProcessor, localId, remoteId, MessageType.DelegateCall);
                
                Data.Serializer.WriteI32(id);
            }

            internal static DelegateCall Create(Message data)
            {
                var call = new DelegateCall();
                call.Data = data;
                
                call.Id = data.Serializer.ReadI32();

                return call;
            }
        }

        public struct DelegateReply
        {
            public Message Data;
            public int Id;

            public DelegateReply(DelegateCall call)
            {
                Id = call.Id;

                Data = new Message(call.Data, MessageType.DelegateReply);
                
                Data.Serializer.WriteI32(Id);
            }

            internal static DelegateReply Create(Message data)
            {
                var reply = new DelegateReply();
                reply.Data = data;
                
                reply.Id = data.Serializer.ReadI32();

                return reply;
            }
        }
    }
}
