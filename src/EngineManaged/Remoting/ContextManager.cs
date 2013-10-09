
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Threading;
using System.Threading.Tasks;
using Flood.Remoting.Metadata;
using Flood.Remoting.Serialization;

namespace Flood.Remoting
{
    public interface IDataObjectFactory
    {
        IObservableDataObject CreateDataObjectReference(
            ushort id, RemotingPeer peer, int remoteId, ReferenceManager referenceManager);
    }

    public interface IContextId
    {
        void Write(Message data);
        void Read(Message data);
    }

    public interface IContextLoader
    {
        IContextId ReadContextId(Message data);
        Task<Assembly> LoadContext(IContextId contextId);
        IContextId GetContextId(Assembly assembly);
    }

    struct GlobalServiceId
    {
        public readonly IContextId ContextId;
        public readonly ushort Id;

        public GlobalServiceId(IContextId contextId, ushort id)
        {
            ContextId = contextId;
            Id = id;
        }

        public static GlobalServiceId Read(Message data)
        {
            var contexId = data.RemotingManager.ContextManager.ReadContextId(data);
            var id = (ushort)data.Serializer.ReadI16();

            return new GlobalServiceId(contexId, id);
        }

        public void Write(Message data)
        {
            ContextId.Write(data);
            data.Serializer.WriteI16((short)Id);
        }
    }

    public class ContextManager
    {
        private class ContextInfo
        {
            public int LocalId;
            public IContextId ContextId;
            public IDataObjectFactory DataObjectFactory;
            public Dictionary<RemotingPeer, ushort> RemoteIds;
        }

        private Dictionary<int, ContextInfo> localIdToContext;
        private Dictionary<IContextId, ContextInfo> contextIdToContext;
        private Dictionary<Assembly, ContextInfo> assemblyToContext;

        private int localIdCounter;

        private RemotingManager remotingManager;
        private IContextLoader Loader;

        public ContextManager(RemotingManager remotingManager, IContextLoader loader)
        {
            localIdToContext = new Dictionary<int, ContextInfo>();
            contextIdToContext = new Dictionary<IContextId, ContextInfo>();
            assemblyToContext = new Dictionary<Assembly, ContextInfo>();
            localIdCounter = 1;

            this.remotingManager = remotingManager;
            Loader = loader;
        }

        public void Process(Message data)
        {
            switch (data.Header.CallType)
            {
                case MessageType.ContextRequest:
                    ProcessContextRequest(data);
                    return;
                case MessageType.ContextResponse:
                    ProcessContextResponse(data);
                    return;
                default:
                    throw new NotImplementedException();
            }
        }

        private void ProcessContextRequest(Message data)
        {
            var remoteId = data.Header.RemoteId;
            var contextId = Loader.ReadContextId(data);
            
            ContextInfo context;
            if (!contextIdToContext.TryGetValue(contextId, out context))
            {
                //TODO: check data.Peer permissions
                var task = Loader.LoadContext(contextId);
                task.ContinueWith(t =>
                {
                    if(t.Result == null)
                        throw new Exception("Context could not be loaded.");

                    var cxt = GetCreateContext(t.Result);
                    var res = new Message(data.Peer, remotingManager, cxt.LocalId, remoteId, MessageType.ContextResponse);
                    res.Dispatch();
                });
                return;
            }

            var response = new Message(data.Peer, remotingManager, context.LocalId, remoteId, MessageType.ContextResponse);
            response.Dispatch();
        }

        private void ProcessContextResponse(Message data)
        {
            var localId = data.Header.LocalId;
            var remoteId = (ushort) data.Header.RemoteId;

            ContextInfo context;
            if(!localIdToContext.TryGetValue(localId, out context))
                throw new Exception("No local context available.");

            if(context.RemoteIds == null)
                context.RemoteIds = new Dictionary<RemotingPeer, ushort>();

            context.RemoteIds.Add(data.Peer, remoteId);
        }

        public void RequestContext(RemotingPeer peer, Assembly assembly)
        {
            var context = GetCreateContext(assembly);
            var request = new Message(peer, remotingManager, context.LocalId, 0, MessageType.ContextRequest);
            context.ContextId.Write(request);
            request.Dispatch();
        }

        public IDataObjectFactory GetDataObjectFactory(int localId)
        {
            ContextInfo context;
            if (!localIdToContext.TryGetValue(localId, out context))
                throw new Exception("Context not found.");

            return context.DataObjectFactory;
        }

        public IContextId GetContextId(Assembly assembly)
        {
            return GetCreateContext(assembly).ContextId;
        }

        public Type GetPeerPolymorphicType(RemotingPeer peer, Type type, Type baseType, out ushort remoteContextId, out ushort dataObjectId)
        {
            remoteContextId = 0;
            dataObjectId = 0;

            if (type == null)
                throw new ArgumentNullException("type");

            if (type == baseType)
                return type;

            var dataObjectAttribute = type.GetCustomAttribute<DataObjectAttribute>(false);
            if (dataObjectAttribute != null)
            {
                dataObjectId = dataObjectAttribute.Id;

                var context = GetCreateContext(type.Assembly);

                if (context.RemoteIds != null &&
                    context.RemoteIds.TryGetValue(peer, out remoteContextId))
                    return type;
            }

            return GetPeerPolymorphicType(peer, type.BaseType, baseType, out remoteContextId, out dataObjectId);
        }

        private ContextInfo GetCreateContext(Assembly assembly)
        {
            ContextInfo context;
            if (assemblyToContext.TryGetValue(assembly, out context))
                return context;

            var contextType = assembly.GetType("DataObjectFactory");

            context = new ContextInfo
            {
                LocalId = Interlocked.Increment(ref localIdCounter),
                ContextId = Loader.GetContextId(assembly),
                DataObjectFactory = (IDataObjectFactory) Activator.CreateInstance(contextType)
            };

            localIdToContext.Add(context.LocalId, context);
            contextIdToContext.Add(context.ContextId, context);
            assemblyToContext.Add(assembly, context);

            return context;
        }

        internal GlobalServiceId GetGlobalServiceId(Type type)
        {
            var attribute = type.GetCustomAttribute<GlobalServiceAttribute>(false);
            if (attribute == null)
                throw new Exception("Type has no attribute GlobalService.");

            var contextId = GetContextId(type.Assembly);
            return new GlobalServiceId(contextId, attribute.Id);
        }

        internal IContextId ReadContextId(Message data)
        {
            return Loader.ReadContextId(data);
        }
    }
}
