using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Tests
{
    class Program
    {
        static void Main()
        {
            var guiTests = new GUITests();
            //while (true)
            //{
                guiTests.Setup();
                guiTests.TestSetup();
                guiTests.TestMenu();
                guiTests.TearDown();
            //}
        }
    }
}
