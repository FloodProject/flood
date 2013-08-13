
namespace Flood.Server
{
    public abstract class Server
    {
        public Network.Server Host;

        public IDatabaseManager Database { get; set; }

        protected Server(HostConnectionDetails details)
        {
            Host = new Network.Server(details);

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