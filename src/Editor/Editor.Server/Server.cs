using System;
using Flood.RPC.Server;
using Flood.RPC.Transport;

namespace Flood.Editor
{
    /// <summary>
    /// Represents the editor server.
    /// </summary>
    public class EditorServer : Server.Server
    {
        public UserManager UserManager { get; set; }

        public EditorServer()
        {
            var handler = new ProjectManager();
            var processor = new IProjectManagerImpl.Processor(handler);

            Socket = new TServerSocket(Settings.RPCPort);
            RPCServer = new TSimpleServer(processor, Socket);
        }

        public void Serve()
        {
            Console.WriteLine("Server is starting...");
            RPCServer.Serve();
        }
    }
}