using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Flood.RPC
{
    class ServiceImpl
    {
        protected object service;

        public ServiceImpl(object service, Type serviceType)
        {
            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(Helper.ImplName(serviceType, true));
            var processorType = implType.GetNestedType("Processor");

            processor = (Processor) Activator.CreateInstance(processorType, service);
            this.service = service;
        }

        private Processor processor;

        public Task<RPCData> Process(RPCData request)
        {
            return processor.Process(request);
        }
    }

    public class RPCImplManager
    {
        private Dictionary<int, ServiceImpl> ServiceImpls;
        private int implCounter;

        public RPCImplManager()
        {
            ServiceImpls = new Dictionary<int, ServiceImpl>();
        }

        internal void Process(RPCData call)
        {
            var serviceId = call.Header.ServiceId;

            ServiceImpl serviceImpl;
            if(!ServiceImpls.TryGetValue(serviceId, out serviceImpl))
            {
                Log.Error("ServiceImpl " + serviceId + " not found for peer:" + call.Peer);
                return;
            }

            var peer = call.Peer;

            var task = serviceImpl.Process(call);

            task.ContinueWith( reply => {
                peer.DispatchCall(reply.Result);
            });
        }

        public int AddImplementation<T>(T service)
        {
            var impl = new ServiceImpl(service, typeof(T));
            var serviceId = implCounter++;
            ServiceImpls.Add(serviceId, impl);
            return serviceId;
        }
    }
}
