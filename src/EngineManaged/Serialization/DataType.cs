
namespace Flood.Serialization
{
    public enum DataType : byte
    {
        Stop = 0,
        Void = 1,
        Bool = 2,
        Byte = 3,
        Double = 4,
        I16 = 5,
        I32 = 6,
        I64 = 7,
        String = 8,
        Map = 9,
        List = 10,
        Guid = 11,
        DateTime = 12,
        Custom = 13
    }
}
