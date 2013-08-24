using Flood.RPC.Serialization;
using System;
using System.Collections.Generic;

namespace Flood.RPC
{
    public class RPCManager
    {
        public RPCImplManager ImplementationManager { get; private set; }

        public RPCProxyManager ProxyManager { get; private set; }

        public RPCManager()
        {
            ImplementationManager = new RPCImplManager();
            ProxyManager = new RPCProxyManager();
        }

        public void Process(byte[] data, RPCPeer peer)
        {
            var serializer = new BinarySerializer();
            serializer.Buffer.Write(data, 0, data.Length);
            serializer.Buffer.Position = 0;

            var rpcData = new RPCData(peer, serializer);
            rpcData.Header.Read();

            Process(rpcData);
        }

        public void Process(RPCData data)
        {
            var implId = data.Header.ImplId;
            var proxyId = data.Header.ProxyId;

            switch(data.Header.CallType)
            {
            case RPCDataType.Call:
                ImplementationManager[implId].ProcessCall(data);
                return;
            case RPCDataType.EventSubscribe:
                ImplementationManager[implId].ProcessEventSubscribe(data);
                return;
            case RPCDataType.EventUnsubscribe:
                ImplementationManager[implId].ProcessEventUnsubscribe(data);
                return;
            case RPCDataType.Reply:
                ProxyManager[proxyId].ProcessReply(data);
                return;
            case RPCDataType.DelegateCall:
                ProxyManager[proxyId].ProcessDelegateCall(data);
                return;
            case RPCDataType.DelegateReply:
                ProxyManager[proxyId].ProcessDelegateReply(data);
                return;
            default:
                throw new NotImplementedException();
            }
        }

        public int AddImplementation<T>(T service)
        {
            return ImplementationManager.AddImplementation(service);
        }

        public T CreateProxy<T>(RPCPeer peer, int implId)
        {
            return ProxyManager.CreateProxy<T>(peer, implId);
        }

    }
}
