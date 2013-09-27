using NUnit.Framework;
using System;
using System.IO;

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

            var assemblyName = "RPCGen.Tests.Services";
            var assemblyDll = assemblyName + ".dll";
            var assemblyPdb = assemblyName + ".pdb";

            var sourceDllPath = Path.GetFullPath(assemblyDll);
            var destDllPath = Path.Combine(genDirectory, assemblyDll);
            var sourcePdbPath = Path.GetFullPath(assemblyPdb);
            var destPdbPath = Path.Combine(genDirectory, assemblyPdb);

            File.Copy(sourceDllPath, destDllPath, true);

            if (File.Exists(sourcePdbPath))
                File.Copy(sourcePdbPath, destPdbPath, true);

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
