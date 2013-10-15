using System;
using System.IO;
using Flood.Serialization;

namespace Flood.Persistence
{
    public enum SerializerType
    {
        Binary,
    }

    public class Store
    {
        public static bool Save(object obj, string file, SerializerType serializerType = SerializerType.Binary)
        {
            var dataObject = obj as IDataObject;
            if (dataObject == null)
                return false;

            using (var stream = new FileStream(file, FileMode.Create))
            {
                var serializer = GetSerializer(serializerType, stream);
                dataObject.Write(serializer);
                stream.Flush();
            }

            return true;
        }

        public static T Load<T>(string file, SerializerType serializerType = SerializerType.Binary)
        {
            var dataObject = (IDataObject) Activator.CreateInstance<T>();

            using (var stream = new FileStream(file, FileMode.Open))
            {
                var serializer = GetSerializer(serializerType, stream);
                dataObject.Read(serializer);
            }

            return (T) dataObject;
        }

        private static Serializer GetSerializer(SerializerType serializerType, System.IO.Stream stream)
        {
            switch (serializerType)
            {
                case SerializerType.Binary:
                    return new BinarySerializer(stream);
                default:
                    throw new NotImplementedException();
            }
        }
    }
}
