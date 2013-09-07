using System.Reflection;
using Flood.RPC;
using System;
using System.Threading.Tasks;

namespace Flood.Network
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

    public class PackageContextLoader : ContextLoader<PackageContextLoader.ContextId>
    {
        public class ContextId : IContextId
        {
            public void Write(RPCData data)
            {
                throw new NotImplementedException();
            }

            public void Read(RPCData data)
            {
                throw new NotImplementedException();
            }
        }

        public override Task<Assembly> LoadContext(RPCPeer peer, IContextId contextId)
        {
            throw new NotImplementedException();
        }

        public override IContextId GetContextId(Assembly assembly)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Hosts represents a node connected to the network.
    /// </summary>
    public class Host
    {
        public const int ChannelCount = 1;

        protected Flood.Host host;

        public RPCManager ServiceManager { get; private set; }
        
        public Host()
        {
            var localPeer = new SessionRPCPeer(null);
            var contextLoader = new PackageContextLoader();
            ServiceManager = new RPCManager(localPeer, contextLoader);
        }

        protected void OnPacket(Session session, Packet packet, int channel)
        {
            var peer = new SessionRPCPeer(session);

            ServiceManager.Process(packet.Read().ToArray(), peer);
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
        public Server(HostEndPoint endPoint)
        {
            // TODO: remove this from here.
            FloodNetwork.NetworkInitialize();

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
        public async Task<bool> Connect(HostEndPoint endPoint, int timeout = 1000)
        {
            // TODO: remove this from here.
            FloodNetwork.NetworkInitialize();

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
