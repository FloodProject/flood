﻿using Flood.Remoting;
using System.Threading.Tasks;

namespace Flood.Networking
{
    /// Represents a network endpoint as an IP address and a port number.
    public struct HostEndPoint
    {
        public string Address { get; private set; }
        public ushort Port { get; private set; }

        public HostEndPoint(string address, ushort port)
            : this()
        {
            Address = address;
            Port = port;
        }
    }

    /// <summary>
    /// Hosts represents a node connected to the network.
    /// </summary>
    public class Host
    {
        public const int ChannelCount = 1;

        protected Flood.Host host;

        public RemotingManager RemotingManager { get; private set; }
        
        public Host(RemotingManager remotingManager)
        {
            RemotingManager = remotingManager;
        }

        protected void OnPacket(Session session, Packet packet, int channel)
        {
            var peer = new SessionRemotingPeer(session);

            RemotingManager.Process(packet.Read().ToArray(), peer);
        }

        public void Update()
        {
            if (host == null)
                return;

            host.ProcessEvents(0);
        }
    }

    /// <summary>
    /// Represents a server host in the network.
    /// </summary>
    public class Server : Host
    {
        public Server(RemotingManager remotingManager, HostEndPoint endPoint)
            : base(remotingManager)
        {
            // TODO: remove this from here
            //Flood.Network.NetworkInitialize();

            var server = new HostServer();
            host = server;

            var details = new HostConnectionDetails()
            {
                Address = endPoint.Address,
                Port = endPoint.Port,
                ChannelCount = ChannelCount
            };

            server.SessionPacket += OnPacket;
            server.CreateSocket(details);
        }
    }

    /// <summary>
    /// Represents a client host in the network.
    /// </summary>
    public class Client : Host
    {
        public Client(RemotingManager remotingManager)
            : base(remotingManager)
        {}

        public async Task<bool> Connect(HostEndPoint endPoint, int timeout = 1000)
        {
            // TODO: remove this from here.
            //Flood.Network.NetworkInitialize();

            var client = new HostClient();
            host = client;

            var tcs = new TaskCompletionSource<bool>();

            var session = client.Session;
            session.StateChange += state =>
                                       {
                                           if (state == SessionState.Open)
                                               tcs.SetResult(true);
                                       };

            var details = new HostConnectionDetails()
            {
                Address = endPoint.Address,
                Port = endPoint.Port,
                ChannelCount = ChannelCount
            };

            client.Connect(details);

            // Wait for task somewhere else
            if (await Task.WhenAny(tcs.Task, Task.Delay(timeout)) != tcs.Task)
                return false;

            client.SessionPacket += OnPacket;

            Log.Info("Client connected with session!");
            return true;
        }

        public Session Session
        {
            get { return ((HostClient) host).Session; }
        }
    }
}
