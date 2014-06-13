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
        public void TestPeerConnection()
        {
            Flood.Network.NetworkInitialize();

            var conn = new HostConnectionDetails {Address = "", Port = 13131};

            var hostServer = new HostServer();
            var hostClient = new HostClient();

            hostServer.CreateSocket(conn);
            hostClient.Connect(conn);

            //Test Connections
            {
                //bool clientConnected = false;
                //bool serverConnected = false;
                //hostClient.PeerConnect += peer => clientConnected = true;
                //hostServer.PeerConnect += peer => serverConnected = true;

                var watch = Stopwatch.StartNew();
                while (watch.ElapsedMilliseconds < 20)
                {
                    hostClient.ProcessEvents(1);
                    hostServer.ProcessEvents(1);
                }

                //Assert.IsTrue(clientConnected);
                //Assert.IsTrue(serverConnected);
            }

            //Test QueuePacket
            {
                const ushort packetId = 13;
                const string packetString = "message";

                var packetReceived = false;
                var packetReceivedId = -1;
                var packetReceiveString = "";

                hostServer.SessionPacket += (session, packet, channel) =>
                                           {
                                               packetReceived = true;
                                               packetReceivedId = packet.Id;
                                               var bytes = packet.Read();
                                               packetReceiveString = Encoding.UTF8.GetString(bytes.ToArray(),0,bytes.Count);
                                           };

                var p = new Packet(packetId);
                p.Write(new List<byte>(Encoding.ASCII.GetBytes(packetString)));

                hostClient.Peer.QueuePacket(p, 0);

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

            //Test Encrypted Packet
            {
                const ushort packetId = 13;
                const string packetString = "message";

                var packetReceived = false;
                var packetReceivedId = -1;
                var packetReceiveString = "";

                hostServer.SessionPacket += (session, packet, channel) =>
                                           {
                                               packetReceived = true;
                                               packetReceivedId = packet.Id;
                                               var bytes = packet.Read();
                                               packetReceiveString = Encoding.UTF8.GetString(bytes.ToArray(),0,bytes.Count);
                                           };

                var p = new Packet(packetId) {Flags = PacketFlags.Encrypted};
                p.Write(new List<byte>(Encoding.ASCII.GetBytes(packetString)));

                hostClient.Peer.QueuePacket(p, 0);

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
