using System;
using System.Collections.Generic;
using System.Threading;

namespace Flood.Remoting
{
    public class DelegateManager
    {
        protected Dictionary<int, RemotingDelegate> delegates;
        private int delegateIdCounter;

        public RemotingManager remotingManager  { get; private set; }

        public DelegateManager(RemotingManager remotingManager)
        {
            delegates = new Dictionary<int, RemotingDelegate>();

            this.remotingManager = remotingManager;
        }

        internal void Process(Message data)
        {
            switch(data.Header.CallType)
            {
                case MessageType.DelegateReply:
                    ProcessDelegateReply(Message.DelegateReply.Create(data));
                    return;
                case MessageType.DelegateCall:
                    ProcessDelegateCall(Message.DelegateCall.Create(data));
                    return;
            }
        }

        private void ProcessDelegateCall(Message.DelegateCall call)
        {
            var localId = call.Data.Header.LocalId;
            var delegateImpl = (RemotingDelegateImpl)GetDelegate(localId);
            delegateImpl.Invoke(call);
        }

        private void ProcessDelegateReply(Message.DelegateReply reply)
        {
            var localId = reply.Data.Header.LocalId;
            var delegateProxy = (RemotingDelegateProxy)GetDelegate(localId);
            delegateProxy.ProcessReply(reply);
        }

        public RemotingDelegate CreateDelegateProxy<T>(RemotingPeer peer, int remoteId)
            where T : RemotingDelegateProxy, new()
        {
            var delegateProxy = Activator.CreateInstance<T>();
            delegateProxy.Peer = peer;
            delegateProxy.LocalId = Interlocked.Increment(ref delegateIdCounter);
            delegateProxy.RemoteId = remoteId;
            delegateProxy.remotingManager = remotingManager;

            delegates.Add(delegateProxy.LocalId, delegateProxy);

            return delegateProxy;
        }

        public RemotingDelegate CreateDelegateImpl<T>(Delegate del)
            where T : RemotingDelegateImpl, new()
        {
            var delegateImpl = Activator.CreateInstance<T>();
            delegateImpl.LocalId = Interlocked.Increment(ref delegateIdCounter);
            delegateImpl.Delegate = del;
            delegateImpl.remotingManager = remotingManager;

            delegates.Add(delegateImpl.LocalId, delegateImpl);

            return delegateImpl;
        }

        public RemotingDelegate GetDelegate(int delegateId)
        {
            if (!delegates.ContainsKey(delegateId))
                throw new Exception("Unexpected delegate id.");

            return delegates[delegateId];
        }
    }
}
