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


namespace Flood.Serialization
{
    public static class SerializerUtil
    {
        public static void Skip(Serializer prot, DataType type)
        {
            switch (type)
            {
                case DataType.Bool:
                    prot.ReadBool();
                    break;
                case DataType.Byte:
                    prot.ReadByte();
                    break;
                case DataType.I16:
                    prot.ReadI16();
                    break;
                case DataType.I32:
                    prot.ReadI32();
                    break;
                case DataType.I64:
                    prot.ReadI64();
                    break;
                case DataType.Double:
                    prot.ReadDouble();
                    break;
                case DataType.String:
                    // Don't try to decode the string, just skip it.
                    prot.ReadBinary();
                    break;
                case DataType.DataObject:
                    prot.ReadDataObjectBegin();
                    while (true)
                    {
                        Field field = prot.ReadFieldBegin();
                        if (field.Type == DataType.Stop)
                        {
                            break;
                        }
                        Skip(prot, field.Type);
                        prot.ReadFieldEnd();
                    }
                    prot.ReadDataObjectEnd();
                    break;
                case DataType.Map:
                    TMap map = prot.ReadMapBegin();
                    for (int i = 0; i < map.Count; i++)
                    {
                        Skip(prot, map.KeyType);
                        Skip(prot, map.ValueType);
                    }
                    prot.ReadMapEnd();
                    break;
                case DataType.List:
                    TList list = prot.ReadListBegin();
                    for (int i = 0; i < list.Count; i++)
                    {
                        Skip(prot, list.ElementType);
                    }
                    prot.ReadListEnd();
                    break;
            }
        }
    }
}
