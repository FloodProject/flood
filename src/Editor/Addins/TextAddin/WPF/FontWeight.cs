
namespace TextAddin
{
    public struct FontWeight
    {
        public int Weight { get; private set; }

        public FontWeight(int weight) : this()
        {
            Weight = weight;
        }

        public static FontWeight FromOpenTypeWeight(int openTypeWeight)
        {
            throw new System.NotImplementedException();
        }

        public object ToOpenTypeWeight()
        {
            throw new System.NotImplementedException();
        }
    }
}
