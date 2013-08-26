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
            RPCManager manager;

            public MockRPCPeer(RPCManager mgr)
            {
                manager = mgr;
            }

            public override void Dispatch(RPCData data)
            {
                data.Serializer.Buffer.Position = 0;
                data.Header.Read();
                manager.Process(data);
            }

            public override Flood.RPC.Serialization.Serializer CreateSerializer()
            {
                return new BinarySerializer();
            }

            public override bool Equals(object other)
            {
                return true;
            }

            public override int GetHashCode()
            {
                return 1337;
            }
        }

        /// Gets a service interface proxy for a given implementation
        public static T GetProxy<T>(T service)
        {
            var serviceManager = new RPCManager();
            var serviceImpl = serviceManager.GetCreateImplementation<T>(service);

            return serviceManager.GetCreateProxy<T>(new MockRPCPeer(serviceManager), serviceImpl.Id);
        }
    }
}
