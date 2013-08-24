using System;
using System.Collections.Generic;

namespace Flood.RPC
{
    public class RPCProxyManager
    {
        private Dictionary<int, RPCProxy> proxies;
        private int proxyCounter;

        private int sequenceNumber;

        public RPCProxyManager()
        {
            proxies = new Dictionary<int, RPCProxy>();
        }

        internal void Process(RPCData data)
        {
            RPCProxy proxy;
            if (!proxies.TryGetValue(data.Header.ProxyId, out proxy))
            {
                Log.Error("Received RPC data to unexpected Proxy.");
                return;
            }

            switch (data.Header.CallType)
            {
            case RPCDataType.Reply:
                proxy.ProcessReply(data);
                return;
            case RPCDataType.EventInvoke:
                proxy.InvokeEvent(data);
                return;
            default:
                throw new NotImplementedException();
            }
        }

        internal T CreateProxy<T>(RPCPeer peer, int implId)
        {
            var serviceType = typeof(T);
            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(Helper.ImplName(serviceType, true));
            var proxyType = implType.GetNestedType("Proxy");

            var proxyId = proxyCounter++;

            var proxy = Activator.CreateInstance(proxyType, peer, implId, proxyId);

            proxies.Add(proxyId, (RPCProxy)proxy);

            return (T)proxy;
        }
    }
}
