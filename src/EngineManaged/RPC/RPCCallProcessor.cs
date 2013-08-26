using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace Flood.RPC
{
    class RPCCallProcessor
    {
        private Dictionary<int, TaskCompletionSource<RPCData>> pendingCalls;
        private int callIdCounter;

        public RPCCallProcessor()
        {
            pendingCalls = new Dictionary<int, TaskCompletionSource<RPCData>>();
        }

        public RPCData.Call CreateCall(int methodId, RPCPeer peer, int localId, int remoteId, bool isDelegate)
        {
            var id = Interlocked.Increment(ref callIdCounter);
            return new RPCData.Call(id, methodId, peer, localId, remoteId, isDelegate);
        }

        public Task<RPCData> DispatchCall(RPCData.Call call)
        {
            var tcs = new TaskCompletionSource<RPCData>();
            pendingCalls.Add(call.Id, tcs);

            call.Data.Peer.Dispatch(call.Data);

            return tcs.Task;
        }

        internal void ProcessReply(RPCData.Reply reply)
        {
            if (!pendingCalls.ContainsKey(reply.Id))
            {
                Log.Error("Received unexpected reply.");
                return;
            }

            pendingCalls[reply.Id].SetResult(reply.Data);
        }
    }
}
