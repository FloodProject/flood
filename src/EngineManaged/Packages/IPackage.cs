
using Flood.Remoting;

namespace Flood.Packages
{
    public interface IPackage
    {
        void OnLoad(RemotingManager remotingManager);

        void Update();
    }
}
