
namespace Flood.Serialization
{
    public enum DataType : byte
    {
        Stop = 0,
        Void = 1,
        Bool = 2,
        Byte = 3,
        Double = 4,
        Exception = 5,
        I16 = 6,
        I32 = 8,
        I64 = 10,
        String = 11,
        DataObject = 12,
        Map = 13,
        List = 14,
        Guid = 15,
        DateTime = 16,
        Custom = 17
    }
}
