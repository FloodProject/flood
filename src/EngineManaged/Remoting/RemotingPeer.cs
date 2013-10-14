using Flood.Serialization;
using System;

namespace Flood.Remoting
{
    public abstract class RemotingPeer
    {
        public abstract void Dispatch(Message data);

        public abstract Serializer CreateSerializer();

        public abstract override bool Equals(Object other);
        public abstract override int GetHashCode();
    }
}
