using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gwen
{
    public class UserData
    {
        private Dictionary<string, object> data;

        public UserData()
        {
            data = new Dictionary<string, object>();
        }

        public void Add(string key, object value)
        {
            data.Add(key, value);
        }

        public T Get<T>(string key)
        {
            return (T) data[key];
        }

        public bool ContainsKey(string key)
        {
            return data.ContainsKey(key);
        }
    }
}
