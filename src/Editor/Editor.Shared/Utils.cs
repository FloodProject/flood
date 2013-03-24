using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Editor.Shared
{
    class Utils
    {
        public static IEnumerable<FieldInfo> GetAllFields(Type t)
        {
            if (t == null)
                return Enumerable.Empty<FieldInfo>();

            BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly;
            return t.GetFields(flags).Union(GetAllFields(t.BaseType));
        }

        public static IEnumerable<PropertyInfo> GetAllProperties(Type t)
        {
            if (t == null)
                return Enumerable.Empty<PropertyInfo>();

            BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly;
            return t.GetProperties(flags).Union(GetAllProperties(t.BaseType));
        }

        public static FieldInfo GetField(Type t, string name)
        {
            var type = t;
            BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly;
            FieldInfo finfo = null;
            while (t != null)
            {
                finfo = t.GetField(name, flags) ?? t.GetField(UnTitleCase(name), flags);
                if (finfo != null)
                    return finfo;

                t = t.BaseType;
            }
            return finfo;
        }

        public static PropertyInfo GetProperty(Type t, string name)
        {
            var type = t;
            BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly;
            PropertyInfo pinfo = null;
            while (t != null)
            {
                pinfo = t.GetProperty(name, flags) ?? t.GetProperty(UnTitleCase(name), flags);
                if (pinfo != null)
                    return pinfo;

                t = t.BaseType;
            }
            return pinfo;
        }


        public static string UnTitleCase(string str)
        {
            var result = str;
            if (!string.IsNullOrEmpty(str))
            {
                var words = str.Split(' ');
                for (int index = 0; index < words.Length; index++)
                {
                    var s = words[index];
                    if (s.Length > 0)
                    {
                        words[index] = s[0].ToString(CultureInfo.InvariantCulture)
                            .ToLower() + s.Substring(1);
                    }
                }
                result = string.Join(" ", words);
            }
            return result;
        }
    }
}
