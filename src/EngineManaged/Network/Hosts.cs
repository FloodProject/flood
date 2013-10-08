using System.Reflection;
using Flood.Remoting;
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
        public struct ContextId : IContextId
        {
            public string Name;
            public int MajorVersion;

            public void Write(Message data)
            {
                data.Serializer.WriteString(Name);
                data.Serializer.WriteI32(MajorVersion);
            }

            public void Read(Message data)
            {
                Name = data.Serializer.ReadString();
                MajorVersion = data.Serializer.ReadI32();
            }
        }

        public override Task<Assembly> LoadContext(RemotingPeer peer, IContextId contextId)
        {
            throw new NotImplementedException();
        }

        public override IContextId GetContextId(Assembly assembly)
        {
            var assemblyName = assembly.GetName();
            return new ContextId { Name = assemblyName.Name, MajorVersion = assemblyName.Version.Major };
        }
    }

    /// <summary>
    /// Hosts represents a node connected to the network.
    /// </summary>
    public class Host
    {
        public const int ChannelCount = 1;

        protected Flood.Host host;

        public RemotingManager ServiceManager { get; private set; }
        
        public Host()
        {
            var contextLoader = new PackageContextLoader();
            ServiceManager = new RemotingManager(contextLoader);
        }

        protected void OnPacket(Session session, Packet packet, int channel)
        {
            var peer = new SessionRemotingPeer(session);

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
