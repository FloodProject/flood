using System.Threading.Tasks;
using Flood;
using Flood.RPC.Metadata;
using Flood.RPC.Protocol;
using System;
using System.Collections.Generic;

namespace EngineManaged.Network
{

    [Message]
    public class ServiceId
    {
        [Id(0)]
        public int Id;

        [Id(1)]
        public string Type;
    }

    [Service]
    public interface IServiceManager
    {
        Task<ServiceId> GetServiceId([Id(0)]string serviceName);
    }


    public abstract class ServiceManager
    {
        internal readonly Dictionary<int, ServiceHandler> services;

        internal readonly Queue<ServiceData> inData;
        internal readonly Queue<ServiceData> outData;

        protected ServiceManager()
        {
            services = new Dictionary<int,ServiceHandler>();
            inData = new Queue<ServiceData>();
            outData = new Queue<ServiceData>();
        }

        protected bool ContainsServiceHandler(int serviceId)
        {
            return services.ContainsKey(serviceId);
        }

        internal ServiceHandler GetServiceHandler(int serviceId)
        {
            if (!services.ContainsKey(serviceId))
                throw new ArgumentException("Service " + serviceId + " not added");

            return services[serviceId];
        }

        internal void AddServiceHandler(int serviceId, ServiceHandler service)
        {
            if (services.ContainsKey(serviceId))
                throw new ArgumentException("Service " + serviceId + " already added");

            services.Add(serviceId, service);
        }

        internal void AddInData(ServiceData @in)
        {
            inData.Enqueue(@in);
        }

        internal ServiceData GetOutData()
        {
            if(outData.Count == 0)
                return null;

            return outData.Dequeue();
        }

        public abstract void Process();
        
    }


    public class RemoteServiceManager : ServiceManager
    {
        private IServiceManager serviceManager;

        private Peer peer;

        public RemoteServiceManager(Peer peer)
        {
            this.peer = peer;
            var serviceManagerProxy = new ServiceProxy<IServiceManager>(0);
            AddServiceHandler(0, (ServiceHandler) serviceManagerProxy);
            serviceManager = serviceManagerProxy.Service;
        }

        public T GetService<T>(int serviceId)
        {
            if (!ContainsServiceHandler(serviceId))
            {
                //TODO verify if service is accessible
                var service = new ServiceProxy<T>(serviceId);
                AddServiceHandler(serviceId, service);
            }

            return ((ServiceProxy<T>) GetServiceHandler(serviceId)).Service;
        }

        public override void Process()
        {
            if(inData.Count == 0)
                return;

            var @in = inData.Dequeue();
            var serviceProxy = (ServiceProxy) GetServiceHandler(@in.ServiceId);

            serviceProxy.WriteResponses(@in.Data);

            foreach(var serviceKV in services)
            {
                var data = ((ServiceProxy) serviceKV.Value).ReadRequests();
            
                if(data == null)
                    return;

                var @out = new ServiceData()
                {
                    Peer = peer,
                    ServiceId = serviceKV.Key,
                    Data = data
                };

                outData.Enqueue(@out);
            }
            
        }
    }

    public class LocalServiceManager : ServiceManager, IServiceManager
    {
        private int localIdCounter = 0;

        private Dictionary<string, int> serviceIds;

        public LocalServiceManager()
        {
            serviceIds = new Dictionary<string, int>();

            AddService<IServiceManager>(this); // serviceId = 0
        }

        public void AddService<T>(T service)
        {
            var serviceName = typeof (T).FullName;
            if(serviceIds.ContainsKey(serviceName))
                throw new ArgumentException("Service "+serviceName+" already added");

            serviceIds.Add(serviceName, localIdCounter);

            var serviceImpl = new ServiceImplementation<T>(service);

            AddServiceHandler(localIdCounter, serviceImpl);
            localIdCounter++;
        }

        public T GetService<T>(int serviceId)
        {
            return ((ServiceImplementation<T>) GetServiceHandler(serviceId)).Service;
        }

        public async Task<ServiceId> GetServiceId(string serviceName)
        {
            if (serviceIds.ContainsKey(serviceName))
                throw new ArgumentException("Service " + serviceName + " not added");

            var serviceId = new ServiceId()
            {
                Id = serviceIds[serviceName],
                Type = serviceName
            };

            return serviceId;
        }

        public override void Process()
        {
            if(inData.Count == 0)
                return;

            var @in = inData.Dequeue();
            var serviceImpl = (ServiceImplementation) GetServiceHandler(@in.ServiceId);

            var data = serviceImpl.Process(@in.Data);

            if(data == null)
                return;

            var @out = new ServiceData()
            {
                Peer = @in.Peer,
                ServiceId = @in.ServiceId,
                Data = data
            };

            outData.Enqueue(@out);
            
        }

    }
}
