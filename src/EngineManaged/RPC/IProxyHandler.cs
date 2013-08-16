using System.Threading.Tasks;

namespace Flood.RPC
{
    public interface IProxyHandler
    {
        Task<RPCData> RemoteProcedureCall(RPCData data);
    }
}
