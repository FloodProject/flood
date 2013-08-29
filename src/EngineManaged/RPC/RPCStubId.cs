using System;
using Flood.RPC.Serialization;

namespace Flood.RPC
{
    public struct RPCStubId
    {
        public int Id;
        public Guid Guid;
        public int VersionMajor;
        public bool HasId;

        public RPCStubId(int id)
        {
            Id = id;
            Guid = Guid.Empty;
            VersionMajor = 0;
            HasId = true;
        }

        public RPCStubId(Guid guid, int versionMajor)
        {
            Id = 0;
            Guid = guid;
            VersionMajor = versionMajor;
            HasId = false;
        }

        public static RPCStubId Read(Serializer serializer)
        {
            var hasId = serializer.ReadBool();
            if (hasId)
            {
                var id = serializer.ReadI32();
                return new RPCStubId(id);
            }
                
            var guid = serializer.ReadString();
            var v = serializer.ReadI32();
            return new RPCStubId(new Guid(guid), v);
        }

        public void Write(Serializer serializer)
        {
            serializer.WriteBool(HasId);
            if (HasId)
            {
                serializer.WriteI32(Id);
                return;
            }

            serializer.WriteString(Guid.ToString());
            serializer.WriteI32(VersionMajor);
        }
    }
}
