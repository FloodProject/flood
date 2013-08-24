using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public class RPCImplManager
    {
        private Dictionary<int, RPCImpl> impls;
        private int implCounter;

        public RPCImplManager()
        {
            impls = new Dictionary<int, RPCImpl>();
        }

        public RPCImpl this[int id]
        {
            get { return impls[id]; }
        }

        protected int GetNextImplementationId()
        {
            return Interlocked.Increment(ref implCounter);
        }

        public int AddImplementation<T>(T service)
        {
            var implId = GetNextImplementationId();
            var seriveType = typeof(T);

            var serviceAssembly = seriveType.Assembly;

            var stubsType = serviceAssembly.GetType(Helper.GetStubsClassName(seriveType, true));
            var implType = stubsType.GetNestedType("Impl");
            var impl = (RPCImpl)Activator.CreateInstance(implType, service);

            impls.Add(implId, impl);
            return implId;
        }
    }
}
