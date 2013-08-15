﻿using Flood;
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
    public class ServiceManager : IRPCManager
    {
        public Processor Processor { get; set; }

        public Task<RPCData> RemoteProcedureCall(RPCData data)
        {
            return Processor.Process(data);
        }
    }

    public class Helper
    {
        public static T GetProxy<T>(T serviceImpl)
        {
            var implType = Assembly.GetExecutingAssembly().GetType(typeof(T).FullName + "Impl");
            Assert.NotNull(implType);

            var clientType = implType.GetNestedType("Client");
            Assert.NotNull(clientType);

            var processorType = implType.GetNestedType("Processor");
            Assert.NotNull(processorType);

            var serviceManager = new ServiceManager();

            serviceManager.Processor = (Processor)Activator.CreateInstance(processorType, new object[] { serviceImpl });
            return (T)Activator.CreateInstance(clientType, new object[] { serviceManager, new Session(), 0 });
        }
    }
}
