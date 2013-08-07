using System;
using Flood;
using Flood.RPC;
using Flood.RPC.Metadata;
using NUnit.Framework;
using System.Reflection;
using System.Threading.Tasks;

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

    public class ServiceManager : IRPCManager
    {
        public Processor Processor { get; set; }

        public Task<RPCData> RemoteProcedureCall(RPCData data)
        {
            return Processor.Process(data);
        }
    }

    public class Tests
    {
        public void Test()
        {
            var implType = Assembly.GetExecutingAssembly().GetType(typeof(IService).FullName+"Impl");
            Assert.NotNull(implType);

            var clientType = implType.GetNestedType("Client");
            Assert.NotNull(clientType);

            var processorType = implType.GetNestedType("Processor");
            Assert.NotNull(processorType);

            var serviceManager = new ServiceManager();

            var serviceImpl = new Service();
            var serviceProxy = (IService) Activator.CreateInstance(clientType, new object[]{serviceManager, new Session(), 0});
            serviceManager.Processor = (Processor) Activator.CreateInstance(processorType, new object[]{serviceImpl});

            var pingTask = serviceProxy.Ping();
            Assert.IsTrue(pingTask.Wait(1000));
            Assert.AreEqual("pong", pingTask.Result);
            
            var messageTask = serviceProxy.GetMessage();
            Assert.IsTrue(messageTask.Wait(1000));
            Assert.AreEqual(1337, messageTask.Result.i);
            Assert.AreEqual("leet", messageTask.Result.s);
        }
    }
}
