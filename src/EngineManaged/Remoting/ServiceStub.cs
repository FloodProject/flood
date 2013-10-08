
namespace Flood.Remoting
{
    public class ServiceStub
    {
        public int LocalId { get; private set; }
        public MessageProcessor MessageProcessor  { get; internal set; }

        protected ServiceStub(int localId)
        {
            LocalId = localId;
        }
    }
}
