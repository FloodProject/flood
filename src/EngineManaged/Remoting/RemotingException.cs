using System;
using Flood.Remoting.Serialization;

namespace Flood.Remoting
{
    public class RemotingException : Exception
    {
        protected ExceptionType type;

        public RemotingException()
        {
        }

        public RemotingException(ExceptionType type)
        {
            this.type = type;
        }

        public RemotingException(ExceptionType type, string message)
            : base(message)
        {
            this.type = type;
        }

        public static RemotingException Read(Serializer iprot)
        {
            Field field;

            string message = null;
            ExceptionType type = ExceptionType.Unknown;

            iprot.ReadDataObjectBegin();
            while (true)
            {
                field = iprot.ReadFieldBegin();
                if (field.Type == TType.Stop)
                {
                    break;
                }

                switch (field.ID)
                {
                    case 1:
                        if (field.Type == TType.String)
                        {
                            message = iprot.ReadString();
                        }
                        else
                        {
                            SerializerUtil.Skip(iprot, field.Type);
                        }
                        break;
                    case 2:
                        if (field.Type == TType.I32)
                        {
                            type = (ExceptionType)iprot.ReadI32();
                        }
                        else
                        {
                            SerializerUtil.Skip(iprot, field.Type);
                        }
                        break;
                    default:
                        SerializerUtil.Skip(iprot, field.Type);
                        break;
                }

                iprot.ReadFieldEnd();
            }

            iprot.ReadDataObjectEnd();

            return new RemotingException(type, message);
        }

        public void Write(Serializer oprot)
        {
            DataObject struc = new DataObject("TApplicationException");
            Field field = new Field();

            oprot.WriteDataObjectBegin(struc);

            if (!String.IsNullOrEmpty(Message))
            {
                field.Name = "message";
                field.Type = TType.String;
                field.ID = 1;
                oprot.WriteFieldBegin(field);
                oprot.WriteString(Message);
                oprot.WriteFieldEnd();
            }

            field.Name = "type";
            field.Type = TType.I32;
            field.ID = 2;
            oprot.WriteFieldBegin(field);
            oprot.WriteI32((int)type);
            oprot.WriteFieldEnd();
            oprot.WriteFieldStop();
            oprot.WriteDataObjectEnd();
        }

        public enum ExceptionType
        {
            Unknown,
            UnknownMethod,
            InvalidMessageType,
            WrongMethodName,
            BadSequenceID,
            MissingResult,
            InternalError,
            ProtocolError,
            InvalidTransform,
            InvalidProtocol,
            UnsupportedClientType
        }
    }
}
