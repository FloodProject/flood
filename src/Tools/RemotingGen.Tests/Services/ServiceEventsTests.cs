using System.Collections.Generic;
using Flood.Remoting.Metadata;
using NUnit.Framework;
using System;

namespace RemotingGen.Tests.Services
{
    [TestFixture]
    public class ServiceEventsTests : ITypeTests
    {
        #region Service
        [Service]
        public interface IService
        {
            [Id(1)]
            event Action EventVoid;

            [Id(2)]
            event Action<byte> EventByte;

            [Id(3)]
            event Action<float> EventFloat;

            [Id(4)]
            event Action<double> EventDouble;

            [Id(5)]
            event Action<short> EventShort;

            [Id(6)]
            event Action<int> EventInt;

            [Id(7)]
            event Action<long> EventLong;

            [Id(8)]
            event Action<string> EventString;

            [Id(9)]
            event Action<Guid> EventGuid;

            [Id(10)]
            event Action<DateTime> EventDateTime;

            [Id(11)]
            event Action<Dictionary<int, string>> EventMap;

            [Id(12)]
            event Action<List<int>> EventList;

            [Id(13)]
            event Action<int[]> EventArray;
        }

        public class Service : IService
        {
            public event Action EventVoid;
            public event Action<byte> EventByte;
            public event Action<float> EventFloat;
            public event Action<double> EventDouble;
            public event Action<short> EventShort;
            public event Action<int> EventInt;
            public event Action<long> EventLong;
            public event Action<string> EventString;
            public event Action<Guid> EventGuid;
            public event Action<DateTime> EventDateTime;
            public event Action<Dictionary<int, string>> EventMap;
            public event Action<List<int>> EventList;
            public event Action<int[]> EventArray;

            public void InvokeEventVoid()
            {
                EventVoid();
            }
            public void InvokeEventByte(byte value)
            {
                EventByte(value);
            }
            public void InvokeEventFloat(float value)
            {
                EventFloat(value);
            }
            public void InvokeEventDouble(double value)
            {
                EventDouble(value);
            }
            public void InvokeEventShort(short value)
            {
                EventShort(value);
            }
            public void InvokeEventInt(int value)
            {
                EventInt(value);
            }
            public void InvokeEventLong(long value)
            {
                EventLong(value);
            }
            public void InvokeEventString(string value)
            {
                EventString(value);
            }
            public void InvokeEventGuid(Guid value)
            {
                EventGuid(value);
            }
            public void InvokeEventDateTime(DateTime value)
            {
                EventDateTime(value);
            }
            public void InvokeEventMap(Dictionary<int, string> value)
            {
                EventMap(value);
            }
            public void InvokeEventList(List<int> value)
            {
                EventList(value);
            }
            public void InvokeEventArray(int[] value)
            {
                EventArray(value);
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
            proxy.EventVoid += () => { wasCalled = true; };
            impl.InvokeEventVoid();
            Assert.IsTrue(wasCalled);
        }

        [Test]
        public void TestByte()
        {
            byte expected;
            byte received = 0;

            expected = byte.MinValue;
            proxy.EventByte += (value) => { received = value; };
            impl.InvokeEventByte(expected);
            Assert.AreEqual(expected, received);

            expected = byte.MaxValue;
            proxy.EventByte += (value) => { received = value; };
            impl.InvokeEventByte(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestFloat()
        {
            float expected;
            float received = 0;

            expected = float.MinValue;
            proxy.EventFloat += (value) => { received = value; };
            impl.InvokeEventFloat(expected);
            Assert.AreEqual(expected, received);

            expected = float.MaxValue;
            proxy.EventFloat += (value) => { received = value; };
            impl.InvokeEventFloat(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestDouble()
        {
            double expected;
            double received = 0;

            expected = double.MinValue;
            proxy.EventDouble += (value) => { received = value; };
            impl.InvokeEventDouble(expected);
            Assert.AreEqual(expected, received);

            expected = double.MaxValue;
            proxy.EventDouble += (value) => { received = value; };
            impl.InvokeEventDouble(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestShort()
        {
            short expected;
            short received = 0;

            expected = short.MinValue;
            proxy.EventShort += (value) => { received = value; };
            impl.InvokeEventShort(expected);
            Assert.AreEqual(expected, received);

            expected = short.MaxValue;
            proxy.EventShort += (value) => { received = value; };
            impl.InvokeEventShort(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestInt()
        {
            int expected;
            int received = 0;

            expected = int.MinValue;
            proxy.EventInt += (value) => { received = value; };
            impl.InvokeEventInt(expected);
            Assert.AreEqual(expected, received);

            expected = int.MaxValue;
            proxy.EventInt += (value) => { received = value; };
            impl.InvokeEventInt(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestLong()
        {
            long expected;
            long received = 0;

            expected = long.MinValue;
            proxy.EventLong += (value) => { received = value; };
            impl.InvokeEventLong(expected);
            Assert.AreEqual(expected, received);

            expected = long.MaxValue;
            proxy.EventLong += (value) => { received = value; };
            impl.InvokeEventLong(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestString()
        {
            string expected = "123456789";
            string received = "";

            proxy.EventString += (value) => { received = value; };
            impl.InvokeEventString(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestGuid()
        {
            Guid expected = new Guid("CB873FC8-6759-4F80-81BE-0EC214BAB26E");
            Guid received = Guid.Empty;

            proxy.EventGuid += (value) => { received = value; };
            impl.InvokeEventGuid(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestDateTime()
        {
            DateTime expected;
            DateTime received = DateTime.Now;

            expected = DateTime.MinValue;
            proxy.EventDateTime += (value) => { received = value; };
            impl.InvokeEventDateTime(expected);
            Assert.AreEqual(expected, received);

            expected = DateTime.MaxValue;
            proxy.EventDateTime += (value) => { received = value; };
            impl.InvokeEventDateTime(expected);
            Assert.AreEqual(expected, received);
        }

        [Test]
        public void TestMap()
        {
            Dictionary<int, string> expected = new Dictionary<int, string>();
            expected.Add(1,"1");
            expected.Add(2,"2");
            expected.Add(3,"3");
            Dictionary<int, string> received = null;
            proxy.EventMap += (value) => { received = value; };
            impl.InvokeEventMap(expected);
            Assert.NotNull(received);
            Assert.AreEqual(expected.Count, received.Count);
            Assert.AreEqual(expected[1], received[1]);
            Assert.AreEqual(expected[2], received[2]);
            Assert.AreEqual(expected[3], received[3]);
        }

        [Test]
        public void TestList()
        {
            List<int> expected = new List<int>{1,2,3};
            List<int> received = null;
            proxy.EventList += (value) => { received = value; };
            impl.InvokeEventList(expected);
            Assert.NotNull(received);
            Assert.AreEqual(expected.Count, received.Count);
            Assert.AreEqual(expected[0], received[0]);
            Assert.AreEqual(expected[1], received[1]);
            Assert.AreEqual(expected[2], received[2]);
        }

        [Test]
        public void TestArray()
        {
            int[] expected = new []{1,2,3};
            int[] received = null;
            proxy.EventArray += (value) => { received = value; };
            impl.InvokeEventArray(expected);
            Assert.NotNull(received);
            Assert.AreEqual(expected.Length, received.Length);
            Assert.AreEqual(expected[0], received[0]);
            Assert.AreEqual(expected[1], received[1]);
            Assert.AreEqual(expected[2], received[2]);
        }
    }
}
