using System;
using System.Threading;
using Flood.Editor.Server;
using Flood.Network;

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

            while(true)
                Server.Update();
        }

        public void CreateBuiltinServer()
        {
            Log.Info("Initializing the built-in editor server...");

            serverCreatedEvent = new ManualResetEventSlim();

            var endPoint = new HostEndPoint(Settings.Host, Settings.RPCPort);
            Server = new EditorServer(endPoint);

            System.Threading.Tasks.Task.Run((Action)RunBuiltinServer);
            serverCreatedEvent.Wait();
        }
    }
}