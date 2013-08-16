
using Flood.Network;
namespace Flood.Server
{
    public abstract class Server
    {
        public Network.Server Host;

        public IDatabaseManager Database { get; set; }

        protected Server(HostEndPoint endPeer)
        {
            Host = new Network.Server(endPeer);

#if USE_RAVENDB
            Database = new RavenDatabaseManager();
#else
            Database = new NullDatabaseManager();
#endif
        }

        public virtual void Update()
        {
            Host.Update();
        }
    }
}