using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public abstract class RPCDelegate
    {
        public int LocalId { get; internal set; }
        public Delegate Delegate { get; internal set; }

        public RPCManager RPCManager;

        protected RPCDelegate() { }
    }

    public abstract class RPCDelegateImpl : RPCDelegate
    {
        protected RPCDelegateImpl(){}

        public abstract void Invoke(RPCData.DelegateCall call);
    }

    public abstract class RPCDelegateProxy : RPCDelegate
    {
        public RPCPeer Peer { get; internal set; }
        public int RemoteId { get; internal set; }

        private RPCCallProcessor callProcessor;

        protected RPCDelegateProxy() 
        {
            callProcessor = new RPCCallProcessor();
        }

        protected void SetDelegate(Delegate @delegate)
        {
            Delegate = @delegate;
        }

        public RPCData.DelegateCall CreateCall()
        {
            var callId = callProcessor.GetNextCallId();
            return new RPCData.DelegateCall(callId, Peer, LocalId, RemoteId);
        }

        internal void ProcessReply(RPCData.DelegateReply reply)
        {
            callProcessor.SetResult(reply.Id, reply.Data);
        }

        public Task<RPCData> DispatchCall(RPCData.DelegateCall call)
        {
            return callProcessor.DispatchCall(call.Id, call.Data);
        }
    }

}
