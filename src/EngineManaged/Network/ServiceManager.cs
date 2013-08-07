using System.Threading.Tasks;
using Flood.RPC;
using Flood;
using Flood.RPC.Metadata;
using Flood.RPC.Serialization;
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

    public class ServiceManager : IServiceManager, IRPCManager
    {
        private int localIdCounter;
        private int requestIdCounter;

        private Dictionary<RPCData, TaskCompletionSource<RPCData>> pendingRPC;

        internal readonly Dictionary<int, ServiceImplementation> implementations;

        private Dictionary<string, int> namedServicesIds;

        public Queue<RPCData> Data { get; private set; }

        public ServiceManager()
        {
            pendingRPC = new Dictionary<RPCData, TaskCompletionSource<RPCData>>();
            namedServicesIds = new Dictionary<string, int>();
            implementations = new Dictionary<int, ServiceImplementation>();
            Data = new Queue<RPCData>();

            AddImplementation<IServiceManager>(this); // serviceId = 0


        }

        public void AddImplementation<T>(T service)
        {
            var serviceName = typeof (T).FullName;
            if(namedServicesIds.ContainsKey(serviceName))
                throw new ArgumentException("Service "+serviceName+" already added");

            namedServicesIds.Add(serviceName, localIdCounter);

            var serviceImpl = new ServiceImplementation<T>(service);

            implementations.Add(localIdCounter, serviceImpl);
            localIdCounter++;
        }

        public T GetProxy<T>(Session session, int serviceId)
        {
            return new ServiceProxy<T>(this, session, serviceId).Service;
        }

        public async Task<ServiceId> GetServiceId(string serviceName)
        {
            if (namedServicesIds.ContainsKey(serviceName))
                throw new ArgumentException("Service " + serviceName + " not added");

            var serviceId = new ServiceId()
            {
                Id = namedServicesIds[serviceName],
                Type = serviceName
            };

            return serviceId;
        }

        public void Process(RPCData data)
        {
            if (data.IsResponse)
            {
                ProcessResponse(data);
                return;
            }

            ProcessRequest(data);
        }

        private void ProcessRequest(RPCData data)
        {
            if (!implementations.ContainsKey(data.ServiceId))
                return;

            var responseTask = implementations[data.ServiceId].Process(data);
            responseTask.ContinueWith(task => Data.Enqueue(task.Result) );
        }

        private void ProcessResponse(RPCData data)
        {
            if (!pendingRPC.ContainsKey(data))
                return;

            pendingRPC[data].SetResult(data);
        }

        public Task<RPCData> RemoteProcedureCall(RPCData data)
        {
            data.RequestId = requestIdCounter++;
            data.IsResponse = false;

            Data.Enqueue(data);

            var tcs = new TaskCompletionSource<RPCData>();
            pendingRPC.Add(data, tcs);

            return tcs.Task;
        }
    }
}
