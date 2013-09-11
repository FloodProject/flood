

using System;
using Flood;
using NUnit.Framework;

namespace EngineManaged.Tests
{
    [TestFixture]
    class StackArrayTests
    {
        struct Struct
        {
            public int Number;
        }

        [Test]
        public void TestStackArray()
        {
            var gcMemory = GC.GetTotalMemory(false);
            var array1 = StackArray<Struct>.S005;
            Assert.AreEqual(gcMemory, GC.GetTotalMemory(false), "Array construction is allocating on heap.");

            var array2 = InitArray(array1);

            gcMemory = GC.GetTotalMemory(false);
            Assert.AreEqual(0, array1[0].Number);
            Assert.AreEqual(gcMemory, GC.GetTotalMemory(false), "Get index is allocating on heap.");

            for (var i = 0; i < 5; i++)
                Assert.AreEqual(0, array1[i].Number);

            for (var i = 0; i < 5; i++)
                Assert.AreEqual(i, array2[i].Number);
        }

        private StackArray<Struct, S005<Struct>> InitArray(StackArray<Struct, S005<Struct>> array)
        {
            for (var i = 0; i < 5; i++)
            {
                var s = new Struct();
                s.Number = i;

                var gcMemory = GC.GetTotalMemory(false);
                array[i] = s;
                Assert.AreEqual(gcMemory, GC.GetTotalMemory(false), "Set index is allocating on heap.");
            }

            return array;
        }
    }
}
