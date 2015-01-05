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
            var guiTests = new GUIv2Tests();
            //while (true)
            //{
                guiTests.Setup();
                guiTests.TestSetup();
                guiTests.TestTextClipping4();
                guiTests.TearDown();
            //}
        }
    }
}
