using Flood.RPC.Server;
using Flood.RPC.Transport;

namespace Flood.Server
{
    public abstract class Server
    {
        public IDatabaseManager Database { get; set; }

        public TSimpleServer RPCServer { get; set; }

        public TServerSocket Socket { get; set; }

        protected Server()
        {
#if USE_RAVENDB
            Database = new RavenDatabaseManager();
#else
            Database = new NullDatabaseManager();
#endif
        }

        public void Shutdown()
        {

        }

        public void Update()
        {

        }
    }
}