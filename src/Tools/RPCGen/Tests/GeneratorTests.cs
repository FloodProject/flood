using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using Flood.RPC.Protocol;
using NUnit.Framework;
using NSubstitute;
using Flood.Tools.RPCGen;

namespace RPCGen.Tests
{
    public class TestClass
    {
    }

    public enum TestEnum : byte
    {
    }

    public struct TestStruct
    {
    }


    [TestFixture]
    public class GeneratorTests
    {
        private Generator generator;
        private bool eventFired;

        [SetUp]
        public void Init()
        {
            Options options = new Options();
            generator = new Generator(options);
            eventFired = false;
        }

        [Test]
        public void ThriftClass()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(TestClass));
            Assert.AreEqual(TType.Struct, type) ;
        }

        [Test]
        public void ThriftCollection()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(ICollection<>));
            Assert.AreEqual(TType.Collection, type);
        }

        [Test]
        public void ThriftVoid()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(void));
            Assert.AreEqual(TType.Void, type);
        }

        [Test]
        public void ThriftBool()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(bool));
            Assert.AreEqual(TType.Bool, type);
        }

        [Test]
        public void ThriftShort()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(short));
            Assert.AreEqual(TType.I16, type);
        }

        [Test]
        public void ThriftInt()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(int));
            Assert.AreEqual(TType.I32, type);
        }

        [Test]
        public void ThriftLong()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(long));
            Assert.AreEqual(TType.I64, type);
        }

        [Test]
        public void ThriftDouble()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(double));
            Assert.AreEqual(TType.Double, type);
        }

        [Test]
        public void ThriftString()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(string));
            Assert.AreEqual(TType.String, type);
        }

        [Test]
        public void ThriftEnum()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(TestEnum));
            Assert.AreEqual(TType.Byte, type);
        }

        [Test]
        public void ThriftStruct()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(TestStruct));
            Assert.AreEqual(TType.Struct, type);
        }

        [Test]
        public void ThriftException()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(Exception));
            Assert.AreEqual(TType.Exception, type);
        }

        [Test]
        public void ThriftException2()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(CannotUnloadAppDomainException));
            Assert.AreEqual(TType.Exception, type);
        }

        [Test]
        public void ThriftList()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(IList<>));
            Assert.AreEqual(TType.List, type);
        }

        [Test]
        public void ThriftMap()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(IDictionary<,>));
            Assert.AreEqual(TType.Map, type);
        }

        [Test]
        public void ThriftSet()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(ISet<>));
            Assert.AreEqual(TType.Set, type);
        }

    }
}