using Flood.RPC.Serialization;
using System;
using System.Collections.Generic;

namespace Flood.RPC
{
    public class RPCManager
    {
        public RPCImplManager ImplementationManager { get; private set; }

        public Dictionary<RPCPeer, RPCProxyManager> ProxyManagers { get; private set; }

        public RPCManager()
        {
            ImplementationManager = new RPCImplManager();
            ProxyManagers = new Dictionary<RPCPeer, RPCProxyManager>();
        }

        public void Process(byte[] data, RPCPeer peer)
        {
            var serializer = new BinarySerializer();
            serializer.Buffer.Write(data);
            var rpcData = new RPCData(serializer);
            rpcData.Header.Read();
            rpcData.Peer = peer;

            Process(rpcData);
        }

        public void Process(RPCData data)
        {
            var peer = data.Peer;

            switch(data.Header.CallType)
            {
            case RPCDataType.Call:
            {
                ImplementationManager.Process(data);
                return;
            }
            case RPCDataType.Reply:
            {
                RPCProxyManager proxyManager;
                if (!ProxyManagers.TryGetValue(peer, out proxyManager))
                {
                    Log.Error("RPC proxy not found for peer:" + peer);
                    return;
                } 

                proxyManager.Process(data);
                return;
            }
            default:
                throw new NotImplementedException();
            }
        }

        public int AddImplementation<T>(T service)
        {
            return ImplementationManager.AddImplementation(service);
        }

        public T CreateProxy<T>(RPCPeer peer, int serviceId)
        {
            if (!ProxyManagers.ContainsKey(peer))
                ProxyManagers.Add(peer, new RPCProxyManager(peer));

            return ProxyManagers[peer].CreateProxy<T>(serviceId);
        }

    }
}
