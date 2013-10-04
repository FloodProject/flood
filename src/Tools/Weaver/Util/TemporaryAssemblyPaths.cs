
using System;
using System.IO;

namespace Weaver.Util
{
    public sealed class TemporaryAssemblyPaths : IDisposable 
    {
        public string DllPath { get; private set; }
        public string PdbPath { get; private set; }

        public TemporaryAssemblyPaths() :
            this(Path.GetTempPath())
        {
        }

        public TemporaryAssemblyPaths(string directory)
        {
            var randomFileName = Path.Combine(directory, Path.GetRandomFileName());
            DllPath = randomFileName + ".dll";
            PdbPath = randomFileName + ".pdb";
        }

        ~TemporaryAssemblyPaths() {
            Delete();
        }

        public void Dispose() {
            Delete();
            GC.SuppressFinalize(this);
        }

        private void Delete() {
            if (File.Exists(DllPath))
                File.Delete(DllPath);
            if (File.Exists(PdbPath))
                File.Delete(PdbPath);
        }
    }
}
