using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor.GUI.Tools
{
    [Tool(Text = "New Document")]
    class ToolNewDoc : SelectableTool
    {
        [Import]
        DocumentManager docManager;

        public void OnSelect()
        {
            Console.WriteLine("Open windows select doc type and path");
            docManager.Open("dummy");
        }
    }
}
