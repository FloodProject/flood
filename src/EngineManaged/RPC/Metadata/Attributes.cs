using System;
using System.Collections.Generic;

namespace Flood.RPC.Metadata
{
    [AttributeUsage(AttributeTargets.Interface)]
    public class ServiceAttribute : System.Attribute
    {

    }

    [AttributeUsage(AttributeTargets.Interface)]
    public class GlobalServiceAttribute : System.Attribute
    {
        public ushort Id { get; private set; }

        public GlobalServiceAttribute(ushort id)
        {
            Id = id;
        }
    }

    [AttributeUsage(AttributeTargets.Class)]
    public class DataObjectAttribute : System.Attribute
    {
        public ushort Id { get; private set; }

        public DataObjectAttribute(ushort id)
        {
            Id = id;
        }
    }

    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Parameter |
        AttributeTargets.Property | AttributeTargets.Method | AttributeTargets.Event)]
    public class IdAttribute : System.Attribute
    {
        public IdAttribute(int id)
        {
            Id = id;
        }

        public int Id { get; set; }
    }

    [AttributeUsage(AttributeTargets.Field)]
    public class DefaultAttribute : System.Attribute
    {
        public DefaultAttribute(object value)
        {
            Value = value;
        }

        public object Value { get; set; }
    }

    [AttributeUsage(AttributeTargets.Field)]
    public class RequiredAttribute : System.Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Field)]
    public class OptionalAttribute : System.Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class OnewayAttribute : System.Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Class)]
    public class ExceptionAttribute : System.Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Method, AllowMultiple = true)]
    public class ThrowsAttribute : System.Attribute
    {
        public ThrowsAttribute(int id, Type type)
        {
            Id = id;
            Exception = type;
        }

        public int Id { get; private set; }
        public Type Exception { get; private set; }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class EncryptedAttribute : System.Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class CompressedAttribute : System.Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class SignedAttribute : System.Attribute
    {
    }
}
