
namespace Flood.RPC.Serialization
{
    public interface IDataObject
    {
        void Read(RPCData data);
        void Write(RPCData data);
        void Write(RPCData data, int[] propertyIds);
    }
}
