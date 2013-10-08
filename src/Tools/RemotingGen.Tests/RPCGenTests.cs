using NUnit.Framework;
using System;
using System.IO;

namespace RemotingGen.Tests
{
    [TestFixture]
    class RemotingGenTests 
    {
        [Test]
        public void MainTest()
        {
            string genDirectory = Path.Combine("..", "..", "gen", "RemotingGen.Tests");

            Directory.CreateDirectory(genDirectory);

            var assemblyName = "RemotingGen.Tests.Services";
            var assemblyDir = Path.GetFullPath(".");
            var assemblyDll = assemblyName + ".dll";

            var sourceDllPath = Path.Combine(assemblyDir, assemblyDll);

            var args = new string[]
            {
                String.Format("-o={0}", genDirectory),
                sourceDllPath
            };

            var ret = RemotingGen.Program.Main(args);
            Assert.AreEqual(0, ret);
        }
    }
}
