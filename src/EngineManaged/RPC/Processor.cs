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
        protected delegate Task<RPCData> ProcessFunction(RPCData request);
        protected Dictionary<int, ProcessFunction> processMap_;

        public SimpleProcessor()
        {
            processMap_ = new Dictionary<int, ProcessFunction>();
        }

        public async Task<RPCData> Process(RPCData request)
        {
            ProcedureCall msg = request.Serializer.ReadProcedureCallBegin();
            ProcessFunction fn;
            processMap_.TryGetValue(msg.Id, out fn);
            if (fn != null)
                return await fn(request);
           
            var response = new RPCData(new BinarySerializer());
            response.Header.ServiceId = request.Header.ServiceId;
            response.Header.SequenceNumber = request.Header.SequenceNumber;
            response.Header.CallType = ProcedureCallType.Exception;
            response.Header.Write();
            SerializerUtil.Skip(request.Serializer, TType.DataObject);
            request.Serializer.ReadProcedureCallEnd();
            RPCException x = new RPCException(RPCException.ExceptionType.UnknownMethod, "Invalid method id: '" + msg.Id + "'");
            response.Serializer.WriteProcedureCallBegin(new ProcedureCall(msg.Id));
            x.Write(response.Serializer);
            response.Serializer.WriteProcedureCallEnd();
            return response;
        }
    }
}
