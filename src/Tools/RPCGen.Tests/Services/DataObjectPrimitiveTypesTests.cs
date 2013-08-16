using System;
using Flood;
using Flood.RPC;
using Flood.RPC.Metadata;
using NUnit.Framework;
using System.Reflection;
using System.Threading.Tasks;
using RPCGen.Tests.Interfaces;

namespace RPCGen.Tests.Services
{
    

    public class DataObjectPrimitiveTypesTests : IPrimitiveTypesTests
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
        }

        #endregion

        IService service;

        public void SetUp()
        {
            var serviceImpl = new Service();
            service = Helper.GetProxy<IService>(serviceImpl);
        }

        public void TestVoid()
        {
            service.TestVoid();
        }

        public void TestByte()
        {
            ByteDO d;
            Task<ByteDO> t;

            d.Value = byte.MinValue;
            t = service.TestByte(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = byte.MaxValue;
            t = service.TestByte(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }

        public void TestFloat()
        {
            FloatDO d;
            Task<FloatDO> t;

            d.Value = float.MinValue;
            t = service.TestFloat(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = float.MaxValue;
            t = service.TestFloat(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }

        public void TestDouble()
        {
            DoubleDO d;
            Task<DoubleDO> t;

            d.Value = double.MinValue;
            t = service.TestDouble(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = double.MaxValue;
            t = service.TestDouble(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }

        public void TestException()
        {
            throw new NotImplementedException();
        }

        public void TestShort()
        {
            ShortDO d;
            Task<ShortDO> t;

            d.Value = short.MinValue;
            t = service.TestShort(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = short.MaxValue;
            t = service.TestShort(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }

        public void TestInt()
        {
            IntDO d;
            Task<IntDO> t;

            d.Value = int.MinValue;
            t = service.TestInt(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = int.MaxValue;
            t = service.TestInt(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }

        public void TestLong()
        {
            LongDO d;
            Task<LongDO> t;

            d.Value = long.MinValue;
            t = service.TestLong(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = long.MaxValue;
            t = service.TestLong(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }

        public void TestString()
        {
            StringDO d;
            Task<StringDO> t;

            d.Value = "";
            t = service.TestString(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = "1234567890";
            t = service.TestString(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = "\01234567890";
            t = service.TestString(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }

        public void TestGuid()
        {
            GuidDO d;
            Task<GuidDO> t;

            d.Value = new Guid("CB873FC8-6759-4F80-81BE-0EC214BAB26E");
            t = service.TestGuid(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }

        public void TestDateTime()
        {
            DateTimeDO d;
            Task<DateTimeDO> t;

            d.Value = DateTime.MinValue;
            t = service.TestDateTime(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = DateTime.Now;
            t = service.TestDateTime(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);

            d.Value = DateTime.MaxValue;
            t = service.TestDateTime(d);
            t.Wait();
            Assert.AreEqual(d, t.Result);
        }
    }
}
