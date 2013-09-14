

using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using Flood.RPC;
using Flood.RPC.Metadata;
using Ionic.Zip;

namespace Flood.Packages
{
    class PackageLoader
    {
        public const string PackageExtension = ".fld";

        public string PackageFolder { get; private set; }
        public RPCManager RpcManager { get; private set; }

        private HashSet<PackageName> availablePackages;

        public PackageLoader(string packageFolder, RPCManager rpcManager)
        {
            packageFolder = Path.GetFullPath(packageFolder);
            PackageFolder = packageFolder;
            RpcManager = rpcManager;

            availablePackages = new HashSet<PackageName>();
            Init();
        }

        private void Init()
        {
            var filePaths = Directory.EnumerateFiles(PackageFolder);
            foreach (var filePath in filePaths)
            {
                if (!filePath.EndsWith(PackageExtension))
                    return;

                var fileName = Path.GetFileNameWithoutExtension(filePath);
                var packageName = new PackageName(fileName);
                availablePackages.Add(packageName);
            }
        }

        public IPackage LoadPackage(PackageName packageName)
        {
            PackageName availablePackageName;
            if(!TryGetPackageName(packageName, out availablePackageName))
                throw new Exception("Package "+ packageName +" unavailable.");

            var packageFile = availablePackageName + PackageExtension;
            var packagePath = Path.Combine(PackageFolder, packageFile);

            using (var zip = ZipFile.Read(packagePath))
            {
                var packageDllEntry = zip[packageName.Id+".dll"];

                var packageAssembly = ExtractAssembly(packageDllEntry);
                return LoadPackageAssembly(packageAssembly);
            }
        }

        private IPackage LoadPackageAssembly(Assembly assembly)
        {
            var package = GetPackageObject(assembly);

            package.OnLoad(RpcManager);
            CheckPackageGlobalServices(assembly);

            return package;
        }

        //Gets the most recent compatible PackageName available.
        private bool TryGetPackageName(PackageName packageName, out PackageName packageNameFound)
        {
            var wasNameFound = false;
            var bestVersion = packageName.Version;
            packageNameFound = default (PackageName);

            foreach (var name in availablePackages)
            {
                if (name.Id != packageName.Id)
                    continue;

                if (name.Version.Major != bestVersion.Major)
                    continue;

                if (name.Version.Minor < bestVersion.Minor)
                    continue;

                if (name.Version.Patch < bestVersion.Patch)
                    continue;

                wasNameFound = true;
                bestVersion = name.Version;
                packageNameFound = name;
            }

            return wasNameFound;
        }

        private Assembly ExtractAssembly(ZipEntry entry)
        {
            var stream = new MemoryStream();
            entry.Extract(stream);
            return Assembly.Load(stream.ReadAllBytes());
        }

        private IPackage GetPackageObject(Assembly assembly)
        {
            Type packageType = null;
            foreach (var type in assembly.GetTypes())
            {
                if (!typeof (IPackage).IsAssignableFrom(type)) 
                    continue;

                if(packageType != null)
                    throw new Exception("Package can have only one IPackage implementation.");

                packageType = type;
            }

            if(packageType == null)
                throw new Exception("Package must have one IPackage implementation.");

            var ctor = packageType.GetConstructor(new Type[] {});
            if(ctor == null)
                throw new Exception("Package's IPackage implementation must have a paramerless constructor.");

            return (IPackage) ctor.Invoke(new object[]{});
        }

        private void CheckPackageGlobalServices(Assembly assembly)
        {
            foreach (var type in assembly.GetTypes())
            {
                var attribute = type.GetCustomAttribute<GlobalServiceAttribute>();
                if (attribute == null)
                    continue;

                if(!RpcManager.ServiceManager.HasGlobalService(type))
                    throw new Exception("Package does not instantiate global service "+type);
            }
        }
    }
}
