using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace RemotingGen.Tests
{
    class Program
    {
        public static void Main()
        {
            try
            {
                var test = new RemotingGenTests();
                test.MainTest();
            }
            catch (System.Exception e)
            {
                System.Diagnostics.Debugger.Break();
            }

            Console.Read();
        }
    }
}
