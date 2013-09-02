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

using System.IO;
using System.Text;

namespace Flood.RPC.Serialization
{
    using Stream = System.IO.Stream;

    public abstract class Serializer
    {
        public Stream Buffer { get; private set; }

        protected Serializer(Stream stream)
        {
            Buffer = stream;
        }

        public abstract void WriteDataObjectBegin(DataObject data);
        public abstract void WriteDataObjectEnd();
        public abstract void WriteFieldBegin(Field field);
        public abstract void WriteFieldEnd();
        public abstract void WriteFieldStop();
        public abstract void WriteMapBegin(TMap map);
        public abstract void WriteMapEnd();
        public abstract void WriteListBegin(TList list);
        public abstract void WriteListEnd();
        public abstract void WriteBool(bool b);
        public abstract void WriteByte(byte b);
        public abstract void WriteI16(short i16);
        public abstract void WriteI32(int i32);
        public abstract void WriteI64(long i64);
        public abstract void WriteDouble(double d);
        public virtual void WriteString(string s) {
            WriteBinary(Encoding.UTF8.GetBytes(s));
        }
        public abstract void WriteBinary(byte[] b);

        public abstract DataObject ReadDataObjectBegin();
        public abstract void ReadDataObjectEnd();
        public abstract Field ReadFieldBegin();
        public abstract void ReadFieldEnd();
        public abstract TMap ReadMapBegin();
        public abstract void ReadMapEnd();
        public abstract TList ReadListBegin();
        public abstract void ReadListEnd();
        public abstract bool ReadBool();
        public abstract byte ReadByte();
        public abstract short ReadI16();
        public abstract int ReadI32();
        public abstract long ReadI64();
        public abstract double ReadDouble();
        public virtual string ReadString() {
            var buf = ReadBinary();
            return Encoding.UTF8.GetString(buf, 0, buf.Length);
        }
        public abstract byte[] ReadBinary();
    }
}
