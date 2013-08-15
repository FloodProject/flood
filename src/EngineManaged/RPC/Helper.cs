using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.RPC
{
    public class Helper
    {
        public static string ImplName(Type type, bool fullName)
        {
            var name = type.FullName;
            if (!fullName && name.Contains('.'))
            {
                var names = name.Split('.');
                name = names[names.Length - 1];
            }

            return name.Replace("+", "_") + "Impl";
        }
    }
}
