
namespace RPCGen.Tests.Services
{
    public interface ITypeTests
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
        void TestMap();
        void TestList();
        void TestArray();
    }
}
