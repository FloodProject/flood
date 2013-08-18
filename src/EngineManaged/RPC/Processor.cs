/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Contains some contributions under the Thrift Software License.
 * Please see doc/old-thrift-license.txt in the Thrift distribution for
 * details.
 */

using System.Collections.Generic;
using System.Threading.Tasks;
using Flood.RPC.Serialization;

namespace Flood.RPC
{
    public interface Processor
    {
        Task<RPCData> Process(RPCData request);
    }


    public abstract class SimpleProcessor : Processor
    {
        protected delegate Task<RPCData> ProcessCall(RPCData request, ProcedureCall call);
        protected delegate void ProcessSubscribe();

        protected struct Processors
        {
            public ProcessCall Call;
            public ProcessSubscribe Subscribe;
            public ProcessSubscribe Unsubscribe;

            public Processors(ProcessCall call)
                : this()
            {
                Call = call;
            }

            public Processors(ProcessSubscribe subscribe, ProcessSubscribe unsubscribe)
                : this()
            {
                Subscribe = subscribe;
                Unsubscribe = unsubscribe;
            }
        }

        protected Dictionary<int, Processors> processors;

        public SimpleProcessor()
        {
            processors = new Dictionary<int, Processors>();
        }

        public async Task<RPCData> Process(RPCData request)
        {
            ProcedureCall msg = request.Serializer.ReadProcedureCallBegin();
            var fn = processors[msg.Id].Call;
            if (fn != null)
                return await fn(request, msg);
           
            var response = RPCData.CreateException(request);
            SerializerUtil.Skip(request.Serializer, TType.DataObject);
            request.Serializer.ReadProcedureCallEnd();
            RPCException x = new RPCException(RPCException.ExceptionType.UnknownMethod, "Invalid method id: '" + msg.Id + "'");
            response.Serializer.WriteProcedureCallBegin(msg);
            x.Write(response.Serializer);
            response.Serializer.WriteProcedureCallEnd();
            return response;
        }

        public void SubscribeEvent(RPCData request)
        {
            var eventId = request.Serializer.ReadI32();

            var fn = processors[eventId].Subscribe;
            //if (fn != null)
            //    fn(request.Peer, eventId);
        }
    }
}
