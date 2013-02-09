using System;
using System.Threading;
using System.Threading.Tasks;
using Flood.Editor.Server;

namespace Flood.Editor
{
    public class ServerManager
    {
        public EditorServer Server { get; private set; }
        private ManualResetEventSlim serverCreatedEvent;

        private void RunBuiltinServer()
        {
            serverCreatedEvent.Set();
            Server.Serve();
        }

        public void CreateBuiltinServer()
        {
            Console.WriteLine("Initializing the built-in editor server...");

            serverCreatedEvent = new ManualResetEventSlim();
            Server = new EditorServer();

            System.Threading.Tasks.Task.Run((Action)RunBuiltinServer);
            serverCreatedEvent.Wait();
        }
    }
}