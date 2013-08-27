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

        [DataObject("4B781EAE-CDA6-45A7-8ADB-E4FE295D78D1")]
        public struct ByteDO
        {
            [Id(0)]
            public byte Value;
        }

        [DataObject("41327695-0E25-4801-B320-0B1D7074FDC8")]
        public struct FloatDO
        {
            [Id(0)]
            public float Value;
        }

        [DataObject("9504185F-A0B5-4CCC-B673-CFA926558424")]
        public struct DoubleDO
        {
            [Id(0)]
            public double Value;
        }

        [DataObject("84851EB5-808C-4360-BC34-FB180446743D")]
        public struct ShortDO
        {
            [Id(0)]
            public short Value;
        }

        [DataObject("117F25CD-5E4A-4321-A064-66FDCB86F534")]
        public struct IntDO
        {
            [Id(0)]
            public int Value;
        }

        [DataObject("F5D2E125-A44A-4CA3-86E3-CDD04D20895C")]
        public struct LongDO
        {
            [Id(0)]
            public long Value;
        }

        [DataObject("7AE92565-5623-480D-8185-778578AFDFFA")]
        public struct StringDO
        {
            [Id(0)]
            public string Value;
        }

        [DataObject("4AD9280A-8D47-4240-8AEC-4C92C8B1EC9C")]
        public struct GuidDO
        {
            [Id(0)]
            public Guid Value;
        }

        [DataObject("01938C12-CD0E-40EF-9A9F-5A2B3E492294")]
        public struct DateTimeDO
        {
            [Id(0)]
            public DateTime Value;
        }

        [DataObject("41EBBBB3-ADA1-42E9-8071-0BCF8C2B8D4A")]
        public struct MapDO
        {
            [Id(0)]
            public Dictionary<int, string> Value;
        }

        [DataObject("7A6D650C-B6C5-41C4-AE5E-C29707E5B82C")]
        public struct ListDO
        {
            [Id(0)]
            public List<int> Value;
        }

        [DataObject("52F0BDA2-4E91-40D2-9136-33B685DB2823")]
        public struct ArrayDO
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
            ByteDO d;
            Task<ByteDO> t;

            d.Value = byte.MinValue;
            t = service.TestByte(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = byte.MaxValue;
            t = service.TestByte(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestFloat()
        {
            FloatDO d;
            Task<FloatDO> t;

            d.Value = float.MinValue;
            t = service.TestFloat(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = float.MaxValue;
            t = service.TestFloat(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestDouble()
        {
            DoubleDO d;
            Task<DoubleDO> t;

            d.Value = double.MinValue;
            t = service.TestDouble(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = double.MaxValue;
            t = service.TestDouble(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestShort()
        {
            ShortDO d;
            Task<ShortDO> t;

            d.Value = short.MinValue;
            t = service.TestShort(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = short.MaxValue;
            t = service.TestShort(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestInt()
        {
            IntDO d;
            Task<IntDO> t;

            d.Value = int.MinValue;
            t = service.TestInt(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = int.MaxValue;
            t = service.TestInt(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestLong()
        {
            LongDO d;
            Task<LongDO> t;

            d.Value = long.MinValue;
            t = service.TestLong(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = long.MaxValue;
            t = service.TestLong(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestString()
        {
            StringDO d;
            Task<StringDO> t;

            d.Value = "";
            t = service.TestString(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = "1234567890";
            t = service.TestString(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = "\01234567890";
            t = service.TestString(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestGuid()
        {
            GuidDO d;
            Task<GuidDO> t;

            d.Value = new Guid("CB873FC8-6759-4F80-81BE-0EC214BAB26E");
            t = service.TestGuid(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestDateTime()
        {
            DateTimeDO d;
            Task<DateTimeDO> t;

            d.Value = DateTime.MinValue;
            t = service.TestDateTime(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = DateTime.Now;
            t = service.TestDateTime(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);

            d.Value = DateTime.MaxValue;
            t = service.TestDateTime(d);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(d, t.Result);
        }

        [Test]
        public void TestMap()
        {
            MapDO d;
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
            ListDO d;
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
            ArrayDO d;
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
