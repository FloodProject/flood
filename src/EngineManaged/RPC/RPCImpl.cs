
using System.Collections.Generic;
using System.Threading.Tasks;
using Flood.RPC.Serialization;
using System;

namespace Flood.RPC
{

    public abstract class RPCImpl : RPCStub
    {
        protected delegate Task DProcessCall(RPCData.Call call);
        protected delegate void DProcessSubscribe(RPCPeer peer, int remoteId, int remoteDelegateId);

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

        public RPCImpl(object impl, int id)
            : base(id)
        {
            Impl = impl;
            processors = new Dictionary<int, Processors>();
        }

        public void ProcessCall(RPCData.Call call)
        {
            var processCall = processors[call.MethodId].Call;
            if (processCall != null)
            {
                processCall(call);
            }
            else
            {
                SerializerUtil.Skip(call.Data.Serializer, TType.DataObject);
                var response = RPCData.Create(call.Data, RPCDataType.Exception);
                RPCException x = new RPCException(RPCException.ExceptionType.UnknownMethod, "Invalid method id: '" + call.MethodId + "'");
                response.Serializer.WriteI32(call.Id);
                x.Write(response.Serializer);
                response.Dispatch();
            }
        }

        internal void ProcessEventSubscribe(RPCData data)
        {
            var eventId = data.Serializer.ReadI32();
            var remoteDelegateId = data.Serializer.ReadI32();

            var subscribe = processors[eventId].Subscribe;
            if (subscribe == null)
                throw new Exception();

            subscribe(data.Peer, data.Header.RemoteId, remoteDelegateId);
        }

        internal void ProcessEventUnsubscribe(RPCData data)
        {
            throw new System.NotImplementedException();
        }
    }
}
