using Microsoft.CSharp;
using NUnit.Framework;
using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

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

            string tempDirectory = Path.Combine(Path.GetTempPath(), Path.GetRandomFileName());
            Directory.CreateDirectory(tempDirectory);

            try {

                var args = new string[]
                {
                    String.Format("-o={0}",tempDirectory),
                    testAssemblyPath
                };

                var ret = Flood.Tools.RPCGen.Program.Main(args);
                Assert.AreEqual(0,ret);

            } finally {
                Directory.Delete(tempDirectory,true);
            }

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
