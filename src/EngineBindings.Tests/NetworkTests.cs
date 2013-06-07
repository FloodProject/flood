using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using Flood;
using NUnit.Framework;

namespace EngineBindings.Tests
{
    [TestFixture]
    public class NetworkTests
    {
        [Test]
        public void Test()
        {
            FloodNetwork.NetworkInitialize();

            var conn = new HostConnectionDetails();
            conn.Address = "";
            conn.Port = 13131;

            var hostServer = new HostServer();
            var hostClient = new HostClient();

            hostServer.CreateSocket(conn);
            hostClient.Connect(conn);

            //Test Connections
            {
                bool clientConnected = false;
                bool serverConnected = false;
                hostClient.ClientConnected += peer => clientConnected = true;
                hostServer.ClientConnected += peer => serverConnected = true;

                var watch = Stopwatch.StartNew();
                while (watch.ElapsedMilliseconds < 20)
                {
                    hostClient.ProcessEvents(1);
                    hostServer.ProcessEvents(1);
                }

                Assert.IsTrue(clientConnected);
                Assert.IsTrue(serverConnected);
            }

            //Test QueuePacket
            {
                const ushort packetId = 13;
                const string packetString = "message";

                var packetReceived = false;
                var packetReceivedId = -1;
                var packetReceiveString = "";

                hostServer.ClientPacket += (peer, packet) =>
                                           {
                                               packetReceived = true;
                                               packetReceivedId = packet.GetId();
                                               var bytes = packet.Read();
                                               packetReceiveString = Encoding.UTF8.GetString(bytes.ToArray(),0,bytes.Count);
                                           };

                var p = new Packet(packetId);
                p.Write(new List<byte>(Encoding.ASCII.GetBytes(packetString)));

                hostClient.GetPeer().QueuePacket(p, 1);

                var watch = Stopwatch.StartNew();
                while (watch.ElapsedMilliseconds < 20)
                {
                    hostClient.ProcessEvents(1);
                    hostServer.ProcessEvents(1);
                }

                Assert.IsTrue(packetReceived);
                Assert.AreEqual(packetId,packetReceivedId);
                Assert.AreEqual(packetString,packetReceiveString);
            }
        }
    }
}
