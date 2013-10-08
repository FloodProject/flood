using System;
using System.Threading.Tasks;

namespace Flood.Remoting
{
    public abstract class RemotingDelegate
    {
        public int LocalId { get; internal set; }
        public Delegate Delegate { get; internal set; }

        public MessageProcessor MessageProcessor;

        protected RemotingDelegate() { }
    }

    public abstract class RemotingDelegateImpl : RemotingDelegate
    {
        protected RemotingDelegateImpl(){}

        public abstract void Invoke(Message.DelegateCall call);
    }

    public abstract class RemotingDelegateProxy : RemotingDelegate
    {
        public RemotingPeer Peer { get; internal set; }
        public int RemoteId { get; internal set; }

        private CallProcessor callProcessor;

        protected RemotingDelegateProxy() 
        {
            callProcessor = new CallProcessor();
        }

        protected void SetDelegate(Delegate @delegate)
        {
            Delegate = @delegate;
        }

        public Message.DelegateCall CreateCall()
        {
            var callId = callProcessor.GetNextCallId();
            return new Message.DelegateCall(callId, Peer, MessageProcessor, LocalId, RemoteId);
        }

        internal void ProcessReply(Message.DelegateReply reply)
        {
            callProcessor.SetResult(reply.Id, reply.Data);
        }

        public Task<Message> DispatchCall(Message.DelegateCall call)
        {
            return callProcessor.DispatchCall(call.Id, call.Data);
        }
    }

}
