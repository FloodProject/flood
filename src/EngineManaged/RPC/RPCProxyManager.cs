using Flood.RPC.Serialization;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public class RPCProxyManager
    {
        private RPCPeer peer;
        private Dictionary<int, TaskCompletionSource<RPCData>> pendingCalls;
        private int sequenceNumber;

        public RPCProxyManager(RPCPeer peer)
        {
            if (peer == null)
                throw new ArgumentNullException("peer");

            this.peer = peer;
            pendingCalls = new Dictionary<int, TaskCompletionSource<RPCData>>();
        }

        internal void Process(RPCData reply)
        {
            switch (reply.Header.CallType)
            {
            case ProcedureCallType.Reply:
            {
                var seqNum = reply.Header.SequenceNumber;
                if (!pendingCalls.ContainsKey(seqNum)) 
                {
                    Log.Error("Received unexpected reply.");
                    return;
                }

                pendingCalls[seqNum].SetResult(reply);
                return;
            }
            default:
                throw new NotImplementedException();
            }
        }

        public RPCData CreateCall(int serviceId, RPCFlags flags = RPCFlags.None)
        {
            var serializer = new BinarySerializer();
            var call = new RPCData(serializer);
            call.Peer = peer;
            call.Flags = flags;
            call.Header.SequenceNumber = sequenceNumber++;
            call.Header.CallType = ProcedureCallType.Call;
            call.Header.ServiceId = serviceId;
            call.Header.Write();

            return call;
        }

        public Task<RPCData> DispatchCall(RPCData data)
        {
            var tcs = new TaskCompletionSource<RPCData>();
            pendingCalls.Add(data.Header.SequenceNumber, tcs);

            data.Peer.DispatchCall(data);

            return tcs.Task;
        }

        internal T CreateProxy<T>(int serviceId)
        {
            var serviceType = typeof(T);
            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(Helper.ImplName(serviceType, true));
            var proxyType = implType.GetNestedType("Proxy");

            return (T)Activator.CreateInstance(proxyType, this, serviceId);
        }
    }
}
