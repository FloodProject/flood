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
        protected int ImplId;
        protected int ProxyId;
        protected int DelegateId;

        protected Delegate Delegate;

        public RPCDelegate(RPCPeer peer, int implId, int proxyId, int delegateId, Delegate del)
        {
            Peer = peer;
            ImplId = implId;
            ProxyId = proxyId;
            DelegateId = delegateId;
            Delegate = del;
        }

        public abstract void Invoke(RPCData data);
    }
}
