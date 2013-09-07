using Flood.RPC;
using Flood.RPC.Serialization;
using System;
using System.Reflection;
using System.Threading.Tasks;

namespace RPCGen.Tests.Services
{

    public class Helper
    {
        class MockContextLoader : ContextLoader<MockContextLoader.MockContextId>
        {
            public struct MockContextId : IContextId
            {
                public string AssemblyFullName;

                public void Write(RPCData data)
                {
                    data.Serializer.WriteString(AssemblyFullName);
                }

                public void Read(RPCData data)
                {
                    AssemblyFullName = data.Serializer.ReadString();
                }
            }

            public override Task<Assembly> LoadContext(RPCPeer peer, IContextId contextId)
            {
                throw new NotImplementedException();
            }

            public override IContextId GetContextId(Assembly assembly)
            {
                return new MockContextId { AssemblyFullName = assembly.FullName };
            }
        }

        class MockRPCPeer : RPCPeer
        {
            public RPCManager Manager;
            public int Id;

            public MockRPCPeer(RPCManager mgr, int id)
            {
                Manager = mgr;
                Id = id;
            }

            public override void Dispatch(RPCData data)
            {
                data.Serializer.Buffer.Position = 0;
                data.Header.Read();
                Manager.Process(data);
            }

            public override Flood.RPC.Serialization.Serializer CreateSerializer()
            {
                return new BinarySerializer();
            }

            public override bool Equals(object other)
            {
                var peer = other as MockRPCPeer;
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
            var serviceManager = CreateRPCManager();
            var remotePeer = CreatePeer(serviceManager);
            
            var serviceImpl = serviceManager.ServiceManager.GetCreateImplementation(service);

            return serviceManager.GetService<T>(remotePeer, serviceImpl.LocalId);
        }

        public static RPCManager CreateRPCManager()
        {
            var localPeer = CreatePeer(null);
            var contextLoader = new MockContextLoader();
            return localPeer.Manager = new RPCManager(localPeer, contextLoader);
        }

        public static MockRPCPeer CreatePeer(RPCManager rpcManager)
        {
            return new MockRPCPeer(rpcManager, peerCounter++);
        }
    }
}
