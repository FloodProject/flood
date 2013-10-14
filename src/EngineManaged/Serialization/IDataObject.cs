
using System;
using Flood.Remoting;

namespace Flood.Serialization
{
    public interface IDataObject
    {
        void Read(Serializer data, Type baseType = null, object customData = null);
        void Write(Serializer data, Type baseType = null, object customData = null);

        unsafe void Write(Serializer data, BitField* bitFields, int bitFieldCount, object customData = null);
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
