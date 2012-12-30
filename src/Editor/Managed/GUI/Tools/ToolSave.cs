using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor.GUI.Tools
{
    [Tool(Text = "Save")]
    class ToolSave : SelectableTool
    {
        [Import]
        DocumentManager docManager;

        public void OnSelect()
        {
            docManager.SaveCurrent();
        }
    }
}
