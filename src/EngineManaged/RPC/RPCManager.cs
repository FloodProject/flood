using System.Reflection;
using System.Threading.Tasks;
using Flood.RPC.Metadata;
using Flood.RPC.Serialization;
using System;
using System.Collections.Generic;
using System.Threading;

namespace Flood.RPC
{
    public class RPCManager
    {
        public RPCPeer Local { get; private set; }

        private Dictionary<RPCStubId, RPCStub> stubs;
        private Dictionary<object, RPCImpl> impls;
        private Dictionary<Tuple<RPCPeer, RPCStubId>, RPCProxy> proxies;
        private Dictionary<Type, RPCStubId> globalServiceIds; 

        private int stubIdCounter;

        public RPCManager(RPCPeer local)
        {
            stubs = new Dictionary<RPCStubId, RPCStub>();
            impls = new Dictionary<object, RPCImpl>();
            proxies = new Dictionary<Tuple<RPCPeer, RPCStubId>, RPCProxy>();
            globalServiceIds = new Dictionary<Type, RPCStubId>();

            Local = local;
        }

        protected RPCStubId GetNextStubId()
        {
            return new RPCStubId(Interlocked.Increment(ref stubIdCounter));
        }

        public void Process(byte[] data, RPCPeer peer)
        {
            var serializer = new BinarySerializer();
            serializer.Buffer.Write(data, 0, data.Length);
            serializer.Buffer.Position = 0;

            var rpcData = new RPCData(peer, serializer);
            rpcData.Header.Read();

            Process(rpcData);
        }

        public void Process(RPCData data)
        {
            var id = data.Header.LocalId;

            var stub = stubs[id];

            switch(data.Header.CallType)
            {
                case RPCDataType.DelegateReply:
                    stub.ProcessDelegateReply(RPCData.DelegateReply.Create(data));
                    return;
                case RPCDataType.DelegateCall:
                    stub.ProcessDelegateCall(RPCData.DelegateCall.Create(data));
                    return;
            }

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

        public T GetService<T>(RPCPeer peer, RPCStubId implId)
        {
            var serviceAttribute = typeof (T).GetCustomAttribute<ServiceAttribute>(false);
            var globalServiceAttribute = typeof (T).GetCustomAttribute<GlobalServiceAttribute>(false);
            if (serviceAttribute == null && globalServiceAttribute == null)
                throw new Exception("Type has no attribute Service or GlobalService.");

            if (peer.Equals(Local))
            {
                RPCStub stub;
                if(!stubs.TryGetValue(implId, out stub))
                    throw new Exception("Local service implementation not found.");

                var impl = (RPCImpl)stub;
                return (T) impl.Impl;
            }

            return GetCreateProxy<T>(peer, implId);
        }

        public T GetGlobalService<T>(RPCPeer peer)
        {
            var attribute = typeof (T).GetCustomAttribute<GlobalServiceAttribute>(false);
            if(attribute == null)
                throw new Exception("Type has no attribute GlobalService.");

            RPCStubId serviceId;
            if (peer.Equals(Local))
            {
                if(!globalServiceIds.TryGetValue(typeof(T), out serviceId))
                    throw new Exception("Global service not available.");

                return GetService<T>(peer, serviceId);
            }

            var versionMajor = typeof (T).Assembly.GetName().Version.Major;
            serviceId = new RPCStubId(attribute.Guid, versionMajor);
            return GetService<T>(peer, serviceId);
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
            if (globalServiceAttribute != null)
            {
                implId.Guid = globalServiceAttribute.Guid;
                implId.VersionMajor = typeof (T).Assembly.GetName().Version.Major;
            }

            var serviceType = typeof(T);

            var serviceAssembly = serviceType.Assembly;

            var stubsType = serviceAssembly.GetType(Helper.GetStubsClassName(serviceType, true));
            var implType = stubsType.GetNestedType("Impl");
            impl = (RPCImpl)Activator.CreateInstance(implType, service, implId);

            stubs.Add(implId, impl);
            impls.Add(service, impl);
            impl.RPCManager = this;

            if (globalServiceAttribute != null)
                globalServiceIds.Add(typeof(T), impl.Id);

            return impl;
        }

        private T GetCreateProxy<T>(RPCPeer peer, RPCStubId implId)
        {
            var tuple = new Tuple<RPCPeer, RPCStubId>(peer, implId);
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
            proxy.RPCManager = this;

            return (T)stub;
        }
    }
}
