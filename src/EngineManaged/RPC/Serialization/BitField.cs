
namespace Flood.RPC.Serialization
{
    public struct BitField
    {
        public long Bits;

        public void SetBit(int bitIndex, bool state = true)
        {
            var bit = (1 << bitIndex);
            Bits = (state)? (Bits | bit) : (Bits & ~bit);
        }

        public bool GetBit(int bitIndex)
        {
            return (Bits & (1 << bitIndex)) != 0;
        }

        public void Reset()
        {
            Bits = 0;
        }
    }
}
