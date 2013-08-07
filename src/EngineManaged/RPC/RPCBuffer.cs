using System;
using System.IO;

namespace Flood.RPC
{
    public class RPCBuffer
    {
        protected MemoryStream stream;
        private bool isReading;

        public RPCBuffer()
        {
            stream = new MemoryStream();
        }

        public int Read(byte[] buf, int off, int len)
        {
            if (!isReading)
            {
                isReading = true;
                stream.Position = 0;
            }
            
            return stream.Read(buf, off, len);
        }

        public byte[] ReadAll()
        {
            var bytes = new byte[stream.Length];
            Read(bytes, 0, (int) stream.Length);
            return bytes;
        }

        public void Write(byte[] buf, int off, int len)
        {
            if(isReading)
                throw new Exception("Cannot write to RPCBuffer after reading it.");
            stream.Write(buf, off, len);
        }

         public void Write(byte[] buf)
         {
             Write(buf, 0, buf.Length);
         }

        public long Length
        {
            get { return stream.Length; }
        }
    }
}
