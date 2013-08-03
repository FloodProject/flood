using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood;

namespace EngineManaged.Network
{
    public class Host
    {
        protected Flood.Host host;

        public LocalServiceManager LocalServiceManager { get; private set; }
        protected Dictionary<Peer,RemoteServiceManager> RemoteServiceManagers { get; private set; }
        
        public Host()
        {
            LocalServiceManager = new LocalServiceManager();
            RemoteServiceManagers = new Dictionary<Peer,RemoteServiceManager>();
        }

        protected void OnPacket(Peer peer, Packet packet, int channel)
        {
            if (peer.Session == null)
                return;

            var sData = new ServiceData()
            {
                Flags = packet.Flags,
                Peer = peer,
                ServiceId = packet.Id,
                Data = packet.Read().ToArray()
            };

            ServiceManager sManager;
            if (channel == 0)
            {
                sManager = LocalServiceManager;
            }
            else
            {
                sManager = RemoteServiceManagers[peer];
            }

            sManager.AddInData(sData);
        }

        public void Update()
        {
            host.ProcessEvents(0);

            var dataSended = Process(LocalServiceManager, 1);
            foreach (var remoteManager in RemoteServiceManagers.Values)
            {
                dataSended |= Process(remoteManager, 0);
            }

            if(dataSended)
                host.ProcessEvents(0);
        }

        private bool Process(ServiceManager sManager, byte channel)
        {
            sManager.Process();

            var dataSended = false;
            ServiceData data = null;
            while ((data = sManager.GetOutData()) != null)
            {
                dataSended = true;
                SendData(data, channel);
            }
            return dataSended;
        }

        private void SendData(ServiceData data, byte channel)
        {
            var packet = new Packet((ushort) data.ServiceId);
            packet.Flags = data.Flags;
            packet.Write(new List<byte>(data.Data));
            
            data.Peer.QueuePacket(packet, channel);
        }
    }

    public class Server : Host
    {
        public Server(ushort port)
        {
            FloodNetwork.NetworkInitialize();

            var conn = new HostConnectionDetails("", port, 2);

            var server = new HostServer();
            server.CreateSocket(conn);

            server.PeerPacket += OnPacket;

            host = server;
        }
    }

    public class Client : Host
    {
        public Client(string address, ushort port)
        {
            FloodNetwork.NetworkInitialize();

            var conn = new HostConnectionDetails(address, port, 2);

            var client = new HostClient();
            client.Connect(conn);

            client.PeerPacket += OnPacket;

            var peer = client.Peer;
            RemoteServiceManagers.Add(peer, new RemoteServiceManager(peer));

            host = client;
        }

        public RemoteServiceManager RemoteServiceManager
        {
            get { 
                var peer = ((HostClient)host).Peer;
                return RemoteServiceManagers[peer]; 
            }
        }
    }
}
