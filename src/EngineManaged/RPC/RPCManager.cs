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
            switch(data.Header.CallType)
            {
            case RPCDataType.EventSubscribe:
            case RPCDataType.Call:
                ImplementationManager.Process(data);
                return;
            case RPCDataType.Reply:
            case RPCDataType.EventInvoke:
                ProxyManager.Process(data);
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
