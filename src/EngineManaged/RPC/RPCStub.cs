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

        internal void ProcessDelegateCall(RPCData.DelegateCall call)
        {
            if (!delegates.ContainsKey(call.LocalDelegateId))
            {
                Log.Error("Received unexpected delegate id to be invoked.");
                return;
            }

            var delegateImpl = (RPCDelegateImpl)delegates[call.LocalDelegateId];
            delegateImpl.Invoke(call);
        }

        public RPCDelegate CreateDelegateProxy<T>(RPCPeer peer, int remoteId, int remoteDelegateId)
            where T : RPCDelegateProxy, new()
        {
            var delegateId = Interlocked.Increment(ref delegateIdCounter);
            var delegateProxy = Activator.CreateInstance<T>();
            delegateProxy.Peer = peer;
            delegateProxy.LocalId = Id;
            delegateProxy.RemoteId = remoteId;
            delegateProxy.Id = delegateId;
            delegateProxy.RemoteDelegateId = remoteDelegateId;
            delegateProxy.Stub = this;

            delegates.Add(delegateId, delegateProxy);

            return delegateProxy;
        }

        public RPCDelegate CreateDelegateImpl<T>(RPCPeer peer, int remoteId, Delegate del)
            where T : RPCDelegateImpl, new()
        {
            var delegateId = Interlocked.Increment(ref delegateIdCounter);
            var delegateImpl = Activator.CreateInstance<T>();
            delegateImpl.Id = delegateId;
            delegateImpl.Delegate = del;
            delegateImpl.Stub = this;

            delegates.Add(delegateId, delegateImpl);

            return delegateImpl;
        }

        public RPCDelegate GetDelegate(int delegateId)
        {
            return delegates[delegateId];
        }

        internal void ProcessDelegateReply(RPCData.DelegateReply reply)
        {
            var delegateProxy = (RPCDelegateProxy)GetDelegate(reply.LocalDelegateId);
            delegateProxy.ProcessReply(reply);
        }
    }
}
