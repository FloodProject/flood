using NUnit.Framework;
using System;
using System.IO;
using System.Reflection;

namespace RPCGen.Tests
{
    [TestFixture]
    class RPCGenTests 
    {
        [Test]
        public void MainTest()
        {
            string genDirectory = Path.Combine("..", "..", "gen", "RPCGen.Tests");
            Directory.CreateDirectory(genDirectory);

            var sourceDllPath = Path.GetFullPath("RPCGen.Tests.Services.dll");
            var destDllPath = Path.Combine(genDirectory, "RPCGen.Tests.Services.dll");
            var sourcePdbPath = Path.GetFullPath("RPCGen.Tests.Services.pdb");
            var destPdbPath = Path.Combine(genDirectory, "RPCGen.Tests.Services.pdb");

            System.IO.File.Copy(sourceDllPath, destDllPath, true);

            if (File.Exists(sourcePdbPath))
                System.IO.File.Copy(sourcePdbPath, destPdbPath, true);

            var args = new string[]
            {
                String.Format("-o={0}", genDirectory),
                destDllPath
            };

            var ret = Flood.Tools.RPCGen.Program.Main(args);
            Assert.AreEqual(0, ret);
        }
    }
}
