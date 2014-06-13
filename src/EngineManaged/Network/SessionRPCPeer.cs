using Flood.Remoting;
using Flood.Serialization;
using System;
using System.Collections.Generic;

namespace Flood.Networking
{
    public class SessionRemotingPeer : RemotingPeer
    {
        public Session Session { get; private set; }

        public SessionRemotingPeer(Session session)
        {
            Session = session;
        }

        public override void Dispatch(Message data)
        {
            var bytes = data.Serializer.Buffer.ReadAllBytes();

            var packet = new Packet(0);
            packet.Write(new List<byte>(bytes));
            packet.Flags = ConvertFlags(data.Flags);

            Session.Peer.QueuePacket(packet, 0);
        }

        public override Serialization.Serializer CreateSerializer()
        {
            var serializer = new BinarySerializer();
            serializer.CustomSerializer = new RemotingSerializer();

            return serializer;
        }

        public override bool Equals(object other)
        {
            var s2 = other as SessionRemotingPeer;
            if (s2 == null)
                return false;

            return Session.Equals(s2.Session);
        }

        public override int GetHashCode()
        {
            return Session.GetHashCode();
        }

        private static PacketFlags ConvertFlags(MessageFlags flags)
        {
            switch (flags)
            {
                case MessageFlags.None:
                    return 0;
                case MessageFlags.Encrypted:
                    return PacketFlags.Encrypted;
                case MessageFlags.Signed:
                    return PacketFlags.Signed;
                case MessageFlags.Compressed:
                    return PacketFlags.Compressed;
                default:
                    throw new NotImplementedException();
            }
        }
    }
}
