
using Flood.RPC;

namespace Flood.Packages
{
    public interface IPackage
    {
        void OnLoad(RPCManager rpcManager);
    }
}
