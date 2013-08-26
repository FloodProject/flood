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

        public int GetNextCallId()
        {
            return Interlocked.Increment(ref callIdCounter);
        }

        public Task<RPCData> DispatchCall(int callId, RPCData data)
        {
            var tcs = new TaskCompletionSource<RPCData>();
            pendingCalls.Add(callId, tcs);

            data.Peer.Dispatch(data);

            return tcs.Task;
        }

        internal void SetResult(int callId, RPCData data)
        {
            if (!pendingCalls.ContainsKey(callId))
            {
                Log.Error("Received unexpected reply.");
                return;
            }

            pendingCalls[callId].SetResult(data);
        }
    }
}
