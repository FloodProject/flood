using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.Packages;
using Flood.RPC;

namespace Flood.Packages
{
    public class PackageManager
    {
        public readonly Dictionary<PackageName, IPackage> Packages;

        private PackageLoader packageLoader;

        public PackageManager(string packageFolder, RPCManager rpcManager)
        {
            Packages = new Dictionary<PackageName, IPackage>();
            packageLoader = new PackageLoader(packageFolder, rpcManager);
        }

        public IPackage GetLoadPackage(PackageName name)
        {
            if (Packages.ContainsKey(name))
                return Packages[name];

            var package = packageLoader.LoadPackage(name);
            Packages.Add(name, package);

            return package;
        }

        public void UpdatePackages()
        {
            foreach (var package in Packages.Values)
            {
                try
                {
                   package.Update();
                }
                catch
                {
                    Log.Error("Error updating package.");
                }
            }
        }
    }
}
