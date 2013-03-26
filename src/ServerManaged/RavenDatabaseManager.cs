#if RAVENDB
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Raven.Client;
using Raven.Client.Embedded;

namespace Flood.Server
{
    public class RavenDatabaseManager : IDatabaseManager
    {
        /// <summary>
        /// Server database  
        /// </summary>
        private EmbeddableDocumentStore Database;

        private IDocumentSession session;

        public RavenDatabaseManager()
        {
            Database = new EmbeddableDocumentStore
            {
                DataDirectory = "Data"
            };
            Database.Initialize();
        }

        /// <summary>
        /// Start database session 
        /// </summary>
        public void StartSession()
        {
            session = Database.OpenSession();
        }

        /// <summary>
        /// Save changes to database 
        /// </summary>
        public void SaveChanges() 
        {
            if(session == null)
                throw new SessionNotOpenException();
            session.SaveChanges();
        }

        /// <summary>
        /// Store entity in the database 
        /// </summary>
        public void Store(dynamic entity, Guid id)
        {
            if (entity == null) throw new ArgumentNullException("entity");

            if (session == null)
                throw new SessionNotOpenException();

            session.Store(entity, id);
        }

        /// <summary>
        /// Load entity from the database 
        /// </summary>
        public T Load<T>(Guid id)
        {
            if (session == null)
                throw new SessionNotOpenException();

            return session.Load<T>(id);
        }

        /// <summary>
        /// Delete entity from the database 
        /// </summary>
        public void Delete<T>(Guid id)
        {
            if (session == null)
                throw new SessionNotOpenException();

            session.Delete(session.Load<T>(id));
        }

        /// <summary>
        /// Load all entities of class T from the database 
        /// </summary>
        public List<T> LoadAll<T>()
        {
            if (session == null)
               throw new SessionNotOpenException();

            const int pageSize = 128;
            RavenQueryStatistics stats;
            List<T> results = session.Query<T>().Statistics(out stats).Take(pageSize).ToList();
            int nResults = stats.TotalResults;
            
            for(int i = 1; i*pageSize < nResults; i++ )
                results.AddRange(session.Query<T>().Skip(i*pageSize).Take(pageSize).ToList());
            
            return  results;
        }
    }
}
#endif