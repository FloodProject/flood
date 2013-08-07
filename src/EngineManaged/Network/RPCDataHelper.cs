using System.Linq;
using Flood;
using Flood.RPC;
using Flood.RPC.Serialization;

namespace EngineManaged.Network
{
    class RPCDataHelper
    {
        public static Packet CreatePacket(RPCData data, int packetId)
        {
            var serializer = new BinarySerializer();
            serializer.WriteI32(data.ServiceId);
            serializer.WriteI32(data.RequestId);
            serializer.WriteBool(data.IsResponse);
            var bytes = data.Serializer.Buffer.ReadAll();
            serializer.WriteBinary(bytes);

            var packet = new Packet((ushort)packetId);
            packet.Write(serializer.Buffer.ReadAll().ToList());
            packet.Flags = data.Flags;

            return packet;
        }

        public static RPCData CreateRPCData(Packet packet)
        {
            var serializer = new BinarySerializer();
            serializer.Buffer.Write(packet.Read().ToArray());

            var data = new RPCData();

            data.ServiceId = serializer.ReadI32();
            data.RequestId = serializer.ReadI32();
            data.IsResponse = serializer.ReadBool();
            var bytes = serializer.ReadBinary();
            data.Serializer.Buffer.Write(bytes);

            data.Flags = packet.Flags;

            return data;
        }
    }
}
