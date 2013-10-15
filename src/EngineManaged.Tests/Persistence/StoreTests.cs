using Flood.Persistence;
using Flood.Remoting.Metadata;
using NUnit.Framework;

namespace Flood.Tests.Persistence
{
    [TestFixture]
    public class DataBaseTests
    {
        [DataObject(1)]
        public class Data
        {
            [Id(1)] public byte ByteMin;
            [Id(2)] public byte ByteMax;
            [Id(3)] public short ShortMin;
            [Id(4)] public short ShortMax;
            [Id(5)] public int IntMin;
            [Id(6)] public int IntMax;
            [Id(7)] public long LongMin;
            [Id(8)] public long LongMax;
            [Id(9)] public float FloatMin;
            [Id(10)] public float FloatMax;
            [Id(11)] public double DoubleMin;
            [Id(12)] public double DoubleMax;
        }

        [Test]
        public void TestStore()
        {
            var dataFile = "data.store";

            var data = new Data
            {
                ByteMin = byte.MinValue,
                ByteMax = byte.MaxValue,
                ShortMin = short.MinValue,
                ShortMax = short.MaxValue,
                IntMin = int.MinValue,
                IntMax = int.MaxValue,
                LongMin = long.MinValue,
                LongMax = long.MaxValue,
                FloatMin = float.MinValue,
                FloatMax = float.MaxValue,
                DoubleMin = double.MinValue,
                DoubleMax = double.MaxValue
            };

            Assert.IsTrue(Store.Save(data, dataFile));

            var data2 = Store.Load<Data>(dataFile);

            Assert.AreEqual(data.ByteMin, data2.ByteMin);
            Assert.AreEqual(data.ByteMax, data2.ByteMax);
            Assert.AreEqual(data.ShortMin, data2.ShortMin);
            Assert.AreEqual(data.ShortMax, data2.ShortMax);
            Assert.AreEqual(data.IntMin, data2.IntMin);
            Assert.AreEqual(data.IntMax, data2.IntMax);
            Assert.AreEqual(data.LongMin, data2.LongMin);
            Assert.AreEqual(data.LongMax, data2.LongMax);
            Assert.AreEqual(data.FloatMin, data2.FloatMin);
            Assert.AreEqual(data.FloatMax, data2.FloatMax);
            Assert.AreEqual(data.DoubleMin, data2.DoubleMin);
            Assert.AreEqual(data.DoubleMax, data2.DoubleMax);
        }
    }
}
