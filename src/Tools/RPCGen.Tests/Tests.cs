using NUnit.Framework;
using System;
using System.IO;
using System.Reflection;

namespace RPCGen.Tests
{
    [TestFixture]
    class Tests
    {
        public object testObject;
        public MethodInfo testMethod;

        [TestFixtureSetUp] 
        public void Init()
        {
            var testAssemblyPath = Path.GetFullPath("RPCGen.Tests.Services.dll");

            string genDirectory = Path.Combine(Path.GetDirectoryName(testAssemblyPath), "gen","RPCGen.Tests.Services");
            Directory.CreateDirectory(genDirectory);

            var args = new string[]
            {
                String.Format("-o={0}", genDirectory),
                testAssemblyPath
            };

            var ret = Flood.Tools.RPCGen.Program.Main(args);
            Assert.AreEqual(0,ret);

            var testAssembly = Assembly.LoadFile(testAssemblyPath);

            var testObjectHandle = Activator.CreateInstance(testAssembly.FullName, "RPCGen.Tests.Services.Tests");
            Assert.NotNull(testObjectHandle);

            testObject = testObjectHandle.Unwrap();

            testMethod = testObject.GetType().GetMethod("Test"); 
            Assert.NotNull(testMethod);
        }

        [Test]
        public void Test()
        {
            testMethod.Invoke(testObject, new object[]{});
        }

        

    }
}
