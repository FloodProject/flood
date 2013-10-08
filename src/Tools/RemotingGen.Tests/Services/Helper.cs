using Flood.Remoting;
using Flood.Remoting.Serialization;
using System;
using System.Reflection;
using System.Threading.Tasks;

namespace RemotingGen.Tests.Services
{

    public class Helper
    {
        class MockContextLoader : ContextLoader<MockContextLoader.MockContextId>
        {
            public struct MockContextId : IContextId
            {
                public string AssemblyFullName;

                public void Write(Message data)
                {
                    data.Serializer.WriteString(AssemblyFullName);
                }

                public void Read(Message data)
                {
                    AssemblyFullName = data.Serializer.ReadString();
                }
            }

            public async override Task<Assembly> LoadContext(RemotingPeer peer, IContextId contextId)
            {
                return Assembly.Load(((MockContextId)contextId).AssemblyFullName);
            }

            public override IContextId GetContextId(Assembly assembly)
            {
                return new MockContextId { AssemblyFullName = assembly.FullName };
            }
        }

        public class MockMessagePeer : RemotingPeer
        {
            public MessageProcessor Manager;
            public int Id;

            public MockMessagePeer(MessageProcessor mgr, int id)
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

        public static MessageProcessor CreateMessageProcessor()
        {
            var localPeer = CreatePeer(null);
            var contextLoader = new MockContextLoader();
            return localPeer.Manager = new MessageProcessor(localPeer, contextLoader);
        }

        public static MockMessagePeer CreatePeer(MessageProcessor messageProcessor)
        {
            return new MockMessagePeer(messageProcessor, peerCounter++);
        }
    }
}
