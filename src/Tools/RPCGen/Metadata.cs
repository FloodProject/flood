using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace RPCGen
{
    public struct ExceptionInfo
    {
        public int Id;
        public Type Type;
    }

    class Metadata
    {
        public static bool IsService(Type type)
        {
            foreach (var attribute in type.GetCustomAttributes(true))
            {
                if(attribute.GetType().FullName == "Flood.RPC.Metadata.ServiceAttribute")
                    return true;
            }

            return false;
        }

        public static bool IsMessage(Type type)
        {
            foreach (var attribute in type.GetCustomAttributes(true))
            {
                if(attribute.GetType().FullName == "Flood.RPC.Metadata.MessageAttribute")
                    return true;
            }

            return false;
        }

        public static bool IsException(Type type)
        {
            foreach (var attribute in type.GetCustomAttributes(true))
            {
                if(attribute.GetType().FullName == "Flood.RPC.Metadata.ExceptionAttribute")
                    return true;
            }

            return false;
        }

        public static bool IsRequired(ICustomAttributeProvider info)
        {
            foreach (var attribute in info.GetCustomAttributes(true))
            {
                if(attribute.GetType().FullName == "Flood.RPC.Metadata.RequiredAttribute")
                    return true;
            }

            return false;
        }

        public static bool HasId(ICustomAttributeProvider info)
        {
            foreach (var attribute in info.GetCustomAttributes(true))
            {
                if (attribute.GetType().FullName == "Flood.RPC.Metadata.IdAttribute") 
                    return true;
            }
            return false;
        }

        public static bool TryGetId(ICustomAttributeProvider info, out int id)
        {
            foreach (var attribute in info.GetCustomAttributes(true))
            {
                if (attribute.GetType().FullName == "Flood.RPC.Metadata.IdAttribute") 
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
            foreach (var attribute in info.GetCustomAttributes(true))
            {
                if (attribute.GetType().FullName == "Flood.RPC.Metadata.ThrowsAttribute") 
                    return true;
            }
            return false;
        }

        public static bool TryGetThrows(ICustomAttributeProvider info, out List<ExceptionInfo> exceptionsInfo)
        {
            exceptionsInfo = new List<ExceptionInfo>();
            foreach (var attribute in info.GetCustomAttributes(true))
            {
                if (attribute.GetType().FullName == "Flood.RPC.Metadata.ThrowsAttribute") 
                { 
                    var eInfo = new ExceptionInfo();
                    eInfo.Id = (int) attribute.GetType().GetProperty("Id").GetValue(attribute);
                    eInfo.Type = (Type) attribute.GetType().GetProperty("Type").GetValue(attribute);
                    exceptionsInfo.Add(eInfo);
                }
                    
            }
            return exceptionsInfo.Count != 0;
        }




    }
}
