using Flood.Remoting.Serialization;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Flood.Remoting
{

    public abstract class ServiceImpl : ServiceStub
    {
        protected delegate Task DProcessCall(Message.Call call);
        protected delegate void DProcessSubscribe(RemotingPeer peer, int remoteDelegateId);

        protected struct Processors
        {
            public DProcessCall Call;
            public DProcessSubscribe Subscribe;
            public DProcessSubscribe Unsubscribe;

            public Processors(DProcessCall call)
                : this()
            {
                Call = call;
            }

            public Processors(DProcessSubscribe subscribe, DProcessSubscribe unsubscribe)
                : this()
            {
                Subscribe = subscribe;
                Unsubscribe = unsubscribe;
            }
        }

        protected Dictionary<int, Processors> processors;

        public object Impl { get; set; }

        public ServiceImpl(object impl, int localId)
            : base(localId)
        {
            Impl = impl;
            processors = new Dictionary<int, Processors>();
        }

        public void ProcessCall(Message.Call call)
        {
            var processCall = processors[call.MethodId].Call;
            if (processCall != null)
            {
                processCall(call);
            }
            else
            {
                SerializerUtil.Skip(call.Data.Serializer, TType.DataObject);
                var response = new Message(call.Data, MessageType.Exception);
                RemotingException x = new RemotingException(RemotingException.ExceptionType.UnknownMethod, "Invalid method id: '" + call.MethodId + "'");
                response.Serializer.WriteI32(call.Id);
                x.Write(response.Serializer);
                response.Dispatch();
            }
        }

        internal void ProcessEventSubscribe(Message data)
        {
            var eventId = data.Serializer.ReadI32();
            var remoteDelegateId = data.Serializer.ReadI32();

            var subscribe = processors[eventId].Subscribe;
            if (subscribe == null)
                throw new Exception();

            subscribe(data.Peer, remoteDelegateId);
        }

        internal void ProcessEventUnsubscribe(Message data)
        {
            throw new System.NotImplementedException();
        }
    }
}
