
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using Flood.Modules;

namespace EngineManaged.Modules
{
    public class ModuleLibrary
    {
        interface IModuleLocation
        {
            Stream Stream { get; }
        }

        class FileModuleLocation : IModuleLocation
        {
            public string FilePath;

            public FileModuleLocation(string filePath)
            {
                FilePath = filePath;
            }

            public Stream Stream
            {
                get { return File.OpenRead(FilePath); }
            }
        }

        private Dictionary<ModuleId, IModuleLocation> moduleLocations;

        public ModuleLibrary()
        {
            moduleLocations = new Dictionary<ModuleId, IModuleLocation>();
        }

        public void AddDirectory(string path)
        {
            var filePaths = Directory.EnumerateFiles(path);
            foreach (var filePath in filePaths)
            {
                if (!filePath.EndsWith(".dll"))
                    continue;

                var fileName = Path.GetFileNameWithoutExtension(filePath);

                AssemblyName assemblyName;
                try
                {
                    assemblyName = AssemblyName.GetAssemblyName(filePath);
                }
                catch
                {
                    continue;
                }

                moduleLocations.Add(new ModuleId(fileName, assemblyName.Version.Major),
                    new FileModuleLocation(filePath));
            }
        }

        public Stream GetModuleStream(ModuleId module)
        {
            IModuleLocation location;
            if(!moduleLocations.TryGetValue(module, out location))
                throw new Exception();

            return location.Stream;
        }
    }
}
