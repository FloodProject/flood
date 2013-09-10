
namespace Flood.RPC
{
    public class RPCStub
    {
        public int LocalId { get; private set; }
        public RPCManager RPCManager  { get; internal set; }

        protected RPCStub(int localId)
        {
            LocalId = localId;
        }
    }
}
