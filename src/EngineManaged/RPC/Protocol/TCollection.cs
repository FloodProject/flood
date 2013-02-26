using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.RPC.Protocol
{
    public struct TCollection
    {
        private TType elementType;
        private int count;

        public TCollection(TType elementType, int count)
            : this()
        {
            this.elementType = elementType;
            this.count = count;
        }

        public TCollection(TList list)
            : this(list.ElementType, list.Count)
        {
        }
        public TType ElementType
        {
            get { return elementType; }
            set { elementType = value; }
        }

        public int Count
        {
            get { return count; }
            set { count = value; }
        }
    }
}
