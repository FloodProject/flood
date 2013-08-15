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
        protected delegate Task<RPCData> ProcessFunction(int seqid, RPCData request);
        protected Dictionary<string, ProcessFunction> processMap_;

        public SimpleProcessor()
        {
            processMap_ = new Dictionary<string, ProcessFunction>();
        }

        public async Task<RPCData> Process(RPCData request)
        {
            DataObject msg = request.Serializer.ReadDataObjectBegin();
            ProcessFunction fn;
            processMap_.TryGetValue(msg.Name, out fn);
            if (fn != null)
                return await fn(msg.SeqID, request);
           
            var response = new RPCData(request);
            response.IsResponse = true;
            SerializerUtil.Skip(request.Serializer, TType.Struct);
            request.Serializer.ReadDataObjectEnd();
            RPCException x = new RPCException(RPCException.ExceptionType.UnknownMethod, "Invalid method name: '" + msg.Name + "'");
            response.Serializer.WriteDataObjectBegin(new DataObject(msg.Name, DataObjectType.Exception, msg.SeqID));
            x.Write(response.Serializer);
            response.Serializer.WriteDataObjectEnd();
            return response;
        }
    }
}
