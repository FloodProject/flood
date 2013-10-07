using System;
using System.Collections.Generic;
using System.Linq;
using Mono.Cecil;
using Weaver;


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

    public static void SetAccessModifier(this MethodDefinition method, AccessModifiers access)
    {
        method.Attributes &= ~(MethodAttributes.Family | MethodAttributes.Assembly |
                                MethodAttributes.Private | MethodAttributes.Public);

        switch (access)
        {
            case AccessModifiers.Protected:
                method.Attributes |= MethodAttributes.Family;
                return;
            case AccessModifiers.Internal:
                method.Attributes |= MethodAttributes.Assembly;
                return;
            case AccessModifiers.Private:
                method.Attributes |= MethodAttributes.Private;
                return;
            case AccessModifiers.Public:
                method.Attributes |= MethodAttributes.Public;
                return;
            default:
                throw new Exception();
        }
    }
}