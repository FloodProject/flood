using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace RPCGen.Tests
{
    class Helper
    {
        static Assembly testAssembly;

        static void GenerateAssembly()
        {
            if (testAssembly != null)
                return;

            string genDirectory = Path.Combine("..", "..", "gen", "RPCGen.Tests.Services");
            Directory.CreateDirectory(genDirectory);

            var testAssemblyPath = Path.GetFullPath("RPCGen.Tests.Services.dll");

            var args = new string[]
            {
                String.Format("-o={0}", genDirectory),
                testAssemblyPath
            };

            var ret = Flood.Tools.RPCGen.Program.Main(args);
            Assert.AreEqual(0, ret);

            testAssembly = Assembly.LoadFile(testAssemblyPath);
        }

        public static object GetInstance(string type)
        {
            GenerateAssembly();

            var testObjectHandle = Activator.CreateInstance(testAssembly.FullName, type);
            Assert.NotNull(testObjectHandle);

            return testObjectHandle.Unwrap();
        }


    }
}
