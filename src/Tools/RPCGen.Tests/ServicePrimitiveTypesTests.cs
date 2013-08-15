using NUnit.Framework;
using RPCGen.Tests.Interfaces;
using System;
using System.IO;
using System.Reflection;

namespace RPCGen.Tests
{
    [TestFixture]
    class ServicePrimitiveTypesTests : IPrimitiveTypesTests
    {
        public IPrimitiveTypesTests tests;

        [TestFixtureSetUp] 
        public void SetUp()
        {
            tests = (IPrimitiveTypesTests)Helper.GetInstance("RPCGen.Tests.Services.ServicePrimitiveTypesTests");
            tests.SetUp();
        }

        [Test]
        public void TestVoid()
        {
            tests.TestVoid();
        }

        [Test]
        public void TestByte()
        {
            tests.TestByte();
        }

        [Test]
        public void TestFloat()
        {
            tests.TestFloat();
        }

        [Test]
        public void TestDouble()
        {
            tests.TestDouble();
        }

        [Test]
        public void TestShort()
        {
            tests.TestShort();
        }

        [Test]
        public void TestInt()
        {
            tests.TestInt();
        }

        [Test]
        public void TestLong()
        {
            tests.TestLong();
        }

        [Test]
        public void TestString()
        {
            tests.TestString();
        }

        [Test]
        public void TestGuid()
        {
            tests.TestGuid();
        }

        [Test]
        public void TestDateTime()
        {
            tests.TestDateTime();
        }
    }
}
