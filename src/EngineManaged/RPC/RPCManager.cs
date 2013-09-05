using Flood.RPC.Serialization;
using System;

namespace Flood.RPC
{
    public class RPCManager
    {
        public RPCPeer Local { get; private set; }

        public ServiceManager ServiceManager { get; private set; }
        public DelegateManager DelegateManager { get; private set; }

        public RPCManager(RPCPeer local)
        {
            ServiceManager = new ServiceManager(this);
            DelegateManager = new DelegateManager(this);

            Local = local;
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
            switch (data.Header.CallType)
            {
                case RPCDataType.Call:
                case RPCDataType.Reply:
                case RPCDataType.EventSubscribe:
                case RPCDataType.EventUnsubscribe:
                    ServiceManager.Process(data);
                    return;
                case RPCDataType.DelegateReply:
                case RPCDataType.DelegateCall:
                    DelegateManager.Process(data);
                    return;
                default:
                    throw new NotImplementedException();
            }
        }

        public T GetService<T>(RPCPeer peer, int implId)
        {
            if (peer.Equals(Local))
                return ServiceManager.GetLocalService<T>(implId);

            return ServiceManager.GetRemoteService<T>(peer, implId);
        }

        public T GetGlobalService<T>(RPCPeer peer)
        {
            if (peer.Equals(Local))
                return ServiceManager.GetGlobalLocalService<T>();

            return ServiceManager.GetGlobalRemoteService<T>(peer);
        }

    }
}
