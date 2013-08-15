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
    public class ServicePrimitiveTypesTests : IPrimitiveTypesTests
    {
        #region Service
        [Service]
        public interface IService
        {
            Task TestVoid();
            Task<Byte> TestByte([Id(0)]Byte param);
            Task<float> TestFloat([Id(0)]float param);
            Task<double> TestDouble([Id(0)]double param);
            Task<short> TestShort([Id(0)]short param);
            Task<int> TestInt([Id(0)]int param);
            Task<long> TestLong([Id(0)]long param);
            Task<string> TestString([Id(0)]string param);
            Task<Guid> TestGuid([Id(0)]Guid param);
            Task<DateTime> TestDateTime([Id(0)]DateTime param);
        }

        public class Service : IService
        {

            public async Task TestVoid()
            {
            }

            public async Task<byte> TestByte(byte param)
            {
                return param;
            }

            public async Task<float> TestFloat(float param)
            {
                return param;
            }

            public async Task<double> TestDouble(double param)
            {
                return param;
            }

            public async Task<short> TestShort(short param)
            {
                return param;
            }

            public async Task<int> TestInt(int param)
            {
                return param;
            }

            public async Task<long> TestLong(long param)
            {
                return param;
            }

            public async Task<string> TestString(string param)
            {
                return param;
            }

            public async Task<Guid> TestGuid(Guid param)
            {
                return param;
            }

            public async Task<DateTime> TestDateTime(DateTime param)
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
            byte v;
            Task<byte> t;

            v = byte.MinValue;
            t = service.TestByte(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = byte.MaxValue;
            t = service.TestByte(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }

        public void TestFloat()
        {
            float v;
            Task<float> t;

            v = float.MinValue;
            t = service.TestFloat(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = float.MaxValue;
            t = service.TestFloat(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }

        public void TestDouble()
        {
            double v;
            Task<double> t;

            v = double.MinValue;
            t = service.TestDouble(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = double.MaxValue;
            t = service.TestDouble(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }

        public void TestException()
        {
            throw new NotImplementedException();
        }

        public void TestShort()
        {
            short v;
            Task<short> t;

            v = short.MinValue;
            t = service.TestShort(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = short.MaxValue;
            t = service.TestShort(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }

        public void TestInt()
        {
            int v;
            Task<int> t;

            v = int.MinValue;
            t = service.TestInt(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = int.MaxValue;
            t = service.TestInt(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }

        public void TestLong()
        {
            long v;
            Task<long> t;

            v = long.MinValue;
            t = service.TestLong(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = long.MaxValue;
            t = service.TestLong(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }

        public void TestString()
        {
            string v;
            Task<string> t;

            v = "";
            t = service.TestString(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = "1234567890";
            t = service.TestString(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = "\01234567890";
            t = service.TestString(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }

        public void TestGuid()
        {
            var v = new Guid("CB873FC8-6759-4F80-81BE-0EC214BAB26E");
            Task<Guid> t;
            t = service.TestGuid(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }

        public void TestDateTime()
        {
            DateTime v;
            Task<DateTime> t;

            v = DateTime.MinValue;
            t = service.TestDateTime(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = DateTime.Now;
            t = service.TestDateTime(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);

            v = DateTime.MaxValue;
            t = service.TestDateTime(v);
            t.Wait();
            Assert.AreEqual(v, t.Result);
        }
    }
}
