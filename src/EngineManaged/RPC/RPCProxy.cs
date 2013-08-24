using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public abstract class RPCProxy
    {
        private Dictionary<int, TaskCompletionSource<RPCData>> pendingCalls;

        /// Maps a Event id to a Delegate id.
        private Dictionary<int, int> eventIdsDelegates;

        /// Maps a Delegate id to a Delegate
        private Dictionary<int, RPCDelegate> delegates;

        private int sequenceNumber;
        private int delegateIdCounter;

        public RPCPeer Peer { get; private set; }
        public int ImplId { get; private set; }
        public int ProxyId { get; private set; }

        protected RPCProxy(RPCPeer peer, int implId, int proxyId)
        {
            Peer = peer;
            ImplId = implId;
            ProxyId = proxyId;

            pendingCalls = new Dictionary<int, TaskCompletionSource<RPCData>>();
            delegates = new Dictionary<int, RPCDelegate>();
            eventIdsDelegates = new Dictionary<int, int>();
        }

        protected int GetNextSequenceNumber()
        {
            return Interlocked.Increment(ref sequenceNumber);
        }

        protected int GetNextDelegateId()
        {
            return Interlocked.Increment(ref delegateIdCounter);
        }

        protected Task<RPCData> DispatchCall(RPCData data, int seqNumber)
        {
            var tcs = new TaskCompletionSource<RPCData>();
            pendingCalls.Add(seqNumber, tcs);

            data.Peer.Dispatch(data);

            return tcs.Task;
        }

        protected void Subscribe(RPCDelegate @delegate, int eventId)
        {
            var delegateId = GetNextDelegateId();

            var data = RPCData.Create(this, RPCDataType.EventSubscribe);
            data.Serializer.WriteI32(eventId);
            data.Serializer.WriteI32(delegateId);
            data.Dispatch();

            delegates.Add(delegateId, @delegate);
            eventIdsDelegates.Add(eventId, delegateId);
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

        internal void ProcessReply(RPCData reply)
        {
            var call = reply.Serializer.ReadProcedureCallBegin();
            if (!pendingCalls.ContainsKey(call.SequenceNumber))
            {
                Log.Error("Received unexpected reply.");
                return;
            }

            pendingCalls[call.SequenceNumber].SetResult(reply);
        }

        internal void ProcessDelegateCall(RPCData data)
        {
            var delegateId = data.Serializer.ReadI32();
            if (!delegates.ContainsKey(delegateId))
            {
                Log.Error("Received unexpected delegate id to be invoked.");
                return;
            }

            delegates[delegateId].Invoke(data);
        }

        internal void ProcessDelegateReply(RPCData data)
        {
            // TODO
        }
    }
}
