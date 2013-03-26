using System;
using System.Threading;
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
            Log.Info("Initializing the built-in editor server...");

            serverCreatedEvent = new ManualResetEventSlim();
            Server = new EditorServer();

            System.Threading.Tasks.Task.Run((Action)RunBuiltinServer);
            serverCreatedEvent.Wait();
        }
    }
}