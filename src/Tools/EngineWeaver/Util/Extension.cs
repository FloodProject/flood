using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


static class StringExtensions
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

