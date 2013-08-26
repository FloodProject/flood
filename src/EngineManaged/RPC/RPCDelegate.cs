using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public abstract class RPCDelegate
    {
        protected RPCPeer Peer;
        protected int RemoteId;
        protected int LocalId;
        protected int DelegateId;

        protected Delegate Delegate;

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
