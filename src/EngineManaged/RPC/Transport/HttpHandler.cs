//
//  THttpHandler.cs
//
//  Authors:
//		Fredrik Hedberg <fhedberg@availo.com>
//
//  Distributed under the Apache Public License
//

using System;
using System.Web;
using System.Net;
using System.IO;

using Flood.RPC.Protocol;

namespace Flood.RPC.Transport
{
    public class THttpHandler : IHttpHandler
    {
        protected Processor processor;

        protected ProtocolFactory inputProtocolFactory;
        protected ProtocolFactory outputProtocolFactory;

        protected const string contentType = "application/x-thrift";
        protected System.Text.Encoding encoding = System.Text.Encoding.UTF8;

        public THttpHandler(Processor processor)
            : this(processor, new BinaryProtocol.Factory())
        {

        }
        
        public THttpHandler(Processor processor, ProtocolFactory protocolFactory)
            : this(processor, protocolFactory, protocolFactory)
        {

        }

        public THttpHandler(Processor processor, ProtocolFactory inputProtocolFactory, ProtocolFactory outputProtocolFactory)
        {
            this.processor = processor;
            this.inputProtocolFactory = inputProtocolFactory;
            this.outputProtocolFactory = outputProtocolFactory;
        }

        public void ProcessRequest(HttpListenerContext context)
        {
            context.Response.ContentType = contentType;
            context.Response.ContentEncoding = encoding;
            ProcessRequest(context.Request.InputStream, context.Response.OutputStream);
        }

        public void ProcessRequest(HttpContext context)
        {
            context.Response.ContentType = contentType;
            context.Response.ContentEncoding = encoding;
            ProcessRequest(context.Request.InputStream, context.Response.OutputStream);
        }

        public void ProcessRequest(Stream input, Stream output)
        {
            TTransport transport = new TStreamTransport(input,output);

            Serializer inputProtocol = null;
            Serializer outputProtocol = null;

            try
            {
                inputProtocol = inputProtocolFactory.GetProtocol(transport);
                outputProtocol = outputProtocolFactory.GetProtocol(transport);

                while (processor.Process(inputProtocol, outputProtocol)) { }
            }
            catch (TTransportException)
            {
                // Client died, just move on
            }
            catch (RPCException tx)
            {
                Console.Error.Write(tx);
            }
            catch (Exception x)
            {
                Console.Error.Write(x);
            }

            transport.Close();
        }

        public bool IsReusable
        {
            get { return true; }
        }
    }
}
