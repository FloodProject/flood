using System;

namespace Flood.RPC
{
    public abstract class RPCPeer
    {
        public abstract void DispatchCall(RPCData data);

        public abstract override bool Equals(Object other);
        public abstract override int GetHashCode();
    }
}
