using System;
using System.Collections.Generic;
using Flood.Server;

namespace Flood.Server
{
    public class NullDatabaseManager : IDatabaseManager
    {
        public void StartSession()
        {
        }

        public void SaveChanges()
        {
        }

        public void Store(dynamic entity, Guid id)
        {
        }

        public T Load<T>(Guid id)
        {
            return default(T);
        }

        public void Delete<T>(Guid id)
        {
        }

        public List<T> LoadAll<T>()
        {
            return new List<T>();
        }
    }
}
