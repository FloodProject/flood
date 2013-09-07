using System;
using System.Collections.Generic;
using Flood;
using Flood.RPC;
using Flood.RPC.Metadata;
using NUnit.Framework;
using System.Reflection;
using System.Threading.Tasks;

namespace RPCGen.Tests.Services
{
    [TestFixture]
    public class DataObjectTests : ITypeTests
    {
        #region DataObjects

        [DataObject(2)]
        public class ByteDO
        {
            [Id(0)] public byte Value { get; set; }
        }

        [DataObject(3)]
        public class FloatDO
        {
            [Id(0)]
            public float Value;
        }

        [DataObject(4)]
        public class DoubleDO
        {
            [Id(0)]
            public double Value { get; set; }
        }

        [DataObject(5)]
        public class ShortDO
        {
            [Id(0)]
            public short Value;
        }

        [DataObject(6)]
        public class IntDO
        {
            [Id(0)]
            public int Value;
        }

        [DataObject(7)]
        public class LongDO
        {
            [Id(0)]
            public long Value;
        }

        [DataObject(8)]
        public class StringDO
        {
            [Id(0)]
            public string Value;
        }

        [DataObject(9)]
        public class GuidDO
        {
            [Id(0)]
            public Guid Value;
        }

        [DataObject(10)]
        public class DateTimeDO
        {
            [Id(0)]
            public DateTime Value;
        }

        [DataObject(11)]
        public class MapDO
        {
            [Id(0)]
            public Dictionary<int, string> Value;
        }

        [DataObject(12)]
        public class ListDO
        {
            [Id(0)]
            public List<int> Value;
        }

        [DataObject(13)]
        public class ArrayDO
        {
            [Id(0)]
            public int[] Value;
        }

        #endregion

        #region Service 

        [Service]
        public interface IService
        {
            [Id(1)]
            Task TestVoid();

            [Id(2)]
            Task<ByteDO> TestByte([Id(0)]ByteDO param);

            [Id(3)]
            Task<FloatDO> TestFloat([Id(0)]FloatDO param);

            [Id(4)]
            Task<DoubleDO> TestDouble([Id(0)]DoubleDO param);

            [Id(5)]
            Task<ShortDO> TestShort([Id(0)]ShortDO param);

            [Id(6)]
            Task<IntDO> TestInt([Id(0)]IntDO param);

            [Id(7)]
            Task<LongDO> TestLong([Id(0)]LongDO param);

            [Id(8)]
            Task<StringDO> TestString([Id(0)]StringDO param);

            [Id(9)]
            Task<GuidDO> TestGuid([Id(0)]GuidDO param);

            [Id(10)]
            Task<DateTimeDO> TestDateTime([Id(0)]DateTimeDO param);

            [Id(11)]
            Task<MapDO> TestMap([Id(0)]MapDO param);

            [Id(12)]
            Task<ListDO> TestList([Id(0)]ListDO param);

            [Id(13)]
            Task<ArrayDO> TestArray([Id(0)]ArrayDO param);
        }

        public class Service : IService
        {
            public async Task TestVoid()
            {
            }

            public async Task<ByteDO> TestByte(ByteDO param)
            {
                return param;
            }

            public async Task<FloatDO> TestFloat(FloatDO param)
            {
                return param;
            }

            public async Task<DoubleDO> TestDouble(DoubleDO param)
            {
                return param;
            }

            public async Task<ShortDO> TestShort(ShortDO param)
            {
                return param;
            }

            public async Task<IntDO> TestInt(IntDO param)
            {
                return param;
            }

            public async Task<LongDO> TestLong(LongDO param)
            {
                return param;
            }

            public async Task<StringDO> TestString(StringDO param)
            {
                return param;
            }

            public async Task<GuidDO> TestGuid(GuidDO param)
            {
                return param;
            }

            public async Task<DateTimeDO> TestDateTime(DateTimeDO param)
            {
                return param;
            }

            public async Task<MapDO> TestMap(MapDO param)
            {
                return param;
            }

            public async Task<ListDO> TestList(ListDO param)
            {
                return param;
            }

            public async Task<ArrayDO> TestArray(ArrayDO param)
            {
                return param;
            }
        }

        #endregion

        IService service;

        [TestFixtureSetUp]
        public void SetUp()
        {
            var serviceImpl = new Service();
            service = Helper.GetProxy<IService>(serviceImpl);
        }

        [Test]
        public void TestVoid()
        {
            service.TestVoid();
        }

        [Test]
        public void TestByte()
        {
            ByteDO d = new ByteDO();
            Task<ByteDO> t;

            d.Value = byte.MinValue;
            t = service.TestByte(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = byte.MaxValue;
            t = service.TestByte(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestFloat()
        {
            FloatDO d = new FloatDO();
            Task<FloatDO> t;

            d.Value = float.MinValue;
            t = service.TestFloat(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = float.MaxValue;
            t = service.TestFloat(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestDouble()
        {
            DoubleDO d = new DoubleDO();
            Task<DoubleDO> t;

            d.Value = double.MinValue;
            t = service.TestDouble(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = double.MaxValue;
            t = service.TestDouble(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestShort()
        {
            ShortDO d = new ShortDO();
            Task<ShortDO> t;

            d.Value = short.MinValue;
            t = service.TestShort(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = short.MaxValue;
            t = service.TestShort(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestInt()
        {
            IntDO d = new IntDO();
            Task<IntDO> t;

            d.Value = int.MinValue;
            t = service.TestInt(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = int.MaxValue;
            t = service.TestInt(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestLong()
        {
            LongDO d = new LongDO();
            Task<LongDO> t;

            d.Value = long.MinValue;
            t = service.TestLong(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = long.MaxValue;
            t = service.TestLong(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestString()
        {
            StringDO d = new StringDO();
            Task<StringDO> t;

            d.Value = "";
            t = service.TestString(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = "1234567890";
            t = service.TestString(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = "\01234567890";
            t = service.TestString(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestGuid()
        {
            GuidDO d = new GuidDO();
            Task<GuidDO> t;

            d.Value = new Guid("CB873FC8-6759-4F80-81BE-0EC214BAB26E");
            t = service.TestGuid(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestDateTime()
        {
            DateTimeDO d = new DateTimeDO();
            Task<DateTimeDO> t;

            d.Value = DateTime.MinValue;
            t = service.TestDateTime(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = DateTime.Now;
            t = service.TestDateTime(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);

            d.Value = DateTime.MaxValue;
            t = service.TestDateTime(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d.Value, t.Result.Value);
        }

        [Test]
        public void TestMap()
        {
            MapDO d = new MapDO();
            d.Value = new Dictionary<int, string>();
            d.Value.Add(1,"1");
            d.Value.Add(2,"2");
            d.Value.Add(3,"3");
            var task = service.TestMap(d);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(d.Value.Count, task.Result.Value.Count);
            Assert.AreEqual(d.Value[1], task.Result.Value[1]);
            Assert.AreEqual(d.Value[2], task.Result.Value[2]);
            Assert.AreEqual(d.Value[3], task.Result.Value[3]);
        }

        [Test]
        public void TestList()
        {
            ListDO d = new ListDO();
            d.Value = new List<int>{1,2,3};
            var task = service.TestList(d);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(d.Value.Count, task.Result.Value.Count);
            Assert.AreEqual(d.Value[0], task.Result.Value[0]);
            Assert.AreEqual(d.Value[1], task.Result.Value[1]);
            Assert.AreEqual(d.Value[2], task.Result.Value[2]);
        }

        [Test]
        public void TestArray()
        {
            ArrayDO d = new ArrayDO();
            d.Value = new []{1,2,3};
            var task = service.TestArray(d);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(d.Value.Length, task.Result.Value.Length);
            Assert.AreEqual(d.Value[0], task.Result.Value[0]);
            Assert.AreEqual(d.Value[1], task.Result.Value[1]);
            Assert.AreEqual(d.Value[2], task.Result.Value[2]);
        }
    }
}
