using System;
using System.Collections.Generic;
using System.Reflection;

namespace RemotingGen
{
    public struct ExceptionInfo
    {
        public int Id;
        public Type Type;
    }

    public class Metadata
    {
        private static bool HasAttribute(ICustomAttributeProvider info, string fullName)
        {
            foreach (var attribute in info.GetCustomAttributes(false))
            {
                if(attribute.GetType().FullName == fullName)
                    return true;
            }

            return false;
        }

        public static bool IsService(Type type)
        {
            return HasAttribute(type, "Flood.Remoting.Metadata.ServiceAttribute")
                || HasAttribute(type, "Flood.Remoting.Metadata.GlobalServiceAttribute");
        }

        public static bool TryGetGlobalServiceId(Type type, out ushort id)
        {
            foreach (var attribute in type.GetCustomAttributes(true))
            {
                if (attribute.GetType().FullName != "Flood.Remoting.Metadata.GlobalServiceAttribute") 
                    continue;

                id = (ushort) attribute.GetType().GetProperty("Id").GetValue(attribute);
                return true;
            }
            id = 0;
            return false;
        }

        public static bool IsDataObject(Type type)
        {
            return HasAttribute(type, "Flood.Remoting.Metadata.DataObjectAttribute");
        }

        public static bool TryGetDataObjectId(Type type, out ushort id)
        {
            foreach (var attribute in type.GetCustomAttributes(true))
            {
                if (attribute.GetType().FullName != "Flood.Remoting.Metadata.DataObjectAttribute") 
                    continue;

                id = (ushort) attribute.GetType().GetProperty("Id").GetValue(attribute);
                return true;
            }
            id = 0;
            return false;
        }

        public static bool IsException(Type type)
        {
            return HasAttribute(type, "Flood.Remoting.Metadata.ExceptionAttribute");
        }

        public static bool IsRequired(ICustomAttributeProvider info)
        {
            return HasAttribute(info, "Flood.Remoting.Metadata.RequiredAttribute");
        }

        public static bool HasId(ICustomAttributeProvider info)
        {
            return HasAttribute(info, "Flood.Remoting.Metadata.IdAttribute");
        }

        public static bool TryGetId(ICustomAttributeProvider info, out int id)
        {
            foreach (var attribute in info.GetCustomAttributes(true))
            {
                if (attribute.GetType().FullName == "Flood.Remoting.Metadata.IdAttribute") 
                { 
                    id = (int) attribute.GetType().GetProperty("Id").GetValue(attribute);
                    return true;
                }
                    
            }
            id = 0;
            return false;
        }

        public static bool HasThrows(ICustomAttributeProvider info)
        {
            return HasAttribute(info, "Flood.Remoting.Metadata.ThrowsAttribute");
        }

        public static bool TryGetThrows(ICustomAttributeProvider info, out List<ExceptionInfo> exceptionsInfo)
        {
            exceptionsInfo = new List<ExceptionInfo>();
            foreach (var attribute in info.GetCustomAttributes(true))
            {
                if (attribute.GetType().FullName == "Flood.Remoting.Metadata.ThrowsAttribute") 
                { 
                    var eInfo = new ExceptionInfo();
                    eInfo.Id = (int) attribute.GetType().GetProperty("Id").GetValue(attribute);
                    eInfo.Type = (Type) attribute.GetType().GetProperty("Exception").GetValue(attribute);
                    exceptionsInfo.Add(eInfo);
                }
                    
            }
            return exceptionsInfo.Count != 0;
        }

        public static bool HasEncrypted(ICustomAttributeProvider info)
        {
            return HasAttribute(info, "Flood.Remoting.Metadata.EncryptedAttribute");
        }

        public static bool HasCompressed(ICustomAttributeProvider info)
        {
            return HasAttribute(info, "Flood.Remoting.Metadata.CompressedAttribute");
        }

        public static bool HasSigned(ICustomAttributeProvider info)
        {
            return HasAttribute(info, "Flood.Remoting.Metadata.SignedAttribute");
        }
    }
}
