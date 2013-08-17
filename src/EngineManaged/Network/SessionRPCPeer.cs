using Flood.RPC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Network
{
    public class SessionRPCPeer : RPCPeer
    {
        public Session Session { get; private set; }

        public SessionRPCPeer(Session session)
        {
            Session = session;
        }

        public override void DispatchCall(RPCData data)
        {
            var stream = data.Serializer.Buffer;
            var bytes = new byte[stream.Length];
            stream.Position = 0;
            stream.Read(bytes, 0, (int) stream.Length);
            var bytesLst = new List<byte>(bytes);

            var packet = new Packet(0);
            packet.Write(bytesLst);
            packet.Flags = ConvertFlags(data.Flags);

            Session.Peer.QueuePacket(packet, 0);
        }

        public override bool Equals(object other)
        {
            var s2 = other as SessionRPCPeer;
            if (s2 == null)
                return false;

            return Session.Equals(s2.Session);
        }

        public override int GetHashCode()
        {
            return Session.GetHashCode();
        }

        private static PacketFlags ConvertFlags(RPCFlags flags)
        {
            switch (flags)
            {
                case RPCFlags.None:
                    return 0;
                case RPCFlags.Encrypted:
                    return PacketFlags.Encrypted;
                case RPCFlags.Signed:
                    return PacketFlags.Signed;
                case RPCFlags.Compressed:
                    return PacketFlags.Compressed;
                default:
                    throw new NotImplementedException();
            }
        }
    }
}
