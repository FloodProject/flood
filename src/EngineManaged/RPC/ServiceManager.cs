using System;
using System.Collections.Generic;
using System.Reflection;
using System.Threading;
using Flood.RPC.Metadata;

namespace Flood.RPC
{
    public class ServiceManager
    {
        private struct GlobalServiceId
        {
            public readonly Guid Guid;
            public readonly int MajorVersion;

            public GlobalServiceId(Guid guid, int majorVersion)
            {
                Guid = guid;
                MajorVersion = majorVersion;
            }

            public GlobalServiceId(Type type)
            {
                var attribute = type.GetCustomAttribute<GlobalServiceAttribute>(false);
                if(attribute == null)
                    throw new Exception("Type has no attribute GlobalService.");

                Guid = attribute.Guid;
                MajorVersion = type.Assembly.GetName().Version.Major;
            }

            public static GlobalServiceId Read(RPCData data)
            {
                var guid = data.Serializer.ReadString();
                var majorVersion = data.Serializer.ReadI32();

                return new GlobalServiceId(new Guid(guid), majorVersion);
            }

            public void Write(RPCData data)
            {
                data.Serializer.WriteString(Guid.ToString());
                data.Serializer.WriteI32(MajorVersion);
            }
        }

        private Dictionary<int, RPCStub> stubs;
        private int stubIdCounter;

        private Dictionary<object, RPCImpl> impls;
        private Dictionary<Tuple<RPCPeer, int>, RPCProxy> proxies;

        private Dictionary<Tuple<RPCPeer, GlobalServiceId>, RPCProxy> globalServiceProxies;
        private Dictionary<GlobalServiceId, int> localGlobalServiceIds;

        private RPCManager RPCManager;

        public ServiceManager(RPCManager rpcMananger)
        {
            stubs = new Dictionary<int, RPCStub>();
            impls = new Dictionary<object, RPCImpl>();
            proxies = new Dictionary<Tuple<RPCPeer, int>, RPCProxy>();
            globalServiceProxies = new Dictionary<Tuple<RPCPeer, GlobalServiceId>, RPCProxy>();
            localGlobalServiceIds = new Dictionary<GlobalServiceId, int>();

            RPCManager = rpcMananger;
        }

        internal void Process(RPCData data)
        {
            var stubId = data.Header.LocalId;
            if (stubId == 0)
            {
                var globalServiceId = GlobalServiceId.Read(data);
                if (!localGlobalServiceIds.TryGetValue(globalServiceId, out stubId))
                    throw new Exception("Global service unavailable.");
            }

            var stub = stubs[stubId];

            var impl = stub as RPCImpl;
            if (impl != null)
            {
                switch (data.Header.CallType)
                {
                    case RPCDataType.Call:
                        impl.ProcessCall(RPCData.Call.Create(data));
                        return;
                    case RPCDataType.EventSubscribe:
                        impl.ProcessEventSubscribe(data);
                        return;
                    case RPCDataType.EventUnsubscribe:
                        impl.ProcessEventUnsubscribe(data);
                        return;
                    default:
                        throw new NotImplementedException();
                }
            }

            var proxy = (RPCProxy)stub;
            switch (data.Header.CallType)
            {
                case RPCDataType.Reply:
                    proxy.ProcessReply(RPCData.Reply.Create(data));
                    return;
                default:
                    throw new NotImplementedException();
            }
        }

        protected int GetNextStubId()
        {
            return Interlocked.Increment(ref stubIdCounter);
        }

        public T GetLocalService<T>(int implId)
        {
            var serviceAttribute = typeof (T).GetCustomAttribute<ServiceAttribute>(false);
            var globalServiceAttribute = typeof (T).GetCustomAttribute<GlobalServiceAttribute>(false);
            if (serviceAttribute == null && globalServiceAttribute == null)
                throw new Exception("Type has no attribute Service or GlobalService.");

            RPCStub stub;
            if(!stubs.TryGetValue(implId, out stub))
                throw new Exception("Local service implementation not found.");

            var impl = (RPCImpl)stub;
            return (T) impl.Impl;
        }

        public T GetRemoteService<T>(RPCPeer peer, int implId)
        {
            var serviceAttribute = typeof (T).GetCustomAttribute<ServiceAttribute>(false);
            var globalServiceAttribute = typeof (T).GetCustomAttribute<GlobalServiceAttribute>(false);
            if (serviceAttribute == null && globalServiceAttribute == null)
                throw new Exception("Type has no attribute Service or GlobalService.");

            return GetCreateProxy<T>(peer, implId);
        }

        public T GetGlobalLocalService<T>()
        {
            var globalServiceAttribute = typeof (T).GetCustomAttribute<GlobalServiceAttribute>(false);
            if (globalServiceAttribute == null)
                throw new Exception("Type has no attribute Service or GlobalService.");

            var globalServiceId = new GlobalServiceId(typeof (T));

            int serviceId;
            if(!localGlobalServiceIds.TryGetValue(globalServiceId, out serviceId))
                    throw new Exception("Global service not available.");

            return GetLocalService<T>(serviceId);
        }

        public T GetGlobalRemoteService<T>(RPCPeer peer)
        {
            var globalServiceAttribute = typeof (T).GetCustomAttribute<GlobalServiceAttribute>(false);
            if (globalServiceAttribute == null)
                throw new Exception("Type has no attribute Service or GlobalService.");

            var globalServiceId = new GlobalServiceId(typeof (T));
            var tuple = new Tuple<RPCPeer, GlobalServiceId>(peer, globalServiceId);
            RPCProxy globalServiceProxy;
            if(globalServiceProxies.TryGetValue(tuple, out globalServiceProxy))
                return (T) (object) globalServiceProxy;

            return GetRemoteService<T>(peer, 0);
        }

        public RPCImpl GetCreateImplementation<T>(T service)
        {
            var serviceAttribute = typeof (T).GetCustomAttribute<ServiceAttribute>(false);
            var globalServiceAttribute = typeof (T).GetCustomAttribute<GlobalServiceAttribute>(false);
            if (serviceAttribute == null && globalServiceAttribute == null)
                throw new Exception("Type has no attribute Service or GlobalService.");

            RPCImpl impl;
            if (impls.TryGetValue(service, out impl))
                return impl;

            var implId = GetNextStubId();

            var serviceType = typeof(T);

            var serviceAssembly = serviceType.Assembly;

            var stubsType = serviceAssembly.GetType(Helper.GetStubsClassName(serviceType, true));
            var implType = stubsType.GetNestedType("Impl");
            impl = (RPCImpl)Activator.CreateInstance(implType, service, implId);

            stubs.Add(implId, impl);
            impls.Add(service, impl);
            impl.RPCManager = RPCManager;

            if (globalServiceAttribute != null)
                localGlobalServiceIds.Add(new GlobalServiceId(typeof (T)), impl.LocalId);

            return impl;
        }

        private T GetCreateProxy<T>(RPCPeer peer, int implId)
        {
            var tuple = new Tuple<RPCPeer, int>(peer, implId);
            RPCProxy proxy;
            if (proxies.TryGetValue(tuple, out proxy))
                return (T)(object)proxy;

            var serviceType = typeof(T);
            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(Helper.GetStubsClassName(serviceType, true));
            var proxyType = implType.GetNestedType("Proxy");

            var proxyId = GetNextStubId();

            var stub = Activator.CreateInstance(proxyType, peer, implId, proxyId);
            proxy = (RPCProxy) stub;

            stubs.Add(proxyId, proxy);
            proxies.Add(tuple, proxy);
            proxy.RPCManager = RPCManager;

            return (T)stub;
        }

        internal bool HasGlobalService(Type type)
        {
            return localGlobalServiceIds.ContainsKey(new GlobalServiceId(type));
        }
    }
}
