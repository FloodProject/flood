using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public abstract class RPCDelegate
    {
        public RPCPeer Peer;
        public int RemoteId;
        public int LocalId;
        public int DelegateId;

        public Delegate Delegate;

        public RPCDelegate()
        {
            callProcessor = new RPCCallProcessor();
        }

        public RPCDelegate(RPCPeer peer, int localId, int remoteId, int delegateId)
        {
            Peer = peer;
            RemoteId = remoteId;
            LocalId = localId;
            DelegateId = delegateId;
            Delegate = del;
        }

        public abstract void Invoke(RPCData data);
    }
}
