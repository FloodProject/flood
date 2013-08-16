using NUnit.Framework;
using RPCGen.Tests.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RPCGen.Tests
{
    [TestFixture]
    class MethodOverloadTests : IMethodOverloadTests
    {
        public IMethodOverloadTests tests;

        [TestFixtureSetUp]
        public void SetUp()
        {
            tests = (IMethodOverloadTests)Helper.GetInstance("RPCGen.Tests.Services.MethodOverloadTests");
        }

        [Test]
        public void TestOverload()
        {
            tests.TestOverload();
        }
    }
}
