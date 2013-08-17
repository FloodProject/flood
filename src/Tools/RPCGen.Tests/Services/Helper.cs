using Flood;
using Flood.RPC;
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

            public override void DispatchCall(RPCData data)
            {
                data.Serializer.Buffer.Position = 0;
                data.Header.Read();
                manager.Process(data);
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
        public static T GetProxy<T>(T serviceImpl)
        {
            var serviceManager = new RPCManager();
            var serviceId = serviceManager.AddImplementation<T>(serviceImpl);

            return serviceManager.CreateProxy<T>(new MockRPCPeer(serviceManager), serviceId);
        }
    }
}
