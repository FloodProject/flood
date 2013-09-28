using System.Collections.Generic;
using System.Linq;
using Mono.Cecil;


internal static class StringExtensions
{
    public static string UppercaseFirst(this string s)
    {
        return char.ToUpper(s[0]) + s.Substring(1);
    }

    public static string LowercaseFirst(this string s)
    {
        return char.ToLower(s[0]) + s.Substring(1);
    }

   
}

internal static class CecilExtensions
{
    public static bool HasInterface(this TypeDefinition type, string interfaceFullName)
    {
        return (type.Interfaces.Any(i => i.FullName.Equals(interfaceFullName))
                || type.BaseType.Resolve().HasInterface(interfaceFullName));
    }

    /// <summary>
    /// Gets event from type or base types
    /// </summary>
    public static EventDefinition GetEvent(this TypeDefinition type, string eventName)
    {
        var @event = type.Events.SingleOrDefault(e => e.Name.Equals(eventName));
       if(@event!=null)
            return @event;
        if(type.BaseType!=null)
            return type.BaseType.Resolve().GetEvent(eventName);
        return null;
    }

    /// <summary>
    /// Gets properties from type and base types
    /// </summary>
    public static List<PropertyDefinition> GetProperties(this TypeDefinition type)
    {
        var properties = type.Properties.ToList();
        if(type.BaseType!=null)
            properties.AddRange(GetProperties(type.BaseType.Resolve()));

        return properties;
    }
}