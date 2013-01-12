using System;
using System.Collections.Generic;

namespace Flood.RPC.Metadata
{
    [AttributeUsage(AttributeTargets.Interface)]
    public class ServiceAttribute : System.Attribute
    {

    }

    [AttributeUsage(AttributeTargets.Struct)]
    public class MessageAttribute : System.Attribute
    {

    }

    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Parameter)]
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

        public int Id;
        public Type Exception;
    }
}
