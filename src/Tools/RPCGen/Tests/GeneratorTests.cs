using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;
using Flood.RPC.Metadata;
using Flood.RPC.Protocol;
using NUnit.Framework;
using NSubstitute;
using Flood.Tools.RPCGen;

namespace RPCGen.Tests
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

    public class TestClass
    {
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
                    "    private RPCGen.Tests.TestClass _t4;\r\n" + 
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
                    "    public RPCGen.Tests.TestClass T4\r\n" + 
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
                    "                    if (field.Type == TType.Struct)\r\n" + 
                    "                    {\r\n" + 
                    "                        var T4Impl = new TestClassImpl();\r\n" + 
                    "                        T4Impl.Read(iprot);\r\n" + 
                    "                        T4 = new RPCGen.Tests.TestClass()\r\n" + 
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
                    "        if (__isset.t4)\r\n" + 
                    "        {\r\n" + 
                    "            field.Name = \"t4\";\r\n" + 
                    "            field.Type = TType.Struct;\r\n" + 
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
            string expected = "";
            Assert.AreEqual(expected, generator.ToString());
        }



    }
}