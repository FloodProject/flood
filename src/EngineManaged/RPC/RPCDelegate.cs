using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public abstract class RPCDelegate
    {
        public RPCPeer Peer;
        public int RemoteId;
        public int LocalId;
        public int DelegateId;

        public Delegate Delegate;

        public RPCStub Stub;

        private RPCCallProcessor callProcessor;

        public RPCDelegate()
        {
            callProcessor = new RPCCallProcessor();
        }

        public RPCDelegate(RPCPeer peer, int localId, int remoteId, int delegateId)
        {
            Peer = peer;
            RemoteId = remoteId;
            LocalId = localId;
            DelegateId = delegateId;

            callProcessor = new RPCCallProcessor();
        }

        public abstract void Invoke(RPCData.Call call);

        internal void ProcessReply(RPCData.Reply reply)
        {
            callProcessor.ProcessReply(reply);
        }

        public RPCData.Call CreateCall(int methodId)
        {
            return callProcessor.CreateCall(methodId, Peer, LocalId, RemoteId, true);
        }

        public Task<RPCData> DispatchCall(RPCData.Call call)
        {
            return callProcessor.DispatchCall(call);
        }
    }

}
