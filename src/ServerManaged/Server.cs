using Flood;
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
        }

        public void Init()
        {
            //init db
#if RAVENDBSET
            Database = new RavenDatabaseManager();
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