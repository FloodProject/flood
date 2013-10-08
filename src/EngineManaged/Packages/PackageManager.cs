
using System.Collections.Generic;

namespace Flood.Packages
{
    public class PackageManager
    {
        public readonly Dictionary<PackageName, IPackage> Packages;

        private PackageLoader packageLoader;

        public PackageManager(PackageLoader packageLoader)
        {
            Packages = new Dictionary<PackageName, IPackage>();
            this.packageLoader = packageLoader;
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
