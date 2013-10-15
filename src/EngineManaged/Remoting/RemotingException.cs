using Flood.Serialization;
using System;

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
            string message = null;
            ExceptionType type = ExceptionType.Unknown;

            message = iprot.ReadString();
            type = (ExceptionType)iprot.ReadI32();

            return new RemotingException(type, message);
        }

        public void Write(Serializer oprot)
        {
            oprot.WriteString(Message);
            oprot.WriteI32((int)type);
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
