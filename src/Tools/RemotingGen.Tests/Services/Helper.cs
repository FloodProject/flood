using Flood.Modules;
using Flood.Remoting;
using Flood.Remoting.Serialization;
using System.Reflection;
using System.Threading.Tasks;

namespace RemotingGen.Tests.Services
{
    public class Helper
    {
        public class MockMessagePeer : RemotingPeer
        {
            public RemotingManager Manager;
            public int Id;

            public MockMessagePeer(RemotingManager mgr, int id)
            {
                Manager = mgr;
                Id = id;
            }

            public override void Dispatch(Message data)
            {
                data.Serializer.Buffer.Position = 0;
                data.Header.Read();
                Manager.Process(data);
            }

            public override Flood.Remoting.Serialization.Serializer CreateSerializer()
            {
                return new BinarySerializer();
            }

            public override bool Equals(object other)
            {
                var peer = other as MockMessagePeer;
                if (peer == null)
                    return true;

                return peer.Id == Id;
            }

            public override int GetHashCode()
            {
                return Id;
            }
        }

        private static int peerCounter = 1;

        /// Gets a service interface proxy for a given implementation
        public static T GetProxy<T>(T service)
        {
            var messageProcessor = CreateMessageProcessor();
            var remotePeer = CreatePeer(messageProcessor);
            
            var serviceImpl = messageProcessor.ServiceManager.GetCreateImplementation(service);

            return messageProcessor.ServiceManager.GetService<T>(remotePeer, serviceImpl.LocalId);
        }

        public static RemotingManager CreateMessageProcessor()
        {
            var contextLoader = new ModuleManager();
            var remotingManager = new RemotingManager(contextLoader);
            contextLoader.Init(remotingManager.ServiceManager);

            return remotingManager;
        }

        public static MockMessagePeer CreatePeer(RemotingManager remotingManager)
        {
            return new MockMessagePeer(remotingManager, peerCounter++);
        }
    }
}
