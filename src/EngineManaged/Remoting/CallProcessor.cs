using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace Flood.Remoting
{
    class CallProcessor
    {
        private Dictionary<int, TaskCompletionSource<Message>> pendingCalls;
        private int callIdCounter;

        public CallProcessor()
        {
            pendingCalls = new Dictionary<int, TaskCompletionSource<Message>>();
        }

        public int GetNextCallId()
        {
            return Interlocked.Increment(ref callIdCounter);
        }

        public Task<Message> DispatchCall(int callId, Message data)
        {
            var tcs = new TaskCompletionSource<Message>();
            pendingCalls.Add(callId, tcs);

            data.Peer.Dispatch(data);

            return tcs.Task;
        }

        internal void SetResult(int callId, Message data)
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
