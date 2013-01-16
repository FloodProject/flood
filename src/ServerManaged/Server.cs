using Flood;
using Flood.RPC.Server;
using Flood.RPC.Transport;

namespace Flood.Server
{
    public abstract class Server
    {
        public TSimpleServer RPCServer { get; set; }

        public TServerSocket Socket { get; set; }

        protected Server()
        {
        }

        public void Init()
        {

        }

        public void Shutdown()
        {

        }

        public void Update()
        {

        }
    }
}