using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RPCGen.Tests.Services
{
    public interface IPrimitiveTypesTests
    {
        void SetUp();

        void TestVoid();
        void TestByte();
        void TestFloat();
        void TestDouble();
        void TestShort();
        void TestInt();
        void TestLong();
        void TestString();
        void TestGuid();
        void TestDateTime();
    }
}
