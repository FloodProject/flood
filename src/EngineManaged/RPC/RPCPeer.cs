using Flood.RPC.Serialization;
using System;
using System.Diagnostics;

namespace Flood.RPC
{
    public abstract class RPCPeer
    {
        public abstract void Dispatch(RPCData data);

        public abstract Serializer CreateSerializer();

        public abstract override bool Equals(Object other);
        public abstract override int GetHashCode();
    }
}
