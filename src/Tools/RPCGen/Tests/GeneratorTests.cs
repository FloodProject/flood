using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;
using Flood.RPC.Metadata;
using Flood.RPC.Protocol;
using NUnit.Framework;
using NSubstitute;
using Flood.Tools.RPCGen;

namespace Flood.Tools.RPCGen.Tests
{

    public abstract class TestAbstract
    {
        [Id(0)]
        private int t1;
        [Id(1)]
        public string t2;
        [Id(2)] 
        public double t3 { get; set; }
        [Id(3)]
        public TestClass t4 { get; private set; }
        
    }

    public class InvalidOperation : Exception
    {
        [Id(1)]
        public int What;
        [Id(2)]
        public string Why;
    }

    public class TestInheritance : TestAbstract
    {
        [Id(4)]// ids must not have values of ids in base classes
        private bool t5;
    }

    public class TestClass
    {
    }

    public class TestClass2
    {
        [Id(0)]
        private long t1;
        [Id(1)]
        public byte t2;
        [Id(2)]
        private char t3 { get; set; }
        [Id(3)]
        public TestAbstract t4 { get; private set; }
    }

    public class TestClass3
    {
        public void GenMethod([Id(0)] int i, [Id(1)] string str, [Id(2)] TestClass cl)
        {
        }

    }

    public class TestClass4
    {
        [Id(0)]
        public Dictionary<int, string> t1;
    }

    public class TestClass5
    {
        [Id(0)]
        public int[] t1;
    }

    public enum TestEnum : byte
    {
    }

    public struct TestStruct
    {
        [Id(0)]
        public int t1;
        [Id(1)]
        public long t2;

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
            Assert.AreEqual(TType.Class, type) ;
        }

        [Test]
        public void ThriftCollection()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(ICollection<>));
            Assert.AreEqual(TType.Collection, type);
        }

        [Test]
        public void ThriftArray()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(byte[]));
            Assert.AreEqual(TType.Array, type);
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

        [Test]
        public void ThriftGuid()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(Guid));
            Assert.AreEqual(TType.Guid, type);
        }

        [Test]
        public void ThriftDateTime()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(DateTime));
            Assert.AreEqual(TType.DateTime, type);
        }

        [Test]
        public void GenerateMessageClass()
        {
            generator.GenerateMessageClass(typeof(TestAbstract));
            string expected = 
                    "[Serializable]\r\n" + 
                    "public class TestAbstractImpl : Base\r\n" + 
                    "{\r\n" + 
                    "    private int _t1;\r\n" + 
                    "    private string _t2;\r\n" + 
                    "    private double _t3;\r\n" +
                    "    private Flood.Tools.RPCGen.Tests.TestClass _t4;\r\n" + 
                    "\r\n" + 
                    "    public int T1\r\n" + 
                    "    {\r\n" + 
                    "        get { return _t1; }\r\n" + 
                    "        set { __isset.t1 = true; this._t1 = value; }\r\n" + 
                    "    }\r\n" + 
                    "\r\n" + 
                    "    public string T2\r\n" + 
                    "    {\r\n" + 
                    "        get { return _t2; }\r\n" + 
                    "        set { __isset.t2 = true; this._t2 = value; }\r\n" + 
                    "    }\r\n" + 
                    "\r\n" + 
                    "    public double T3\r\n" + 
                    "    {\r\n" + 
                    "        get { return _t3; }\r\n" + 
                    "        set { __isset.t3 = true; this._t3 = value; }\r\n" + 
                    "    }\r\n" + 
                    "\r\n" +
                    "    public Flood.Tools.RPCGen.Tests.TestClass T4\r\n" + 
                    "    {\r\n" + 
                    "        get { return _t4; }\r\n" + 
                    "        set { __isset.t4 = true; this._t4 = value; }\r\n" + 
                    "    }\r\n" + 
                    "\r\n" + 
                    "    public Isset __isset;\r\n" + 
                    "\r\n" + 
                    "    [Serializable]\r\n" + 
                    "    public struct Isset\r\n" + 
                    "    {\r\n" + 
                    "        public bool t1;\r\n" + 
                    "        public bool t2;\r\n" + 
                    "        public bool t3;\r\n" + 
                    "        public bool t4;\r\n" + 
                    "    }\r\n" + 
                    "\r\n" + 
                    "    public TestAbstractImpl()\r\n" + 
                    "    {\r\n" + 
                    "    }\r\n" + 
                    "\r\n" +  
                    "    public void Read(Serializer iprot)\r\n" + 
                    "    {\r\n" + 
                    "        iprot.ReadStructBegin();\r\n" + 
                    "        while (true)\r\n" + 
                    "        {\r\n" + 
                    "            var field = iprot.ReadFieldBegin();\r\n" + 
                    "            if (field.Type == TType.Stop)\r\n" + 
                    "                break;\r\n" + 
                    "\r\n" + 
                    "            switch (field.ID)\r\n" + 
                    "            {\r\n" + 
                    "                case 0:\r\n" + 
                    "                    if (field.Type == TType.I32)\r\n" + 
                    "                    {\r\n" + 
                    "                        T1 = iprot.ReadI32();\r\n" + 
                    "                    }\r\n" + 
                    "                    else\r\n" + 
                    "                    {\r\n" + 
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" + 
                    "                    }\r\n" + 
                    "                    break;\r\n" + 
                    "                case 1:\r\n" + 
                    "                    if (field.Type == TType.String)\r\n" + 
                    "                    {\r\n" + 
                    "                        T2 = iprot.ReadString();\r\n" + 
                    "                    }\r\n" + 
                    "                    else\r\n" + 
                    "                    {\r\n" + 
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" + 
                    "                    }\r\n" + 
                    "                    break;\r\n" + 
                    "                case 2:\r\n" + 
                    "                    if (field.Type == TType.Double)\r\n" + 
                    "                    {\r\n" + 
                    "                        T3 = iprot.ReadDouble();\r\n" + 
                    "                    }\r\n" + 
                    "                    else\r\n" + 
                    "                    {\r\n" + 
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" + 
                    "                    }\r\n" + 
                    "                    break;\r\n" + 
                    "                case 3:\r\n" +
                    "                    if (field.Type == TType.Class)\r\n" + 
                    "                    {\r\n" + 
                    "                        var T4Impl = new TestClassImpl();\r\n" + 
                    "                        T4Impl.Read(iprot);\r\n" +
                    "                        T4 = new Flood.Tools.RPCGen.Tests.TestClass()\r\n" + 
                    "                        {\r\n" + 
                    "                        };\r\n" + 
                    "                    }\r\n" + 
                    "                    else\r\n" + 
                    "                    {\r\n" + 
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" + 
                    "                    }\r\n" + 
                    "                    break;\r\n" + 
                    "                default:\r\n" + 
                    "                    ProtocolUtil.Skip(iprot, field.Type);\r\n" + 
                    "                    break;\r\n" + 
                    "            }\r\n" + 
                    "            iprot.ReadFieldEnd();\r\n" + 
                    "        }\r\n" + 
                    "        iprot.ReadStructEnd();\r\n" + 
                    "    }\r\n" + 
                    "\r\n" + 
                    "    public void Write(Serializer oprot)\r\n" + 
                    "    {\r\n" + 
                    "        var struc = new Struct(\"TestAbstract_args\");\r\n" + 
                    "        oprot.WriteStructBegin(struc);\r\n" + 
                    "        var field = new Field();\r\n" + 
                    "        if (__isset.t1)\r\n" + 
                    "        {\r\n" + 
                    "            field.Name = \"t1\";\r\n" + 
                    "            field.Type = TType.I32;\r\n" + 
                    "            field.ID = 0;\r\n" + 
                    "            oprot.WriteFieldBegin(field);\r\n" + 
                    "            oprot.WriteI32(_t1);\r\n" + 
                    "            oprot.WriteFieldEnd();\r\n" + 
                    "        }\r\n" + 
                    "        if (T2 != null && __isset.t2)\r\n" + 
                    "        {\r\n" + 
                    "            field.Name = \"t2\";\r\n" + 
                    "            field.Type = TType.String;\r\n" + 
                    "            field.ID = 1;\r\n" + 
                    "            oprot.WriteFieldBegin(field);\r\n" + 
                    "            oprot.WriteString(_t2);\r\n" + 
                    "            oprot.WriteFieldEnd();\r\n" + 
                    "        }\r\n" + 
                    "        if (__isset.t3)\r\n" + 
                    "        {\r\n" + 
                    "            field.Name = \"t3\";\r\n" + 
                    "            field.Type = TType.Double;\r\n" + 
                    "            field.ID = 2;\r\n" + 
                    "            oprot.WriteFieldBegin(field);\r\n" + 
                    "            oprot.WriteDouble(_t3);\r\n" + 
                    "            oprot.WriteFieldEnd();\r\n" + 
                    "        }\r\n" +
                    "        if (T4 != null && __isset.t4)\r\n" + 
                    "        {\r\n" + 
                    "            field.Name = \"t4\";\r\n" +
                    "            field.Type = TType.Class;\r\n" + 
                    "            field.ID = 3;\r\n" + 
                    "            oprot.WriteFieldBegin(field);\r\n" + 
                    "            var T4Impl = new TestClassImpl()\r\n" + 
                    "            {\r\n" + 
                    "            };\r\n" + 
                    "            T4Impl.Write(oprot);\r\n" + 
                    "            oprot.WriteFieldEnd();\r\n" + 
                    "        }\r\n" + 
                    "        oprot.WriteFieldStop();\r\n" + 
                    "        oprot.WriteStructEnd();\r\n" +
                    "    }\r\n" + 
                    "}\r\n";

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateMessageClass2()
        {
            generator.GenerateMessageClass(typeof(TestStruct));
            string expected = 
                    "[Serializable]\r\n" +
                    "public class TestStructImpl : Base\r\n" +
                    "{\r\n" +
                    "    private int _t1;\r\n" +
                    "    private long _t2;\r\n" +
                    "\r\n" +
                    "    public int T1\r\n" +
                    "    {\r\n" +
                    "        get { return _t1; }\r\n" +
                    "        set { __isset.t1 = true; this._t1 = value; }\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public long T2\r\n" +
                    "    {\r\n" +
                    "        get { return _t2; }\r\n" +
                    "        set { __isset.t2 = true; this._t2 = value; }\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public Isset __isset;\r\n" +
                    "\r\n" +
                    "    [Serializable]\r\n" +
                    "    public struct Isset\r\n" +
                    "    {\r\n" +
                    "        public bool t1;\r\n" +
                    "        public bool t2;\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public TestStructImpl()\r\n" +
                    "    {\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public void Read(Serializer iprot)\r\n" +
                    "    {\r\n" +
                    "        iprot.ReadStructBegin();\r\n" +
                    "        while (true)\r\n" +
                    "        {\r\n" +
                    "            var field = iprot.ReadFieldBegin();\r\n" +
                    "            if (field.Type == TType.Stop)\r\n" +
                    "                break;\r\n" +
                    "\r\n" +
                    "            switch (field.ID)\r\n" +
                    "            {\r\n" +
                    "                case 0:\r\n" +
                    "                    if (field.Type == TType.I32)\r\n" +
                    "                    {\r\n" +
                    "                        T1 = iprot.ReadI32();\r\n" +
                    "                    }\r\n" +
                    "                    else\r\n" +
                    "                    {\r\n" +
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                    "                    }\r\n" +
                    "                    break;\r\n" +
                    "                case 1:\r\n" +
                    "                    if (field.Type == TType.I64)\r\n" +
                    "                    {\r\n" +
                    "                        T2 = iprot.ReadI64();\r\n" +
                    "                    }\r\n" +
                    "                    else\r\n" +
                    "                    {\r\n" +
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                    "                    }\r\n" +
                    "                    break;\r\n" +
                    "                default:\r\n" +
                    "                    ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                    "                    break;\r\n" +
                    "            }\r\n" +
                    "            iprot.ReadFieldEnd();\r\n" +
                    "        }\r\n" +
                    "        iprot.ReadStructEnd();\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public void Write(Serializer oprot)\r\n" +
                    "    {\r\n" +
                    "        var struc = new Struct(\"TestStruct_args\");\r\n" +
                    "        oprot.WriteStructBegin(struc);\r\n" +
                    "        var field = new Field();\r\n" +
                    "        if (__isset.t1)\r\n" +
                    "        {\r\n" +
                    "            field.Name = \"t1\";\r\n" +
                    "            field.Type = TType.I32;\r\n" +
                    "            field.ID = 0;\r\n" +
                    "            oprot.WriteFieldBegin(field);\r\n" +
                    "            oprot.WriteI32(_t1);\r\n" +
                    "            oprot.WriteFieldEnd();\r\n" +
                    "        }\r\n" +
                    "        if (__isset.t2)\r\n" +
                    "        {\r\n" +
                    "            field.Name = \"t2\";\r\n" +
                    "            field.Type = TType.I64;\r\n" +
                    "            field.ID = 1;\r\n" +
                    "            oprot.WriteFieldBegin(field);\r\n" +
                    "            oprot.WriteI64(_t2);\r\n" +
                    "            oprot.WriteFieldEnd();\r\n" +
                    "        }\r\n" +
                    "        oprot.WriteFieldStop();\r\n" +
                    "        oprot.WriteStructEnd();\r\n" +
                    "    }\r\n" +
                    "}\r\n";

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateMessageClass3()
        {
            generator.GenerateMessageClass(typeof(TestInheritance));
            string expected =
                   "[Serializable]\r\n" +
                   "public class TestInheritanceImpl : Base\r\n" +
                   "{\r\n" +
                   "    private bool _t5;\r\n" +
                   "    private int _t1;\r\n" +
                   "    private string _t2;\r\n" +
                   "    private double _t3;\r\n" +
                   "    private Flood.Tools.RPCGen.Tests.TestClass _t4;\r\n" +
                   "\r\n" +
                   "    public bool T5\r\n" +
                   "    {\r\n" +
                   "        get { return _t5; }\r\n" +
                   "        set { __isset.t5 = true; this._t5 = value; }\r\n" +
                   "    }\r\n" +
                   "\r\n" +
                   "    public int T1\r\n" +
                   "    {\r\n" +
                   "        get { return _t1; }\r\n" +
                   "        set { __isset.t1 = true; this._t1 = value; }\r\n" +
                   "    }\r\n" +
                   "\r\n" +
                   "    public string T2\r\n" +
                   "    {\r\n" +
                   "        get { return _t2; }\r\n" +
                   "        set { __isset.t2 = true; this._t2 = value; }\r\n" +
                   "    }\r\n" +
                   "\r\n" +
                   "    public double T3\r\n" +
                   "    {\r\n" +
                   "        get { return _t3; }\r\n" +
                   "        set { __isset.t3 = true; this._t3 = value; }\r\n" +
                   "    }\r\n" +
                   "\r\n" +
                   "    public Flood.Tools.RPCGen.Tests.TestClass T4\r\n" +
                   "    {\r\n" +
                   "        get { return _t4; }\r\n" +
                   "        set { __isset.t4 = true; this._t4 = value; }\r\n" +
                   "    }\r\n" +
                   "\r\n" +
                   "    public Isset __isset;\r\n" +
                   "\r\n" +
                   "    [Serializable]\r\n" +
                   "    public struct Isset\r\n" +
                   "    {\r\n" +
                   "        public bool t5;\r\n" +
                   "        public bool t1;\r\n" +
                   "        public bool t2;\r\n" +
                   "        public bool t3;\r\n" +
                   "        public bool t4;\r\n" +
                   "    }\r\n" +
                   "\r\n" +
                   "    public TestInheritanceImpl()\r\n" +
                   "    {\r\n" +
                   "    }\r\n" +
                   "\r\n" +
                   "    public void Read(Serializer iprot)\r\n" +
                   "    {\r\n" +
                   "        iprot.ReadStructBegin();\r\n" +
                   "        while (true)\r\n" +
                   "        {\r\n" +
                   "            var field = iprot.ReadFieldBegin();\r\n" +
                   "            if (field.Type == TType.Stop)\r\n" +
                   "                break;\r\n" +
                   "\r\n" +
                   "            switch (field.ID)\r\n" +
                   "            {\r\n" +
                   "                case 4:\r\n" +
                   "                    if (field.Type == TType.Bool)\r\n" +
                   "                    {\r\n" +
                   "                        T5 = iprot.ReadBool();\r\n" +
                   "                    }\r\n" +
                   "                    else\r\n" +
                   "                    {\r\n" +
                   "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                   "                    }\r\n" +
                   "                    break;\r\n" +
                   "                case 0:\r\n" +
                   "                    if (field.Type == TType.I32)\r\n" +
                   "                    {\r\n" +
                   "                        T1 = iprot.ReadI32();\r\n" +
                   "                    }\r\n" +
                   "                    else\r\n" +
                   "                    {\r\n" +
                   "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                   "                    }\r\n" +
                   "                    break;\r\n" +
                   "                case 1:\r\n" +
                   "                    if (field.Type == TType.String)\r\n" +
                   "                    {\r\n" +
                   "                        T2 = iprot.ReadString();\r\n" +
                   "                    }\r\n" +
                   "                    else\r\n" +
                   "                    {\r\n" +
                   "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                   "                    }\r\n" +
                   "                    break;\r\n" +
                   "                case 2:\r\n" +
                   "                    if (field.Type == TType.Double)\r\n" +
                   "                    {\r\n" +
                   "                        T3 = iprot.ReadDouble();\r\n" +
                   "                    }\r\n" +
                   "                    else\r\n" +
                   "                    {\r\n" +
                   "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                   "                    }\r\n" +
                   "                    break;\r\n" +
                   "                case 3:\r\n" +
                   "                    if (field.Type == TType.Class)\r\n" +
                   "                    {\r\n" +
                   "                        var T4Impl = new TestClassImpl();\r\n" +
                   "                        T4Impl.Read(iprot);\r\n" +
                   "                        T4 = new Flood.Tools.RPCGen.Tests.TestClass()\r\n" +
                   "                        {\r\n" +
                   "                        };\r\n" +
                   "                    }\r\n" +
                   "                    else\r\n" +
                   "                    {\r\n" +
                   "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                   "                    }\r\n" +
                   "                    break;\r\n" +
                   "                default:\r\n" +
                   "                    ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                   "                    break;\r\n" +
                   "            }\r\n" +
                   "            iprot.ReadFieldEnd();\r\n" +
                   "        }\r\n" +
                   "        iprot.ReadStructEnd();\r\n" +
                   "    }\r\n" +
                   "\r\n" +
                   "    public void Write(Serializer oprot)\r\n" +
                   "    {\r\n" +
                   "        var struc = new Struct(\"TestInheritance_args\");\r\n" +
                   "        oprot.WriteStructBegin(struc);\r\n" +
                   "        var field = new Field();\r\n" +
                   "        if (__isset.t5)\r\n" +
                   "        {\r\n" +
                   "            field.Name = \"t5\";\r\n" +
                   "            field.Type = TType.Bool;\r\n" +
                   "            field.ID = 4;\r\n" +
                   "            oprot.WriteFieldBegin(field);\r\n" +
                   "            oprot.WriteBool(_t5);\r\n" +
                   "            oprot.WriteFieldEnd();\r\n" +
                   "        }\r\n" +
                   "        if (__isset.t1)\r\n" +
                   "        {\r\n" +
                   "            field.Name = \"t1\";\r\n" +
                   "            field.Type = TType.I32;\r\n" +
                   "            field.ID = 0;\r\n" +
                   "            oprot.WriteFieldBegin(field);\r\n" +
                   "            oprot.WriteI32(_t1);\r\n" +
                   "            oprot.WriteFieldEnd();\r\n" +
                   "        }\r\n" +
                   "        if (T2 != null && __isset.t2)\r\n" +
                   "        {\r\n" +
                   "            field.Name = \"t2\";\r\n" +
                   "            field.Type = TType.String;\r\n" +
                   "            field.ID = 1;\r\n" +
                   "            oprot.WriteFieldBegin(field);\r\n" +
                   "            oprot.WriteString(_t2);\r\n" +
                   "            oprot.WriteFieldEnd();\r\n" +
                   "        }\r\n" +
                   "        if (__isset.t3)\r\n" +
                   "        {\r\n" +
                   "            field.Name = \"t3\";\r\n" +
                   "            field.Type = TType.Double;\r\n" +
                   "            field.ID = 2;\r\n" +
                   "            oprot.WriteFieldBegin(field);\r\n" +
                   "            oprot.WriteDouble(_t3);\r\n" +
                   "            oprot.WriteFieldEnd();\r\n" +
                   "        }\r\n" +
                   "        if (T4 != null && __isset.t4)\r\n" +
                   "        {\r\n" +
                   "            field.Name = \"t4\";\r\n" +
                   "            field.Type = TType.Class;\r\n" +
                   "            field.ID = 3;\r\n" +
                   "            oprot.WriteFieldBegin(field);\r\n" +
                   "            var T4Impl = new TestClassImpl()\r\n" +
                   "            {\r\n" +
                   "            };\r\n" +
                   "            T4Impl.Write(oprot);\r\n" +
                   "            oprot.WriteFieldEnd();\r\n" +
                   "        }\r\n" +
                   "        oprot.WriteFieldStop();\r\n" +
                   "        oprot.WriteStructEnd();\r\n" +
                   "    }\r\n" +
                   "}\r\n";



            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateMessageClass4()
        {

            generator.GenerateMessageClass(typeof(TestClass4));
            string expected =
                    "[Serializable]\r\n"+
                    "public class TestClass4Impl : Base\r\n"+
                    "{\r\n"+
                    "    private IDictionary<System.Int32, System.String> _t1;\r\n"+
                    "\r\n"+
                    "    public IDictionary<System.Int32, System.String> T1\r\n"+
                    "    {\r\n"+
                    "        get { return _t1; }\r\n"+
                    "        set { __isset.t1 = true; this._t1 = value; }\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public Isset __isset;\r\n"+
                    "\r\n"+
                    "    [Serializable]\r\n"+
                    "    public struct Isset\r\n"+
                    "    {\r\n"+
                    "        public bool t1;\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public TestClass4Impl()\r\n"+
                    "    {\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public void Read(Serializer iprot)\r\n"+
                    "    {\r\n"+
                    "        iprot.ReadStructBegin();\r\n"+
                    "        while (true)\r\n"+
                    "        {\r\n"+
                    "            var field = iprot.ReadFieldBegin();\r\n"+
                    "            if (field.Type == TType.Stop)\r\n"+
                    "                break;\r\n"+
                    "\r\n"+
                    "            switch (field.ID)\r\n"+
                    "            {\r\n"+
                    "                case 0:\r\n"+
                    "                    if (field.Type == TType.Map)\r\n"+
                    "                    {\r\n"+
                    "                        T1 = new Dictionary<System.Int32, System.String>();\r\n"+
                    "                        var _set0 = iprot.ReadMapBegin();\r\n"+
                    "                        for (var _i1 = 0; _i1 < _set0.Count; ++_i1)\r\n"+
                    "                        {\r\n"+
                    "                            var _elem2 = iprot.ReadI32();\r\n"+
                    "                            var _elem3 = iprot.ReadString();\r\n"+
                    "                            T1.Add(_elem2, _elem3);\r\n"+
                    "                        }\r\n"+
                    "                        iprot.ReadMapEnd();\r\n"+
                    "                    }\r\n"+
                    "                    else\r\n"+
                    "                    {\r\n"+
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n"+
                    "                    }\r\n"+
                    "                    break;\r\n"+
                    "                default:\r\n"+
                    "                    ProtocolUtil.Skip(iprot, field.Type);\r\n"+
                    "                    break;\r\n"+
                    "            }\r\n"+
                    "            iprot.ReadFieldEnd();\r\n"+
                    "        }\r\n"+
                    "        iprot.ReadStructEnd();\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public void Write(Serializer oprot)\r\n"+
                    "    {\r\n"+
                    "        var struc = new Struct(\"TestClass4_args\");\r\n"+
                    "        oprot.WriteStructBegin(struc);\r\n"+
                    "        var field = new Field();\r\n"+
                    "        if (T1 != null && __isset.t1)\r\n"+
                    "        {\r\n"+
                    "            field.Name = \"t1\";\r\n"+
                    "            field.Type = TType.Map;\r\n"+
                    "            field.ID = 0;\r\n"+
                    "            oprot.WriteFieldBegin(field);\r\n"+
                    "            oprot.WriteMapBegin(new TMap(TType.I32, TType.String, T1.Count));\r\n" +
                    "            foreach (var _iter6 in T1)\r\n"+
                    "            {\r\n"+
                    "                oprot.WriteI32(_iter6.Key);\r\n"+
                    "                oprot.WriteString(_iter6.Value);\r\n"+
                    "            }\r\n"+
                    "            oprot.WriteMapEnd();\r\n"+
                    "            oprot.WriteFieldEnd();\r\n"+
                    "        }\r\n"+
                    "        oprot.WriteFieldStop();\r\n"+
                    "        oprot.WriteStructEnd();\r\n"+
                    "    }\r\n"+
                    "}\r\n";

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateMessageClass5()
        { 

            generator.GenerateMessageClass(typeof(TestClass5));
            string expected =
                    "[Serializable]\r\n" +
                    "public class TestClass5Impl : Base\r\n" +
                    "{\r\n" +
                    "    private int[] _t1;\r\n" +
                    "\r\n" +
                    "    public int[] T1\r\n" +
                    "    {\r\n" +
                    "        get { return _t1; }\r\n" +
                    "        set { __isset.t1 = true; this._t1 = value; }\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public Isset __isset;\r\n" +
                    "\r\n" +
                    "    [Serializable]\r\n" +
                    "    public struct Isset\r\n" +
                    "    {\r\n" +
                    "        public bool t1;\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public TestClass5Impl()\r\n" +
                    "    {\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public void Read(Serializer iprot)\r\n" +
                    "    {\r\n" +
                    "        iprot.ReadStructBegin();\r\n" +
                    "        while (true)\r\n" +
                    "        {\r\n" +
                    "            var field = iprot.ReadFieldBegin();\r\n" +
                    "            if (field.Type == TType.Stop)\r\n" +
                    "                break;\r\n" +
                    "\r\n" +
                    "            switch (field.ID)\r\n" +
                    "            {\r\n" +
                    "                case 0:\r\n" +
                    "                    if (field.Type == TType.Array)\r\n" +
                    "                    {\r\n" +
                    "                        var _array0 = iprot.ReadArrayBegin();\r\n" +
                    "                        T1 = new int[_array0.Count];\r\n" +
                    "                        for (var _i1 = 0; _i1 < _array0.Count; ++_i1)\r\n" +
                    "                        {\r\n" +
                    "                            T1[_i1] = iprot.ReadI32();\r\n" +
                    "                        }\r\n" +
                    "                        iprot.ReadArrayEnd();\r\n" +
                    "                    }\r\n" +
                    "                    else\r\n" +
                    "                    {\r\n" +
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                    "                    }\r\n" +
                    "                    break;\r\n" +
                    "                default:\r\n" +
                    "                    ProtocolUtil.Skip(iprot, field.Type);\r\n" +
                    "                    break;\r\n" +
                    "            }\r\n" +
                    "            iprot.ReadFieldEnd();\r\n" +
                    "        }\r\n" +
                    "        iprot.ReadStructEnd();\r\n" +
                    "    }\r\n" +
                    "\r\n" +
                    "    public void Write(Serializer oprot)\r\n" +
                    "    {\r\n" +
                    "        var struc = new Struct(\"TestClass5_args\");\r\n" +
                    "        oprot.WriteStructBegin(struc);\r\n" +
                    "        var field = new Field();\r\n" +
                    "        if (T1 != null && __isset.t1)\r\n" +
                    "        {\r\n" +
                    "            field.Name = \"t1\";\r\n" +
                    "            field.Type = TType.Array;\r\n" +
                    "            field.ID = 0;\r\n" +
                    "            oprot.WriteFieldBegin(field);\r\n" +
                    "            oprot.WriteArrayBegin(new TArray(TType.Array, T1.Length));\r\n" +
                    "            foreach (var _iter3 in T1)\r\n" +
                    "            {\r\n" +
                    "                oprot.WriteI32(_iter3);\r\n" +
                    "            }\r\n" +
                    "            oprot.WriteArrayEnd();\r\n" +
                    "            oprot.WriteFieldEnd();\r\n" +
                    "        }\r\n" +
                    "        oprot.WriteFieldStop();\r\n" +
                    "        oprot.WriteStructEnd();\r\n" +
                    "    }\r\n" +
                    "}\r\n";

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateExceptionClass()
        {
            generator.GenerateMessageClass(typeof(InvalidOperation));
            string expected = 
                    "[Serializable]\r\n"+
                    "public class InvalidOperationImpl : Base\r\n"+
                    "{\r\n" +                    "    private int _What;\r\n"+
                    "    private string _Why;\r\n"+
                    "\r\n"+
                    "    public int What\r\n"+
                    "    {\r\n"+
                    "        get { return _What; }\r\n"+
                    "        set { __isset.What = true; this._What = value; }\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public string Why\r\n"+
                    "    {\r\n"+
                    "        get { return _Why; }\r\n"+
                    "        set { __isset.Why = true; this._Why = value; }\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public Isset __isset;\r\n"+
                    "\r\n"+
                    "    [Serializable]\r\n"+
                    "    public struct Isset\r\n"+
                    "    {\r\n"+
                    "        public bool What;\r\n"+
                    "        public bool Why;\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public InvalidOperationImpl()\r\n"+
                    "    {\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public void Read(Serializer iprot)\r\n"+
                    "    {\r\n"+
                    "        iprot.ReadStructBegin();\r\n"+
                    "        while (true)\r\n"+
                    "        {\r\n"+
                    "            var field = iprot.ReadFieldBegin();\r\n"+
                    "            if (field.Type == TType.Stop)\r\n"+
                    "                break;\r\n"+
                    "\r\n"+
                    "            switch (field.ID)\r\n"+
                    "            {\r\n"+
                    "                case 1:\r\n"+
                    "                    if (field.Type == TType.I32)\r\n"+
                    "                    {\r\n"+
                    "                        What = iprot.ReadI32();\r\n"+
                    "                    }\r\n"+
                    "                    else\r\n"+
                    "                    {\r\n"+
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n"+
                    "                    }\r\n"+
                    "                    break;\r\n"+
                    "                case 2:\r\n"+
                    "                    if (field.Type == TType.String)\r\n"+
                    "                    {\r\n"+
                    "                        Why = iprot.ReadString();\r\n"+
                    "                    }\r\n"+
                    "                    else\r\n"+
                    "                    {\r\n"+
                    "                        ProtocolUtil.Skip(iprot, field.Type);\r\n"+
                    "                    }\r\n"+
                    "                    break;\r\n"+
                    "                default:\r\n"+
                    "                    ProtocolUtil.Skip(iprot, field.Type);\r\n"+
                    "                    break;\r\n"+
                    "            }\r\n"+
                    "            iprot.ReadFieldEnd();\r\n"+
                    "        }\r\n"+
                    "        iprot.ReadStructEnd();\r\n"+
                    "    }\r\n"+
                    "\r\n"+
                    "    public void Write(Serializer oprot)\r\n"+
                    "    {\r\n"+
                    "        var struc = new Struct(\"InvalidOperation_args\");\r\n"+
                    "        oprot.WriteStructBegin(struc);\r\n"+
                    "        var field = new Field();\r\n"+
                    "        if (__isset.What)\r\n"+
                    "        {\r\n"+
                    "            field.Name = \"What\";\r\n"+
                    "            field.Type = TType.I32;\r\n"+
                    "            field.ID = 1;\r\n"+
                    "            oprot.WriteFieldBegin(field);\r\n"+
                    "            oprot.WriteI32(_What);\r\n"+
                    "            oprot.WriteFieldEnd();\r\n"+
                    "        }\r\n"+
                    "        if (Why != null && __isset.Why)\r\n"+
                    "        {\r\n"+
                    "            field.Name = \"Why\";\r\n"+
                    "            field.Type = TType.String;\r\n"+
                    "            field.ID = 2;\r\n"+
                    "            oprot.WriteFieldBegin(field);\r\n"+
                    "            oprot.WriteString(_Why);\r\n"+
                    "            oprot.WriteFieldEnd();\r\n"+
                    "        }\r\n"+
                    "        oprot.WriteFieldStop();\r\n"+
                    "        oprot.WriteStructEnd();\r\n"+
                    "    }\r\n"+
                    "}\r\n";	

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateService()
        {
        }

        [Test]
        public void GenerateParameterList()
        {
            string expected = "int i, string str, Flood.Tools.RPCGen.Tests.TestClass cl";
            generator.GenerateParameterList(typeof(TestClass3).GetMethod("GenMethod").GetParameters());
    
            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListSerialize()
        {
            string expected =
                "oprot.WriteListBegin(new TList(TType.List, Lst.Count));\r\n"+
                "foreach (var _iter0 in Lst)\r\n"+
                "{\r\n"+
                "    oprot.WriteI32(_iter0);\r\n" +
                "}\r\n"+
                "oprot.WriteListEnd();\r\n";

            generator.GenerateListSerialize(new Generator.Parameter("Lst", typeof(List<int>),0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListSerialize2()
        {
            string expected =
                "oprot.WriteListBegin(new TList(TType.List, Lst.Count));\r\n" +
                "foreach (var _iter0 in Lst)\r\n" +
                "{\r\n" +
                "    oprot.WriteI64(_iter0);\r\n" +
                "}\r\n" +
                "oprot.WriteListEnd();\r\n";

            generator.GenerateListSerialize(new Generator.Parameter("Lst", typeof(List<long>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListSerialize3()
        {
            string expected =
                "oprot.WriteListBegin(new TList(TType.List, Lst.Count));\r\n" +
                "foreach (var _iter0 in Lst)\r\n" +
                "{\r\n" +
                "    oprot.WriteDouble(_iter0);\r\n" +
                "}\r\n" +
                "oprot.WriteListEnd();\r\n";

            generator.GenerateListSerialize(new Generator.Parameter("Lst", typeof(List<double>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListSerialize4()
        {
            string expected =
                "oprot.WriteListBegin(new TList(TType.List, Lst.Count));\r\n" +
                "foreach (var _iter0 in Lst)\r\n" +
                "{\r\n" +
                "    oprot.WriteString(_iter0.ToString());\r\n" +
                "}\r\n" +
                "oprot.WriteListEnd();\r\n";

            generator.GenerateListSerialize(new Generator.Parameter("Lst", typeof(List<Guid>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListSerialize5()
        {
            string expected =
                "oprot.WriteListBegin(new TList(TType.List, Lst.Count));\r\n" +
                "foreach (var _iter0 in Lst)\r\n" +
                "{\r\n" +
                "    oprot.WriteI64(_iter0.Ticks);\r\n" +
                "}\r\n" +
                "oprot.WriteListEnd();\r\n";

            generator.GenerateListSerialize(new Generator.Parameter("Lst", typeof(List<DateTime>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListSerialize6()
        {
            string expected =
                "oprot.WriteListBegin(new TList(TType.List, Lst.Count));\r\n" +
                "foreach (var _iter0 in Lst)\r\n" +
                "{\r\n" +
                "    var _elem1 = new TestClass2Impl()\r\n" +
                "    {\r\n" +
                "        t1 = _iter0.t1,\r\n" +
                "        t2 = _iter0.t2,\r\n" +
                "        t3 = _iter0.t3,\r\n" +
                "        t4 = _iter0.t4,\r\n" +
                "    };\r\n" +
                "    _elem1.Write(oprot);\r\n" +
                "}\r\n" +
                "oprot.WriteListEnd();\r\n";

            generator.GenerateListSerialize(new Generator.Parameter("Lst", typeof(List<TestClass2>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListSerialize7()
        {
            string expected =
                "oprot.WriteListBegin(new TList(TType.List, Lst.Count));\r\n" +
                "foreach (var _iter0 in Lst)\r\n" +
                "{\r\n" +
                "    oprot.WriteString(_iter0);\r\n" +
                "}\r\n" +
                "oprot.WriteListEnd();\r\n";

            generator.GenerateListSerialize(new Generator.Parameter("Lst", typeof(List<string>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListDeserialize()
        {
            string expected =
                "Lst = new List<System.Double>();\r\n" +
                "var _list0 = iprot.ReadListBegin();\r\n" +
                "for (var _i1 = 0; _i1 < _list0.Count; ++_i1)\r\n" +
                "{\r\n" +
                "    Lst.Add(iprot.ReadDouble());\r\n" +
                "}\r\n" +
                "iprot.ReadListEnd();\r\n";

            generator.GenerateListDeserialize(new Generator.Parameter("Lst", typeof(List<double>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListDeserialize2()
        {
            string expected =
                "Lst = new List<System.Guid>();\r\n" +
                "var _list0 = iprot.ReadListBegin();\r\n" +
                "for (var _i1 = 0; _i1 < _list0.Count; ++_i1)\r\n" +
                "{\r\n" +
                "    Lst.Add(new System.Guid(iprot.ReadString()));\r\n" +
                "}\r\n" +
                "iprot.ReadListEnd();\r\n";

            generator.GenerateListDeserialize(new Generator.Parameter("Lst", typeof(List<Guid>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListDeserialize3()
        {
            string expected =
                "Lst = new List<System.DateTime>();\r\n" +
                "var _list0 = iprot.ReadListBegin();\r\n" +
                "for (var _i1 = 0; _i1 < _list0.Count; ++_i1)\r\n" +
                "{\r\n" +
                "    Lst.Add(new System.DateTime(iprot.ReadI64()));\r\n" +
                "}\r\n" +
                "iprot.ReadListEnd();\r\n";

            generator.GenerateListDeserialize(new Generator.Parameter("Lst", typeof(List<DateTime>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateListDeserialize4()
        {
            string expected =
                "Lst = new List<Flood.Tools.RPCGen.Tests.TestClass2>();\r\n" +
                "var _list0 = iprot.ReadListBegin();\r\n" +
                "for (var _i1 = 0; _i1 < _list0.Count; ++_i1)\r\n" +
                "{\r\n" +
                "    var _elem2 = new TestClass2Impl();\r\n" +
                "    _elem2.Read(iprot);\r\n" +
                "    var _elem3 = new Flood.Tools.RPCGen.Tests.TestClass2()\r\n" +
                "    {\r\n" +
                "        t1 = _elem2.t1,\r\n" +
                "        t2 = _elem2.t2,\r\n" +
                "        t3 = _elem2.t3,\r\n" +
                "        t4 = _elem2.t4,\r\n" +
                "    };\r\n" +
                "    Lst.Add(_elem3);\r\n" +
                "}\r\n" +
                "iprot.ReadListEnd();\r\n";

            generator.GenerateListDeserialize(new Generator.Parameter("Lst", typeof(List<TestClass2>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateMapSerialize()
        {
            string expected =
                "oprot.WriteMapBegin(new TMap(TType.I32, TType.String, Map.Count));\r\n" +
                "foreach (var _iter0 in Map)\r\n" +
                "{\r\n" +
                "    oprot.WriteI32(_iter0.Key);\r\n" +
                "    oprot.WriteString(_iter0.Value);\r\n" +
                "}\r\n" +
                "oprot.WriteMapEnd();\r\n";

            generator.GenerateMapSerialize(new Generator.Parameter("Map", typeof(Dictionary<int, string>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateMapSerialize2()
        {
            string expected =
                "oprot.WriteMapBegin(new TMap(TType.Class, TType.String, Map.Count));\r\n" +
                "foreach (var _iter0 in Map)\r\n" +
                "{\r\n" +
                "    var _elem1 = new TestClass2Impl()\r\n" +
                "    {\r\n" +
                "        t1 = _iter0.Key.t1,\r\n" +
                "        t2 = _iter0.Key.t2,\r\n" +
                "        t3 = _iter0.Key.t3,\r\n" +
                "        t4 = _iter0.Key.t4,\r\n" +
                "    };\r\n" +
                "    _elem1.Write(oprot);\r\n" +
                "    oprot.WriteString(_iter0.Value);\r\n" +
                "}\r\n" +
                "oprot.WriteMapEnd();\r\n";

            generator.GenerateMapSerialize(new Generator.Parameter("Map", typeof(Dictionary<TestClass2, string>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateMapDeserialize()
        {
            string expected =
                "Map = new Dictionary<Flood.Tools.RPCGen.Tests.TestClass2, System.String>();\r\n"+
                "var _set0 = iprot.ReadMapBegin();\r\n"+
                "for (var _i1 = 0; _i1 < _set0.Count; ++_i1)\r\n"+
                "{\r\n"+
                "    var _elem4 = new TestClass2Impl();\r\n"+
                "    _elem4.Read(iprot);\r\n"+
                "    var _elem2 = new Flood.Tools.RPCGen.Tests.TestClass2()\r\n"+
                "    {\r\n"+
                "        t1 = _elem4.t1,\r\n"+
                "        t2 = _elem4.t2,\r\n"+
                "        t3 = _elem4.t3,\r\n"+
                "        t4 = _elem4.t4,\r\n"+
                "    };\r\n"+
                "    var _elem3 = iprot.ReadString();\r\n"+
                "    Map.Add(_elem2, _elem3);\r\n"+
                "}\r\n"+
                "iprot.ReadMapEnd();\r\n";

            generator.GenerateMapDeserialize(new Generator.Parameter("Map", typeof(Dictionary<TestClass2, string>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void GenerateMapDeserialize2()
        {
            string expected =
                "Map = new Dictionary<System.Int64, System.Boolean>();\r\n" +
                "var _set0 = iprot.ReadMapBegin();\r\n" +
                "for (var _i1 = 0; _i1 < _set0.Count; ++_i1)\r\n" +
                "{\r\n" +
                "    var _elem2 = iprot.ReadI64();\r\n" +
                "    var _elem3 = iprot.ReadBool();\r\n" +
                "    Map.Add(_elem2, _elem3);\r\n" +
                "}\r\n" +
                "iprot.ReadMapEnd();\r\n";
            
            generator.GenerateMapDeserialize(new Generator.Parameter("Map", typeof(Dictionary<long, bool>), 0));

            Assert.AreEqual(expected, generator.ToString());
        }

        [Test]
        public void ConvertFieldToParametersList()
        {
            var parameters = new List<Generator.Parameter>();
            parameters.Add(new Generator.Parameter("t1", typeof(long), 0));
            parameters.Add(new Generator.Parameter("t2", typeof(byte), 1));
            parameters.Add(new Generator.Parameter("t3", typeof(char), 2));
            parameters.Add(new Generator.Parameter("t4", typeof(TestAbstract), 3));

            Assert.AreEqual(parameters, generator.ConvertFieldToParametersList(typeof(TestClass2)));
        }

        [Test]
        public void ToTitleCase()
        {
            Assert.AreEqual("Some Random String", Generator.ToTitleCase("some random string"));
        }
        /*
        [Test]
        public void GenerateFields()
        {
            var fields = typeof(TestInheritance).GetFields(BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Public);
            string expected = "";

            Assert.AreEqual(expected, generator.ToString());
        }
        */

    }
}