using Flood.RPC.Protocol;
using Flood;

namespace Flood.RPC
{
    public class RPCData
    {
        public int ServiceId { get; set; }
        public int RequestId { get; set; }
        public virtual bool IsResponse { get; set; }

        public BinarySerializer Serializer { get; private set; }

        public PacketFlags Flags { get; set; }
        public Session Session { get; set; }
        

        public RPCData()
        {
            Serializer = new BinarySerializer();
        }

        public RPCData(RPCBuffer buffer)
        {
            Serializer = new BinarySerializer();
        }

        public RPCData(RPCData data)
            : this()
        {
            ServiceId = data.ServiceId;
            RequestId = data.RequestId;
            Session = data.Session;
        }

        // GetHashCode and Equals are implemented in a way that a response 
        // is considered equal to its request. This way we can use a request
        // as a dictionary key and retrieve its value with the given response.
        public override int GetHashCode()
        {
            return RequestId;
        }

        public override bool Equals(object obj)
        {
            var d = obj as RPCData;
            if (d == null) return false;

            return ServiceId == d.ServiceId
                   && RequestId == d.RequestId
                   && Session.Equals(d.Session);
        }
    }
}
