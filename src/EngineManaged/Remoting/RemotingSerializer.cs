using System;
using System.Reflection;
using Flood.Remoting.Metadata;
using Flood.Serialization;

namespace Flood.Remoting
{
    public class RemotingSerializer : ICustomSerializer
    {
        public enum DataType : byte
        {
            DataObject,
            Delegate,
            Service
        }

        public object Read(Serializer serializer, CustomData customData)
        {
            var message = (Message) customData.Data;

            var dataType = (DataType) serializer.ReadByte();
            switch (dataType)
            {
                case DataType.DataObject:
                    if(!typeof(IDataObject).IsAssignableFrom(customData.Type))
                        throw new Exception();

                    return ReadDataObject(serializer, customData.Type, message);

                case DataType.Delegate:
                    var remoteDelegateId = serializer.ReadI32();
                    var contextId = message.RemotingManager.ContextManager.GetContextId(customData.RequestingAssembly);
                    var del = message.RemotingManager.DelegateManager.CreateDelegateProxy(contextId, customData.Type, message.Peer, remoteDelegateId);
                    return del.Delegate;

                case DataType.Service:
                    var remoteId = serializer.ReadI32();
                    return message.RemotingManager.ServiceManager.GetService(customData.Type, message.Peer, remoteId);

                default:
                    throw new NotImplementedException();
            }
        }

        public void Write(Serializer serializer, object value, CustomData customData)
        {
            var message = (Message) customData.Data;

            if (value is IDataObject)
            {
                serializer.WriteByte((byte)DataType.DataObject);
                WriteDataObject(serializer, customData.Type, (IDataObject) value, message);
                return;
            }

            if (value is Delegate)
            {
                serializer.WriteByte((byte)DataType.Delegate);
                var contextId = message.RemotingManager.ContextManager.GetContextId(customData.RequestingAssembly);
                var del = message.RemotingManager.DelegateManager.CreateDelegateImpl(contextId, (Delegate)value);
                serializer.WriteI32(del.LocalId);
                // TODO: Serialize Peer and RemoteId so we can create delegate proxies to other peer's delegates.
                return;
            }

            if (ServiceManager.IsService(customData.Type))
            {
                serializer.WriteByte((byte)DataType.Service);
                var serviceImpl = message.RemotingManager.ServiceManager.GetCreateImplementation(customData.Type, value);
                serializer.WriteI32(serviceImpl.LocalId);
                return;
            }

            throw new NotImplementedException();
        }

        private static object ReadDataObject(Serializer serializer, Type type, Message message)
        {
            if(!typeof(IObservableDataObject).IsAssignableFrom(type))
                return Activator.CreateInstance(type);

            IDataObjectFactory dataObjectFactory;
            int dataObjectId;
 
            var referenceRemoteId = serializer.ReadI32();
            var isPolymorphic = serializer.ReadBool();
            if (isPolymorphic)
            {
                var contextId = serializer.ReadI16();
                dataObjectId = serializer.ReadI16();
                dataObjectFactory = message.RemotingManager.ContextManager.GetDataObjectFactory(contextId);
            }
            else
            {
                dataObjectId = type.GetCustomAttribute<DataObjectAttribute>().Id;
                var contextId = message.RemotingManager.ContextManager.GetContextId(type.Assembly);
                dataObjectFactory = message.RemotingManager.ContextManager.GetDataObjectFactory(contextId);
            }

            var dataObject = dataObjectFactory.CreateDataObjectReference((ushort) dataObjectId, message.Peer, referenceRemoteId, message.RemotingManager.ReferenceManager);
            dataObject.Read(serializer, null, message);

            return dataObject;
        }

        private static void WriteDataObject(Serializer serializer, Type type, IDataObject dataObject, Message message)
        {
            if (!(dataObject is IObservableDataObject))
            {
                dataObject.Write(serializer);
                return;
            }

            var observable = (IObservableDataObject)dataObject;
            if(observable.IsReference)
                message.RemotingManager.ReferenceManager.Publish(observable);

            int referenceLocalId;
            if (!message.RemotingManager.ReferenceManager.TryGetLocalId(observable, out referenceLocalId))
                referenceLocalId = 0;

            serializer.WriteI32(referenceLocalId);

            ushort remoteContextId;
            ushort dataObjectId;
            var polymorphicType = message.RemotingManager.ContextManager.GetPeerPolymorphicType(message.Peer, dataObject.GetType(), type, out remoteContextId, out dataObjectId);
            var isPolymorphic = polymorphicType != type;
            serializer.WriteBool(isPolymorphic);
            if (isPolymorphic)
            {
                serializer.WriteI16((short)remoteContextId);
                serializer.WriteI16((short)dataObjectId);
            }

            dataObject.Write(serializer, polymorphicType);
        }
    }
}
