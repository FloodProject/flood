﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
        public void Test()
        {
            var server = new Server(13337);
            var client = new Client();

            var sThread = new Thread(Process);
            var cThread = new Thread(Process);
            sThread.Start(server);
            cThread.Start(client);

            var connectTask = client.Connect("localhost", 13337);
            connectTask.Wait();
            Assert.IsTrue(connectTask.Result);

            var impl = new ServiceTest();
            server.ServiceManager.AddImplementation<IServiceTest>(impl);

            var proxy = client.ServiceManager.GetProxy<IServiceTest>(client.Session, 1);

            var pingTask = proxy.Ping();

            Assert.IsTrue(pingTask.Wait(1000));
            Assert.AreEqual("Pong", pingTask.Result);

            sThread.Abort();
            cThread.Abort();
         }

        private void Process(object peer)
        {
            var p = (Host) peer;
            while (true)
            {
                try
                {
                    p.Update();
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
