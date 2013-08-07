using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.RPC;
using Flood;
using Flood.RPC.Serialization;

namespace EngineManaged.Network
{
    public class Host
    {
        protected Flood.Host host;

        public ServiceManager ServiceManager { get; private set; }
        
        public Host()
        {
            ServiceManager = new ServiceManager();
        }

        protected void OnPacket(Session session, Packet packet, int channel)
        {
            if (packet.Id != 1)
                return;

            var rpcData = RPCDataHelper.CreateRPCData(packet);
            rpcData.Session = session;
            ServiceManager.Process(rpcData);
        }

        public void Update()
        {
            if (host == null)
                return;

            host.ProcessEvents(0);

            if(SendData(1))
                host.ProcessEvents(0);
        }

        private bool SendData(byte channel)
        {
            var hasDataToSend = ServiceManager.Data.Count>0;

            while (ServiceManager.Data.Count != 0)
            {
                var data = ServiceManager.Data.Dequeue(); 
                var packet = RPCDataHelper.CreatePacket(data, 1);
                data.Session.Peer.QueuePacket(packet, channel);
            }

            return hasDataToSend;
        }

    }

    public class Server : Host
    {
        public Server(ushort port)
        {
            FloodNetwork.NetworkInitialize();

            var conn = new HostConnectionDetails("", port, 2);

            var server = new HostServer();
            host = server;

            server.CreateSocket(conn);

            server.SessionPacket += OnPacket;
        }
    }

    public class Client : Host
    {

        public async Task<bool> Connect(string address, ushort port, int timeout = 1000)
        {
            FloodNetwork.NetworkInitialize();
            var conn = new HostConnectionDetails(address, port, 2);

            var client = new HostClient();
            host = client;

            var tcs = new TaskCompletionSource<bool>();

            var session = client.Session;
            session.StateChange += state =>
                                       {
                                           if (state == SessionState.Open)
                                               tcs.SetResult(true);
                                       };

            client.Connect(conn);

            // wait for task somewhere else
            if (await Task.WhenAny(tcs.Task, Task.Delay(timeout)) != tcs.Task)
                return false;

            client.SessionPacket += OnPacket;

            Console.WriteLine("Client connected with session!");

            return true;
        }

        public Session Session
        {
            get { return ((HostClient) host).Session; }
        }
    }
}
