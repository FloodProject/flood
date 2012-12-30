using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor.GUI.Tools
{
    [Tool(Text = "Close")]
    class ToolClose : SelectableTool
    {
        [Import]
        DocumentManager docManager;

        public void OnSelect()
        {
            if (docManager.Current != null)
                docManager.Close(docManager.Current.Id);
        }
    }
}
