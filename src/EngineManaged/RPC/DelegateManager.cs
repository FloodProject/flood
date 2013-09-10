using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public class DelegateManager
    {
        protected Dictionary<int, RPCDelegate> delegates;
        private int delegateIdCounter;

        public RPCManager RPCManager  { get; private set; }

        public DelegateManager(RPCManager rpcManager)
        {
            delegates = new Dictionary<int, RPCDelegate>();

            RPCManager = rpcManager;
        }

        internal void Process(RPCData data)
        {
            switch(data.Header.CallType)
            {
                case RPCDataType.DelegateReply:
                    ProcessDelegateReply(RPCData.DelegateReply.Create(data));
                    return;
                case RPCDataType.DelegateCall:
                    ProcessDelegateCall(RPCData.DelegateCall.Create(data));
                    return;
            }
        }

        private void ProcessDelegateCall(RPCData.DelegateCall call)
        {
            var localId = call.Data.Header.LocalId;
            var delegateImpl = (RPCDelegateImpl)GetDelegate(localId);
            delegateImpl.Invoke(call);
        }

        private void ProcessDelegateReply(RPCData.DelegateReply reply)
        {
            var localId = reply.Data.Header.LocalId;
            var delegateProxy = (RPCDelegateProxy)GetDelegate(localId);
            delegateProxy.ProcessReply(reply);
        }

        public RPCDelegate CreateDelegateProxy<T>(RPCPeer peer, int remoteId)
            where T : RPCDelegateProxy, new()
        {
            var delegateProxy = Activator.CreateInstance<T>();
            delegateProxy.Peer = peer;
            delegateProxy.LocalId = Interlocked.Increment(ref delegateIdCounter);
            delegateProxy.RemoteId = remoteId;
            delegateProxy.RPCManager = RPCManager;

            delegates.Add(delegateProxy.LocalId, delegateProxy);

            return delegateProxy;
        }

        public RPCDelegate CreateDelegateImpl<T>(Delegate del)
            where T : RPCDelegateImpl, new()
        {
            var delegateImpl = Activator.CreateInstance<T>();
            delegateImpl.LocalId = Interlocked.Increment(ref delegateIdCounter);
            delegateImpl.Delegate = del;
            delegateImpl.RPCManager = RPCManager;

            delegates.Add(delegateImpl.LocalId, delegateImpl);

            return delegateImpl;
        }

        public RPCDelegate GetDelegate(int delegateId)
        {
            if (!delegates.ContainsKey(delegateId))
                throw new Exception("Unexpected delegate id.");

            return delegates[delegateId];
        }
    }
}
