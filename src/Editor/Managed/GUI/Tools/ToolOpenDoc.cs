using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor.GUI.Tools
{
    [Tool(Text = "Open Document")]
    class ToolOpenDoc : SelectableTool
    {

        [Import]
        DocumentManager docManager;

        public void OnSelect()
        {
            Gwen.Platform.Neutral.FileOpen("Open Document", "", "", Open);
            
        }

        private void Open(string file)
        {
            Console.WriteLine("Open document " + file);
            docManager.Open(file);
        }
    }
}
