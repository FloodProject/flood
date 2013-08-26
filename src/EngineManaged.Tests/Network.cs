using System;
using System.Threading.Tasks;
using Flood.Network;
using NUnit.Framework;
using Flood.RPC.Metadata;
using System.Threading;

namespace Flood.Tests
{
    [Service]
    public interface IServiceTest
    {
        [Id(1)]
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

            var endPoint = new HostEndPoint("localhost", ServerPort);

            // Start the server and setup our service.
            var server = new Server(endPoint);
            var serverThread = new Thread(Process);
            serverThread.Start(server);

            // Start the client and connect to the service.
            var client = new Client();
            var clientThread = new Thread(Process);
            clientThread.Start(client);

            var connectTask = client.Connect(endPoint);
            connectTask.Wait();
            Assert.IsTrue(connectTask.Result);

            var service = new ServiceTest();
            var serviceImpl = server.ServiceManager.GetCreateImplementation<IServiceTest>(service);

            var serviceProxy = client.ServiceManager.GetCreateProxy<IServiceTest>(new SessionRPCPeer(client.Session), serviceImpl.Id);
            var pingTask = serviceProxy.Ping();

            Assert.IsTrue(pingTask.Wait(1000));
            Assert.AreEqual("Pong", pingTask.Result);

            serverThread.Abort();
            clientThread.Abort();
         }

        private void Process(object peer)
        {
            var host = (Flood.Network.Host) peer;

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
