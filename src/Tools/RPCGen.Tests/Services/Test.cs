using EngineManaged.Network;
using Flood.RPC;
using Flood.RPC.Protocol;
using Flood.RPC.Transport;
using Flood.RPC.Metadata;
using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace RPCGen.Tests.Services
{
    [Service]
    public interface IService
    {
        string Ping();
    }

    public class Service : IService
    {
        public string Ping()
        {
            return "pong"; 
        }
    }

    public class Tests
    {
        struct ProcessInfo
        {
            public Processor Processor;
            public Serializer Serializer;
        }

        public void Test()
        {
            var implType = Assembly.GetExecutingAssembly().GetType("IServiceImpl");
            Assert.NotNull(implType);

            var clientType = implType.GetNestedType("Client");
            Assert.NotNull(clientType);
            var clientCttr = clientType.GetConstructor(new Type[]{typeof(Serializer)});
            Assert.NotNull(clientCttr);

            var processorType = implType.GetNestedType("Processor");
            Assert.NotNull(processorType);
            var processorCttr = processorType.GetConstructor(new Type[]{typeof(IService)});
            Assert.NotNull(processorCttr);

            var stream1 = new ProducerConsumerStream();
            var stream2 = new ProducerConsumerStream();

            var proxyTransport = new TStreamTransport(stream1, stream2);
            var implTransport = new TStreamTransport(stream2, stream1);

            var proxySerializer = new BinaryProtocol(proxyTransport);
            var implSerializer = new BinaryProtocol(implTransport);

            var serviceImpl = new Service();
            var serviceProxy = (IService) clientCttr.Invoke(new object[]{proxySerializer});
            var processor = (Processor) processorCttr.Invoke(new object[]{serviceImpl});

            var thread = new Thread(new ParameterizedThreadStart(Process));
            var processInfo = new ProcessInfo(){Processor=processor, Serializer=implSerializer};
            thread.Start(processInfo);
             
            var result = serviceProxy.Ping();
            Assert.AreEqual("pong", result);

            thread.Abort();
        }

        private void Process(object processInfo)
        {
            var p = (ProcessInfo)processInfo;
            while (true)
            {
                try
                {
                    p.Processor.Process(p.Serializer, p.Serializer);
                    Thread.Sleep(50);
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
            }
        }
    }
}
