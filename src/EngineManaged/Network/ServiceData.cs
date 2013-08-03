using Flood;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EngineManaged.Network
{
    class ServiceData
    {
        public int ServiceId { get; set;}
        public Peer Peer { get; set;}
        public byte[] Data { get; set;}
        public PacketFlags Flags { get; set;}
    }
}
