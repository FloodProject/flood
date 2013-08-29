using Flood;
using Flood.RPC;
using Flood.RPC.Serialization;
using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace RPCGen.Tests.Services
{

    public class Helper
    {
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

        /// Gets a service interface proxy for a given implementation
        public static T GetProxy<T>(T service)
        {
            var localPeer = new MockRPCPeer(null, 1);
            var serviceManager = new RPCManager(localPeer);
            localPeer.Manager = serviceManager;
            
            var remotePeer = new MockRPCPeer(serviceManager, 2);
            
            var serviceImpl = serviceManager.GetCreateImplementation<T>(service);

            return serviceManager.GetService<T>(remotePeer, serviceImpl.Id);
        }
    }
}
