using System.Threading.Tasks;

namespace Flood.RPC
{
    public interface IRPCManager
    {
        Task<RPCData> RemoteProcedureCall(RPCData data);
    }
}
