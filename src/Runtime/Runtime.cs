
using System.Threading;
using Flood.Network;
using Flood.Packages;
using Flood.Remoting;

namespace Flood.Editor
{
    public class Runtime
    {
        public static PackageManager packageManager;

        public static void Main(string[] args)
        {
            var messageProcessor = new MessageProcessor(new SessionRemotingPeer(null), new PackageContextLoader());

            packageManager = new PackageManager("Packages", messageProcessor);
            while (true)
            {
                packageManager.UpdatePackages();
                Thread.Sleep(1);
            }
        }
    }
}