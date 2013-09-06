
using System;

namespace Flood.RPC.Serialization
{
    public interface IDataObject
    {
        void Read(RPCData data);
        void Write(RPCData data);
        void Write(RPCData data, BitField properties);
    }

    public interface IObservableDataObject : IDataObject
    {
        event Action<int> PropertyChanged;
        BitField GetResetChanges();
    }
}
