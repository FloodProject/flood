using System;
using Flood.RPC.Server;
using Flood.RPC.Transport;
using Flood.Server;
namespace Flood.Editor.Server
{
    /// <summary>
    /// Represents the editor server.
    /// </summary>
    public class EditorServer : Flood.Server.Server
    {
        public UserManager UserManager { get; set; }

        public EditorServer()
        {
            var handler = new ProjectManager(this);
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