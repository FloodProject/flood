using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public abstract class RPCProxy : RPCStub
    {
        public RPCPeer Peer { get; private set; }
        public int RemoteId { get; private set; }

        private RPCCallProcessor callProcessor;

        /// Maps a Event id to a Delegate id.
        private Dictionary<int, int> eventIdsDelegates;

        protected RPCProxy(RPCPeer peer, int remoteId, int id)
            : base(id)
        {
            Peer = peer;
            RemoteId = remoteId;

            callProcessor = new RPCCallProcessor();

            eventIdsDelegates = new Dictionary<int, int>();
        }

        protected void Subscribe<T>(int eventId, Delegate del)
            where T : RPCDelegateImpl, new()
        {
            var rpcDelegate = CreateDelegateImpl<T>(Peer, RemoteId, del);
            var data = RPCData.Create(this, RPCDataType.EventSubscribe);
            data.Serializer.WriteI32(eventId);
            data.Serializer.WriteI32(rpcDelegate.Id);
            data.Dispatch();

            eventIdsDelegates.Add(eventId, rpcDelegate.Id);
        }

        protected void Unsubscribe(int eventId)
        {
            var delegateId = eventIdsDelegates[eventId];

            var data = RPCData.Create(this, RPCDataType.EventUnsubscribe);
            data.Serializer.WriteI32(eventId);
            data.Serializer.WriteI32(delegateId);
            data.Dispatch();

            delegates.Remove(delegateId);
            eventIdsDelegates.Remove(eventId);
        }

        internal void ProcessReply(RPCData.Reply reply)
        {
            callProcessor.ProcessReply(reply);
        }

        public RPCData.Call CreateCall(int methodId)
        {
            return callProcessor.CreateCall(methodId, Peer, Id, RemoteId, false);
        }

        public Task<RPCData> DispatchCall(RPCData.Call call)
        {
            return callProcessor.DispatchCall(call);
        }
    }
}
