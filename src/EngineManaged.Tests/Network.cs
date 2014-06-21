using System;
using System.Threading.Tasks;
using Flood.Modules;
using Flood.Networking;
using Flood.Remoting;
using NUnit.Framework;
using Flood.Remoting.Metadata;
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

            var moduleManager = new ModuleManager();
            var remotingManager = new RemotingManager(moduleManager);
            moduleManager.Init(remotingManager.ServiceManager);

            var endPoint = new HostEndPoint("localhost", ServerPort);

            // Start the server and setup our service.
            var server = new Server(remotingManager, endPoint);
            var serverThread = new Thread(Process);
            serverThread.Start(server);

            // Start the client and connect to the service.
            var client = new Client(remotingManager);
            var clientThread = new Thread(Process);
            clientThread.Start(client);

            var connectTask = client.Connect(endPoint);
            connectTask.Wait();
            Assert.IsTrue(connectTask.Result);

            var service = new ServiceTest();
            var serviceImpl = server.RemotingManager.ServiceManager.GetCreateImplementation<IServiceTest>(service);

            var serviceProxy = client.RemotingManager.ServiceManager.GetService<IServiceTest>(new SessionRemotingPeer(client.Session), serviceImpl.LocalId);
            var pingTask = serviceProxy.Ping();

            Assert.IsTrue(pingTask.Wait(1000));
            Assert.AreEqual("Pong", pingTask.Result);

            serverThread.Abort();
            clientThread.Abort();
         }

        private void Process(object peer)
        {
            var host = (Flood.Networking.Host) peer;

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
