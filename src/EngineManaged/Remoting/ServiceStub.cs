
namespace Flood.Remoting
{
    public class ServiceStub
    {
        public int LocalId { get; private set; }
        public RemotingManager RemotingManager  { get; internal set; }

        protected ServiceStub(int localId)
        {
            LocalId = localId;
        }
    }
}
