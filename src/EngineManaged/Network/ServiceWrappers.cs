using Flood.RPC;
using Flood.RPC.Protocol;
using Flood.RPC.Transport;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EngineManaged.Network
{
    abstract class ServiceHandler
    {

        protected MemoryStream input;
        protected MemoryStream output;
        protected TStreamTransport transport;
        protected BinarySerializer serializer;

        protected object service;

        protected ServiceHandler()
        {
            input = new MemoryStream();
            output = new MemoryStream();
            transport = new TStreamTransport(input, output);
            serializer = new BinarySerializer(transport);
        }

        public object GetService()
        {
            return service;
        }

        protected byte[] Read()
        {
            var data = new byte[output.Length];
            output.Read(data, 0, (int)output.Length);
            output.Flush();
            return data;
        }

        protected void Write(byte[] data)
        {
            input.Write(data, 0, data.Length);
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

        public byte[] Process(byte[] request)
        {
            Write(request);
            processor.Process(serializer,serializer);
            return Read();
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
        public ServiceProxy(int id, Type serviceType)
        {
            var serviceAssembly = serviceType.Assembly;

            var implType = serviceAssembly.GetType(serviceType.FullName + "Impl");
            var proxyType = implType.GetNestedType("Client");

            service = Activator.CreateInstance(proxyType, serializer);
            Id = id;
        }

        public int Id { get; private set; }

        public byte[] ReadRequests()
        {
            return Read();
        }

        public void WriteResponses(byte[] responses)
        {
            Write(responses);
        }
    }

    class ServiceProxy<T> : ServiceProxy
    {
        public ServiceProxy(int id)
            :base(id, typeof(T))
        {}

        public T Service { get { return (T) GetService(); } }
    }
}
