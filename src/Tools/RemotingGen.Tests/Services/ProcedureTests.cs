using System.Collections.Generic;
using Flood.Remoting.Metadata;
using NUnit.Framework;
using System;
using System.Threading.Tasks;

namespace RemotingGen.Tests.Services
{
    [TestFixture]
    public class ProcedureTests : ITypeTests
    {
        #region Service
        [Service]
        public interface IService
        {
            [Id(1)]
            Task TestVoid();

            [Id(2)]
            Task<Byte> TestByte([Id(0)]Byte param);

            [Id(3)]
            Task<float> TestFloat([Id(0)]float param);

            [Id(4)]
            Task<double> TestDouble([Id(0)]double param);

            [Id(5)]
            Task<short> TestShort([Id(0)]short param);

            [Id(6)]
            Task<int> TestInt([Id(0)]int param);

            [Id(7)]
            Task<long> TestLong([Id(0)]long param);

            [Id(8)]
            Task<string> TestString([Id(0)]string param);

            [Id(9)]
            Task<Guid> TestGuid([Id(0)]Guid param);

            [Id(10)]
            Task<DateTime> TestDateTime([Id(0)]DateTime param);

            [Id(11)]
            Task<Dictionary<int, string>> TestMap(Dictionary<int, string> param);

            [Id(12)]
            Task<List<int>> TestList(List<int> param);

            [Id(13)]
            Task<int[]> TestArray(int[] param);
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

            public async Task<Dictionary<int, string>> TestMap(Dictionary<int, string> param)
            {
                return param;
            }

            public async Task<List<int>> TestList(List<int> param)
            {
                return param;
            }

            public async Task<int[]> TestArray(int[] param)
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
            byte v;
            Task<byte> t;

            v = byte.MinValue;
            t = service.TestByte(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = byte.MaxValue;
            t = service.TestByte(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestFloat()
        {
            float v;
            Task<float> t;

            v = float.MinValue;
            t = service.TestFloat(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = float.MaxValue;
            t = service.TestFloat(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestDouble()
        {
            double v;
            Task<double> t;

            v = double.MinValue;
            t = service.TestDouble(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = double.MaxValue;
            t = service.TestDouble(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestShort()
        {
            short v;
            Task<short> t;

            v = short.MinValue;
            t = service.TestShort(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = short.MaxValue;
            t = service.TestShort(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestInt()
        {
            int v;
            Task<int> t;

            v = int.MinValue;
            t = service.TestInt(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = int.MaxValue;
            t = service.TestInt(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestLong()
        {
            long v;
            Task<long> t;

            v = long.MinValue;
            t = service.TestLong(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = long.MaxValue;
            t = service.TestLong(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestString()
        {
            string v;
            Task<string> t;

            v = "";
            t = service.TestString(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = "1234567890";
            t = service.TestString(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = "\01234567890";
            t = service.TestString(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestGuid()
        {
            var v = new Guid("CB873FC8-6759-4F80-81BE-0EC214BAB26E");
            Task<Guid> t;
            t = service.TestGuid(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestDateTime()
        {
            DateTime v;
            Task<DateTime> t;

            v = DateTime.MinValue;
            t = service.TestDateTime(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = DateTime.Now;
            t = service.TestDateTime(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);

            v = DateTime.MaxValue;
            t = service.TestDateTime(v);
            Assert.IsTrue(t.Wait(1000));
            Assert.AreEqual(v, t.Result);
        }

        [Test]
        public void TestMap()
        {
            Dictionary<int, string> expected = new Dictionary<int, string>();
            expected.Add(1,"1");
            expected.Add(2,"2");
            expected.Add(3,"3");
            var task = service.TestMap(expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected.Count, task.Result.Count);
            Assert.AreEqual(expected[1], task.Result[1]);
            Assert.AreEqual(expected[2], task.Result[2]);
            Assert.AreEqual(expected[3], task.Result[3]);
        }

        [Test]
        public void TestList()
        {
            List<int> expected = new List<int>{1,2,3};
            var task = service.TestList(expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected.Count, task.Result.Count);
            Assert.AreEqual(expected[0], task.Result[0]);
            Assert.AreEqual(expected[1], task.Result[1]);
            Assert.AreEqual(expected[2], task.Result[2]);
        }

        [Test]
        public void TestArray()
        {
            int[] expected = new []{1,2,3};
            var task = service.TestArray(expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected.Length, task.Result.Length);
            Assert.AreEqual(expected[0], task.Result[0]);
            Assert.AreEqual(expected[1], task.Result[1]);
            Assert.AreEqual(expected[2], task.Result[2]);
        }
    }
}
