
using System.Threading;
using Flood.Network;
using Flood.Packages;
using Flood.RPC;

namespace Flood.Editor
{
    public class Runtime
    {
        public static PackageManager packageManager;

        public static void Main(string[] args)
        {
            var rpcManager = new RPCManager(new SessionRPCPeer(null), new PackageContextLoader());

            packageManager = new PackageManager("Packages", rpcManager);

            packageManager.GetLoadPackage(new PackageName("EditorPackage.0.0.0"));

            while (true)
            {
                packageManager.UpdatePackages();
                Thread.Sleep(1);
            }
        }
    }
}