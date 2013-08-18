using Flood.RPC.Serialization;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public abstract class RPCProxy
    {
        private Dictionary<int, TaskCompletionSource<RPCData>> pendingCalls;
        private int sequenceNumber;

        protected RPCProxy()
        {
            pendingCalls = new Dictionary<int, TaskCompletionSource<RPCData>>();
        }

        protected int GetNextSequenceNumber()
        {
            return sequenceNumber++;
        }

        public abstract void InvokeEvent(RPCData data);

        protected Task<RPCData> DispatchCall(RPCData data, int seqNumber)
        {
            var tcs = new TaskCompletionSource<RPCData>();
            pendingCalls.Add(seqNumber, tcs);

            data.Peer.Dispatch(data);

            return tcs.Task;
        }

        internal void ProcessReply(RPCData reply)
        {
            var call = reply.Serializer.ReadProcedureCallBegin();
            if (!pendingCalls.ContainsKey(call.SequenceNumber)) 
            {
                Log.Error("Received unexpected reply.");
                return;
            }

            pendingCalls[call.SequenceNumber].SetResult(reply);
        }
    }

    public class RPCProxyManager
    {
        private Dictionary<int, RPCProxy> proxies;
        private int proxyCounter;

        private int sequenceNumber;

        public RPCProxyManager()
        {
            proxies = new Dictionary<int, RPCProxy>();
        }

        internal void Process(RPCData data)
        {
            RPCProxy proxy;
            if (!proxies.TryGetValue(data.Header.ProxyId, out proxy))
            {
                Log.Error("Received RPC data to unexpected Proxy.");
                return;
            }

            switch (data.Header.CallType)
            {
            case RPCDataType.Reply:
                proxy.ProcessReply(data);
                return;
            case RPCDataType.EventInvoke:
                proxy.InvokeEvent(data);
                return;
            default:
                throw new NotImplementedException();
            }
        }

        internal T CreateProxy<T>(RPCPeer peer, int implId)
        {
            var serviceType = typeof(T);
            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(Helper.ImplName(serviceType, true));
            var proxyType = implType.GetNestedType("Proxy");

            var proxyId = proxyCounter++;

            var proxy = Activator.CreateInstance(proxyType, peer, implId, proxyId);

            proxies.Add(proxyId, (RPCProxy)proxy);

            return (T)proxy;
        }
    }
}
