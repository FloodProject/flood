using System.Diagnostics;
using Flood.Remoting.Serialization;

namespace Flood.Remoting
{
    public class RemotingManager
    {
        public ServiceManager ServiceManager { get; private set; }
        public DelegateManager DelegateManager { get; private set; }
        public ReferenceManager ReferenceManager  { get; private set; }
        public ContextManager ContextManager { get; private set; }

        public RemotingManager(IContextLoader contextLoader)
        {
            ServiceManager = new ServiceManager(this);
            DelegateManager = new DelegateManager(this);
            ReferenceManager = new ReferenceManager(this);
            ContextManager = new ContextManager(this, contextLoader);
        }

        public void Process(byte[] data, RemotingPeer peer)
        {
            var serializer = new BinarySerializer();
            serializer.Buffer.Write(data, 0, data.Length);
            serializer.Buffer.Position = 0;

            var message = new Message(peer, this, serializer);
            message.Header.Read();

            Process(message);
        }

        public void Process(Message data)
        {
            switch (data.Header.CallType)
            {
                case MessageType.Call:
                case MessageType.Reply:
                case MessageType.EventSubscribe:
                case MessageType.EventUnsubscribe:
                    ServiceManager.Process(data);
                    return;
                case MessageType.DelegateReply:
                case MessageType.DelegateCall:
                    DelegateManager.Process(data);
                    return;
                case MessageType.ReferenceChanges:
                case MessageType.ReferenceSubscribe:
                case MessageType.ReferenceUnsubscribe:
                    ReferenceManager.Process(data);
                    return;
                case MessageType.ContextRequest:
                case MessageType.ContextResponse:
                    ContextManager.Process(data);
                    return;
                default:
                    Debug.Assert(false);
                    return;
            }
        }
    }
}
