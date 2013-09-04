
using System;

namespace Flood.RPC.Serialization
{
    public interface IDataObject
    {
        void Read(RPCData data);
        void Write(RPCData data);
        void Write(RPCData data, int[] propertyIds);
    }

    public interface IObservableDataObject : IDataObject
    {
        event Action<int> PropertyChanged;
        void WriteChanges(RPCData data);
    }
}
