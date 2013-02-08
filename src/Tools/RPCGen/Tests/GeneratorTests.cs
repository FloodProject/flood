using Flood.RPC.Protocol;
using NUnit.Framework;
using NSubstitute;
using Flood.Tools.RPCGen;

namespace RPCGen.Tests
{
    public class TestClass
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
        public void ProjectProperties()
        {
            TType type = Generator.ConvertFromTypeToThrift(typeof(TestClass));
            Assert.AreEqual(TType.Struct, type) ;
        }
    }
}