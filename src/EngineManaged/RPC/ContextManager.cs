
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Threading;
using System.Threading.Tasks;
using Flood.RPC.Metadata;
using Flood.RPC.Serialization;

namespace Flood.RPC
{
    public interface IDataObjectFactory
    {
        IObservableDataObject CreateDataObjectReference(
            ushort id, RPCPeer peer, int remoteId, ReferenceManager referenceManager);
    }

    public interface IContextId
    {
        void Write(RPCData data);
        void Read(RPCData data);
    }

    public interface IContextLoader
    {
        IContextId ReadContextId(RPCData data);
        Task<Assembly> LoadContext(RPCPeer peer, IContextId contextId);
        IContextId GetContextId(Assembly assembly);
    }

    public abstract class ContextLoader<T> : IContextLoader where T : IContextId, new()
    {
        public IContextId ReadContextId(RPCData data)
        {
            var contextId = new T();
            contextId.Read(data);
            return contextId;
        }

        public abstract Task<Assembly> LoadContext(RPCPeer peer, IContextId contextId);
        public abstract IContextId GetContextId(Assembly assembly);
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

        public static GlobalServiceId Read(RPCData data)
        {
            var contexId = data.RPCManager.ContextManager.ReadContextId(data);
            var id = (ushort)data.Serializer.ReadI16();

            return new GlobalServiceId(contexId, id);
        }

        public void Write(RPCData data)
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
            public Dictionary<RPCPeer, ushort> RemoteIds;
        }

        private Dictionary<int, ContextInfo> localIdToContext;
        private Dictionary<IContextId, ContextInfo> contextIdToContext;
        private Dictionary<Assembly, ContextInfo> assemblyToContext;

        private int localIdCounter;

        private RPCManager RPCManager;
        private IContextLoader Loader;

        public ContextManager(RPCManager rpcManager, IContextLoader loader)
        {
            localIdToContext = new Dictionary<int, ContextInfo>();
            contextIdToContext = new Dictionary<IContextId, ContextInfo>();
            assemblyToContext = new Dictionary<Assembly, ContextInfo>();
            localIdCounter = 1;

            RPCManager = rpcManager;
            Loader = loader;
        }

        public void Process(RPCData data)
        {
            switch (data.Header.CallType)
            {
                case RPCDataType.ContextRequest:
                    ProcessContextRequest(data);
                    return;
                case RPCDataType.ContextResponse:
                    ProcessContextResponse(data);
                    return;
                default:
                    throw new NotImplementedException();
            }
        }

        private void ProcessContextRequest(RPCData data)
        {
            var remoteId = data.Header.RemoteId;
            var contextId = Loader.ReadContextId(data);
            
            ContextInfo context;
            if (!contextIdToContext.TryGetValue(contextId, out context))
            {
                var task = Loader.LoadContext(data.Peer, contextId);
                task.ContinueWith(t =>
                {
                    if(t.Result == null)
                        throw new Exception("Context could not be loaded.");

                    var cxt = GetCreateContext(t.Result);
                    var res = new RPCData(data.Peer, RPCManager, cxt.LocalId, remoteId, RPCDataType.ContextResponse);
                    res.Dispatch();
                });
                return;
            }

            var response = new RPCData(data.Peer, RPCManager, context.LocalId, remoteId, RPCDataType.ContextResponse);
            response.Dispatch();
        }

        private void ProcessContextResponse(RPCData data)
        {
            var localId = data.Header.LocalId;
            var remoteId = (ushort) data.Header.RemoteId;

            ContextInfo context;
            if(!localIdToContext.TryGetValue(localId, out context))
                throw new Exception("No local context available.");

            if(context.RemoteIds == null)
                context.RemoteIds = new Dictionary<RPCPeer, ushort>();

            context.RemoteIds.Add(data.Peer, remoteId);
        }

        public void RequestContext(RPCPeer peer, Assembly assembly)
        {
            var context = GetCreateContext(assembly);
            var request = new RPCData(peer, RPCManager, context.LocalId, 0, RPCDataType.ContextRequest);
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

        public Type GetPeerPolymorphicType(RPCPeer peer, Type type, Type baseType, out ushort remoteContextId, out ushort dataObjectId)
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

        internal IContextId ReadContextId(RPCData data)
        {
            return Loader.ReadContextId(data);
        }
    }
}
