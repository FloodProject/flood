
using System.Collections.Generic;
using System.Threading.Tasks;
using Flood.RPC.Serialization;
using System;

namespace Flood.RPC
{

    public abstract class RPCImpl : RPCStub
    {
        protected delegate Task DProcessCall(RPCData request, ProcedureCall call);
        protected delegate void DProcessSubscribe(RPCPeer peer, int implId, int proxyId, int delegateId);

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

        public void ProcessCall(RPCData request)
        {
            ProcedureCall msg = request.Serializer.ReadProcedureCallBegin();
            var call = processors[msg.Id].Call;
            if (call != null)
            {
                call(request, msg);
            }
            else
            {
                SerializerUtil.Skip(request.Serializer, TType.DataObject);
                var response = RPCData.Create(request, RPCDataType.Exception);
                RPCException x = new RPCException(RPCException.ExceptionType.UnknownMethod, "Invalid method id: '" + msg.Id + "'");
                response.Serializer.WriteProcedureCallBegin(msg);
                x.Write(response.Serializer);
                response.Serializer.WriteProcedureCallEnd();
                response.Dispatch();
            }

            request.Serializer.ReadProcedureCallEnd();
        }

        internal void ProcessEventSubscribe(RPCData data)
        {
            var eventId = data.Serializer.ReadI32();
            var delegateId = data.Serializer.ReadI32();

            var subscribe = processors[eventId].Subscribe;
            if (subscribe == null)
                throw new Exception();

            subscribe(data.Peer, data.Header.ImplId, data.Header.ProxyId, delegateId);
        }

        internal void ProcessEventUnsubscribe(RPCData data)
        {
            throw new System.NotImplementedException();
        }
    }
}
