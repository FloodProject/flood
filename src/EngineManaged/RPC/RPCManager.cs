using Flood.RPC.Serialization;
using System;
using System.Collections.Generic;
using System.Threading;

namespace Flood.RPC
{
    public class RPCManager
    {
        private Dictionary<int, RPCStub> stubs;
        private Dictionary<object, RPCImpl> impls;
        private Dictionary<Tuple<RPCPeer, int>, RPCProxy> proxies;

        private int stubIdCounter;

        public RPCManager()
        {
            stubs = new Dictionary<int, RPCStub>();
            impls = new Dictionary<object, RPCImpl>();
            proxies = new Dictionary<Tuple<RPCPeer, int>, RPCProxy>();
        }

        protected int GetNextStubId()
        {
            return Interlocked.Increment(ref stubIdCounter);
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

        public RPCImpl GetCreateImplementation<T>(T service)
        {
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
            impl.RPCManager = this;

            return impl;
        }

        public T GetCreateProxy<T>(RPCPeer peer, int implId)
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
            proxy.RPCManager = this;

            return (T)stub;
        }
    }
}
