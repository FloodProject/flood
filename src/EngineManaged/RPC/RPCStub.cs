using System;
using System.Collections.Generic;
using System.Threading;

namespace Flood.RPC
{
    public class RPCStub
    {
        /// Maps a Delegate id to a Delegate
        protected Dictionary<int, RPCDelegate> delegates;
        private int delegateIdCounter;

        public int Id { get; private set; }

        protected RPCStub(int id)
        {
            Id = id;
            delegates = new Dictionary<int, RPCDelegate>();
        }

        internal void ProcessDelegateCall(RPCData.Call call)
        {
            if (!delegates.ContainsKey(call.MethodId))
            {
                Log.Error("Received unexpected delegate id to be invoked.");
                return;
            }

            delegates[call.MethodId].Invoke(call);
        }

        public RPCDelegate CreateDelegate<T>(RPCPeer peer, int remoteId, Delegate del = null)
            where T : RPCDelegate, new()
        {
            var delegateId = Interlocked.Increment(ref delegateIdCounter);
            var rpcDelegate = Activator.CreateInstance<T>();
            rpcDelegate.Peer = peer;
            rpcDelegate.LocalId = Id;
            rpcDelegate.RemoteId = remoteId;
            rpcDelegate.DelegateId = delegateId;
            rpcDelegate.Stub = this;

            if (del != null)
                rpcDelegate.Delegate = del;

            delegates.Add(delegateId, rpcDelegate);

            return rpcDelegate;
        }

        public RPCDelegate GetDelegate(int delegateId)
        {
            return delegates[delegateId];
        }

        internal void ProcessDelegateReply(RPCData.Reply reply)
        {
            var del = GetDelegate(reply.MethodId);
            del.ProcessReply(reply);
        }
    }
}
