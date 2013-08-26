using Flood.RPC.Metadata;
using NUnit.Framework;
using System;
using System.Threading.Tasks;

namespace RPCGen.Tests.Services
{
    [TestFixture]
    public class ProcedureParamDelegateTests : IPrimitiveTypesTests
    {
        #region Service
        [Service]
        public interface IService
        {
            [Id(1)]
            Task TestVoid(Func<Task> action);

            [Id(2)]
            Task<byte> TestByte(Func<byte, Task<byte>> action, byte value);

            [Id(3)]
            Task<float> TestFloat(Func<float, Task<float>> action, float value);

            [Id(4)]
            Task<double> TestDouble(Func<double, Task<double>> action, double value);

            [Id(5)]
            Task<short> TestShort(Func<short, Task<short>> action, short value);

            [Id(6)]
            Task<int> TestInt(Func<int, Task<int>> action, int value);

            [Id(7)]
            Task<long> TestLong(Func<long, Task<long>> action, long value);

            [Id(8)]
            Task<string> TestString(Func<string, Task<string>> action, string value);

            [Id(9)]
            Task<Guid> TestGuid(Func<Guid, Task<Guid>> action, Guid value);

            [Id(10)]
            Task<DateTime> TestDateTime(Func<DateTime, Task<DateTime>> action, DateTime value);
        }

        public class Service : IService
        {
            public Task TestVoid(Func<Task> action)
            {
                return action.Invoke();
            }

            public Task<byte> TestByte(Func<byte, Task<byte>> action, byte value)
            {
                return action.Invoke(value);
            }

            public Task<float> TestFloat(Func<float, Task<float>> action, float value)
            {
                return action.Invoke(value);
            }

            public Task<double> TestDouble(Func<double, Task<double>> action, double value)
            {
                return action.Invoke(value);
            }

            public Task<short> TestShort(Func<short, Task<short>> action, short value)
            {
                return action.Invoke(value);
            }

            public Task<int> TestInt(Func<int, Task<int>> action, int value)
            {
                return action.Invoke(value);
            }

            public Task<long> TestLong(Func<long, Task<long>> action, long value)
            {
                return action.Invoke(value);
            }

            public Task<string> TestString(Func<string, Task<string>> action, string value)
            {
                return action.Invoke(value);
            }

            public Task<Guid> TestGuid(Func<Guid, Task<Guid>> action, Guid value)
            {
                return action.Invoke(value);
            }

            public Task<DateTime> TestDateTime(Func<DateTime, Task<DateTime>> action, DateTime value)
            {
                return action.Invoke(value);
            }
        }
        #endregion

        Service impl;
        IService proxy;

        [TestFixtureSetUp]
        public void SetUp()
        {
            impl = new Service();
            proxy = Helper.GetProxy<IService>(impl);
        }

        [Test]
        public void TestVoid()
        {
            var wasCalled = false;
            proxy.TestVoid( async () => { wasCalled = true; });
            Assert.IsTrue(wasCalled);
        }

        [Test]
        public void TestByte()
        {
            byte expected;
            Task<byte> task;

            expected = byte.MinValue;
            task = proxy.TestByte(async (value) => { 
                return value; 
            }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);

            expected = byte.MaxValue;
            task = proxy.TestByte(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }

        [Test]
        public void TestFloat()
        {
            float expected;
            Task<float> task;

            expected = float.MinValue;
            task = proxy.TestFloat(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);

            expected = float.MaxValue;
            task = proxy.TestFloat(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }

        [Test]
        public void TestDouble()
        {
            double expected;
            Task<double> task;

            expected = double.MinValue;
            task = proxy.TestDouble(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);

            expected = double.MaxValue;
            task = proxy.TestDouble(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }

        [Test]
        public void TestShort()
        {
            short expected;
            Task<short> task;

            expected = short.MinValue;
            task = proxy.TestShort(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);

            expected = short.MaxValue;
            task = proxy.TestShort(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }

        [Test]
        public void TestInt()
        {
            int expected;
            Task<int> task;

            expected = int.MinValue;
            task = proxy.TestInt(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);

            expected = int.MaxValue;
            task = proxy.TestInt(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }

        [Test]
        public void TestLong()
        {
            long expected;
            Task<long> task;

            expected = long.MinValue;
            task = proxy.TestLong(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);

            expected = long.MaxValue;
            task = proxy.TestLong(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }

        [Test]
        public void TestString()
        {
            string expected = "123456789";
            Task<string> task;

            task = proxy.TestString(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }

        [Test]
        public void TestGuid()
        {
            Guid expected = new Guid("CB873FC8-6759-4F80-81BE-0EC214BAB26E");
            Task<Guid> task;

            task = proxy.TestGuid(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }

        [Test]
        public void TestDateTime()
        {
            DateTime expected;
            Task<DateTime> task;

            expected = DateTime.MinValue;
            task = proxy.TestDateTime(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);

            expected = DateTime.MaxValue;
            task = proxy.TestDateTime(async (value) => { return value; }, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }
    }
}
