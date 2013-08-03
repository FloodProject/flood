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
        Task<string> Ping();

        Task<Message> GetMessage();
    }

    [Message]
    public class Message
    {
        [Id(0)]
        public int i;

        [Id(1)]
        public string s;
    }

    public class Service : IService
    {
        public async Task<string> Ping()
        {
            return "pong"; 
        }

        public async Task<Message> GetMessage()
        {
            return new Message()
            {
                i = 1337,
                s = "leet"
            };
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
            var implType = Assembly.GetExecutingAssembly().GetType(typeof(IService).FullName+"Impl");
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

            var proxySerializer = new BinarySerializer(proxyTransport);
            var implSerializer = new BinarySerializer(implTransport);

            var serviceImpl = new Service();
            var serviceProxy = (IService) clientCttr.Invoke(new object[]{proxySerializer});
            var processor = (Processor) processorCttr.Invoke(new object[]{serviceImpl});

            var thread = new Thread(new ParameterizedThreadStart(Process));
            var processInfo = new ProcessInfo(){Processor=processor, Serializer=implSerializer};
            thread.Start(processInfo);
             
            var pingTask = serviceProxy.Ping();
            Assert.IsTrue(pingTask.Wait(1000));
            Assert.AreEqual("pong", pingTask.Result);
            
            var messageTask = serviceProxy.GetMessage();
            Assert.IsTrue(messageTask.Wait(1000));
            Assert.AreEqual(1337, messageTask.Result.i);
            Assert.AreEqual("leet", messageTask.Result.s);

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
