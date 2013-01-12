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

using System;
using System.Collections.Generic;
using System.IO;
using Flood.RPC.Protocol;

namespace Flood.RPC
{
	public interface Processor
	{
		bool Process(Serializer iprot, Serializer oprot);
	}

    public abstract class SimpleProcessor : Processor
    {
        protected delegate void ProcessFunction(int seqid, Serializer iprot, Serializer oprot);
        protected Dictionary<string, ProcessFunction> processMap_;

        public SimpleProcessor()
        {
            processMap_ = new Dictionary<string, ProcessFunction>();
        }

        public bool Process(Serializer iprot, Serializer oprot)
        {
            try
            {
                Message msg = iprot.ReadMessageBegin();
                ProcessFunction fn;
                processMap_.TryGetValue(msg.Name, out fn);
                if (fn == null)
                {
                    ProtocolUtil.Skip(iprot, TType.Struct);
                    iprot.ReadMessageEnd();
                    RPCException x = new RPCException(RPCException.ExceptionType.UnknownMethod, "Invalid method name: '" + msg.Name + "'");
                    oprot.WriteMessageBegin(new Message(msg.Name, MessageType.Exception, msg.SeqID));
                    x.Write(oprot);
                    oprot.WriteMessageEnd();
                    oprot.Transport.Flush();
                    return true;
                }
                fn(msg.SeqID, iprot, oprot);
            }
            catch (IOException)
            {
                return false;
            }
            return true;
        }
    }
}
