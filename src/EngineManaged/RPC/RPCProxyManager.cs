using System;
using System.Collections.Generic;

namespace Flood.RPC
{
    public class RPCProxyManager
    {
        private Dictionary<int, RPCProxy> proxies;
        private int proxyCounter;

        public RPCProxyManager()
        {
            proxies = new Dictionary<int, RPCProxy>();
        }

        public RPCProxy this[int id]
        {
            get { return proxies[id]; }
        }

        internal T CreateProxy<T>(RPCPeer peer, int implId)
        {
            var serviceType = typeof(T);
            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(Helper.GetStubsClassName(serviceType, true));
            var proxyType = implType.GetNestedType("Proxy");

            var proxyId = proxyCounter++;

            var proxy = Activator.CreateInstance(proxyType, peer, implId, proxyId);

            proxies.Add(proxyId, (RPCProxy)proxy);

            return (T)proxy;
        }
    }
}
