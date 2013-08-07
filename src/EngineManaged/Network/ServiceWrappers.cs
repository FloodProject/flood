using System.Threading.Tasks;
using Flood.RPC;
using Flood;
using System;

namespace EngineManaged.Network
{
    abstract class ServiceHandler
    {

        protected object service;

        public object GetService()
        {
            return service;
        }
    }

    abstract class ServiceImplementation : ServiceHandler
    {
        public ServiceImplementation(object service, Type serviceType)
        {
            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(serviceType.FullName + "Impl");
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

    class ServiceImplementation<T> : ServiceImplementation
    {
        public ServiceImplementation(T service)
            :base(service, typeof(T))
        {}

        public T Service { get { return (T) GetService(); } }
    }

    class ServiceProxy : ServiceHandler
    {
        public ServiceProxy(IRPCManager rpcManager, Session session, int id, Type serviceType)
        {
            if(session == null)
                throw new ArgumentNullException("session");

            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(serviceType.FullName + "Impl");
            var proxyType = implType.GetNestedType("Client");

            service = Activator.CreateInstance(proxyType, rpcManager, session, id);
            Id = id;
        }

        public int Id { get; private set; }
    }

    class ServiceProxy<T> : ServiceProxy
    {
        public ServiceProxy(IRPCManager rpcManager, Session session, int id)
            :base(rpcManager, session, id, typeof(T))
        {}

        public T Service { get { return (T) GetService(); } }
    }
}
