
using System;

namespace Flood.Remoting.Serialization
{
    public interface IDataObject
    {
        void Read(Message data, Type baseType = null);
        void Write(Message data, Type baseType = null);

        unsafe void Write(Message data, BitField* bitFields, int bitFieldCount);
        int BaseDataObjectCount { get; }
    }

    public interface IObservableDataObject : IDataObject
    {
        event Action<Type, int> PropertyChanged;
        unsafe void GetResetChanges(BitField* bitFields);
        bool IsReference { get; set; }
    }

    public interface IDataObjectReference
    {
        RemotingPeer Peer { get; }
        int RemoteId { get; }
        ReferenceManager ReferenceManager { get; }
    }
}
