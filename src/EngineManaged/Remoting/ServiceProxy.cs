using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Flood.Remoting
{
    public abstract class ServiceProxy : ServiceStub
    {
        public RemotingPeer Peer { get; private set; }
        public int RemoteId { get; private set; }

        private CallProcessor callProcessor;

        /// Maps a Event id to a Delegate id.
        private Dictionary<int, int> eventIdsDelegates;

        protected ServiceProxy(RemotingPeer peer, int remoteId, int localId)
            : base(localId)
        {
            Peer = peer;
            RemoteId = remoteId;

            callProcessor = new CallProcessor();

            eventIdsDelegates = new Dictionary<int, int>();
        }

        protected void Subscribe<T>(int eventId, Delegate del)
            where T : RemotingDelegateImpl, new()
        {
            var rpcDelegate = RemotingManager.DelegateManager.CreateDelegateImpl<T>(del);
            var data = new Message(Peer, RemotingManager, LocalId, RemoteId, MessageType.EventSubscribe);
            data.Serializer.WriteI32(eventId);
            data.Serializer.WriteI32(rpcDelegate.LocalId);
            data.Dispatch();

            eventIdsDelegates.Add(eventId, rpcDelegate.LocalId);
        }

        protected void Unsubscribe(int eventId)
        {
            var delegateId = eventIdsDelegates[eventId];

            var data = new Message(Peer, RemotingManager, LocalId, RemoteId, MessageType.EventUnsubscribe);
            data.Serializer.WriteI32(eventId);
            data.Serializer.WriteI32(delegateId);
            data.Dispatch();

            eventIdsDelegates.Remove(eventId);
        }

        public Message.Call CreateCall(int methodId)
        {
            var callId = callProcessor.GetNextCallId();
            return new Message.Call(callId, methodId, Peer, RemotingManager, LocalId, RemoteId);
        }

        internal void ProcessReply(Message.Reply reply)
        {
            callProcessor.SetResult(reply.Id, reply.Data);
        }

        public Task<Message> DispatchCall(Message.Call call)
        {
            return callProcessor.DispatchCall(call.Id, call.Data);
        }
    }
}
