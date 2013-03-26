using System;
using System.Collections.Generic;

namespace Flood.Server
{
    public class SessionNotOpenException : Exception
    {
        public SessionNotOpenException()
            : base("A session has not been opened.")
        {
        }
    }

    public interface IDatabaseManager
    {
        /// <summary>
        /// Start database session
        /// </summary>
        void StartSession();

        /// <summary>
        /// Save changes to database
        /// </summary>
        void SaveChanges(); 

        /// <summary>
        /// Store entity in the database
        /// </summary>
        void Store(dynamic entity, Guid id);

        /// <summary>
        /// Load entity from the database 
        /// </summary>
        T Load<T>(Guid id);

        /// <summary>
        /// Delete entity from the database
        /// </summary>
        void Delete<T>(Guid id);

        /// <summary>
        /// Load all entities of class T from the database
        /// </summary>
        List<T> LoadAll<T>();
    }

}
