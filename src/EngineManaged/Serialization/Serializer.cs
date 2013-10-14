using System.Text;

namespace Flood.Serialization
{
    using Stream = System.IO.Stream;

    public interface ICustomSerializer
    {
        T Read<T>(Serializer serializer, object customData);
        void Write<T>(Serializer serializer, T value, object customData);
    }

    public abstract class Serializer
    {
        public Stream Buffer { get; private set; }

        public ICustomSerializer CustomSerializer;

        protected Serializer(Stream stream)
        {
            Buffer = stream;
        }

        public abstract void WriteDataObjectBegin(DataObject data);
        public abstract void WriteDataObjectEnd();
        public abstract void WriteFieldBegin(Field field);
        public abstract void WriteFieldEnd();
        public abstract void WriteFieldStop();
        public abstract void WriteMapBegin(TMap map);
        public abstract void WriteMapEnd();
        public abstract void WriteListBegin(TList list);
        public abstract void WriteListEnd();
        public abstract void WriteBool(bool b);
        public abstract void WriteByte(byte b);
        public abstract void WriteI16(short i16);
        public abstract void WriteI32(int i32);
        public abstract void WriteI64(long i64);
        public abstract void WriteDouble(double d);
        public virtual void WriteString(string s) {
            WriteBinary(Encoding.UTF8.GetBytes(s));
        }
        public abstract void WriteBinary(byte[] b);
        public void WriteCustom<T>(T value, object customData = null)
        {
            if(CustomSerializer != null)
                CustomSerializer.Write(this, value, customData);
        }

        public abstract DataObject ReadDataObjectBegin();
        public abstract void ReadDataObjectEnd();
        public abstract Field ReadFieldBegin();
        public abstract void ReadFieldEnd();
        public abstract TMap ReadMapBegin();
        public abstract void ReadMapEnd();
        public abstract TList ReadListBegin();
        public abstract void ReadListEnd();
        public abstract bool ReadBool();
        public abstract byte ReadByte();
        public abstract short ReadI16();
        public abstract int ReadI32();
        public abstract long ReadI64();
        public abstract double ReadDouble();
        public virtual string ReadString() {
            var buf = ReadBinary();
            return Encoding.UTF8.GetString(buf, 0, buf.Length);
        }
        public abstract byte[] ReadBinary();
        public T ReadCustom<T>(object customData = null)
        {
            if(CustomSerializer != null)
                return CustomSerializer.Read<T>(this, customData);

            return default(T);
        }
    }
}
