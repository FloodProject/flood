using System;
using System.Threading.Tasks;
using NUnit.Framework;
using EngineManaged.Network;
using Flood.RPC.Metadata;
using System.Threading;

namespace EngineManaged.Tests
{
    [Service]
    public interface IServiceTest
    {
        Task<string> Ping();
    }

    public class ServiceTest : IServiceTest
    {
        public async Task<string> Ping()
        {
            return "Pong";
        }
    }

    [TestFixture]
    public class Network
    {
        [Test]
        public void TestServiceAsyncCall()
        {
            const ushort ServerPort = 13337;

            // Start the server and setup our service.
            var server = new Server(ServerPort);
            var serverThread = new Thread(Process);
            serverThread.Start(server);

            var serviceImpl = new ServiceTest();
            server.ServiceManager.AddImplementation<IServiceTest>(serviceImpl);

            // Start the client and connect to the service.
            var client = new Client();
            var clientThread = new Thread(Process);
            clientThread.Start(client);

            var connectTask = client.Connect("localhost", ServerPort);
            connectTask.Wait();
            Assert.IsTrue(connectTask.Result);

            var serviceProxy = client.ServiceManager.GetProxy<IServiceTest>(client.Session, 1);
            var pingTask = serviceProxy.Ping();

            Assert.IsTrue(pingTask.Wait(1000));
            Assert.AreEqual("Pong", pingTask.Result);

            serverThread.Abort();
            clientThread.Abort();
         }

        private void Process(object peer)
        {
            var host = (Host) peer;

            while (true)
            {
                try
                {
                    host.Update();
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
