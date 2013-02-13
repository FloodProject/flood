using System;
using System.ComponentModel.Composition;

namespace Flood.Editor.Controls
{
    class ToolOpenDoc : EditorTool, BarTool
    {

        [Import]
        DocumentManager docManager;

        public void OnSelect()
        {
            EngineManaged.GUI.Platform.Neutral.FileOpen("Open Document", "", "", Open);
            
        }

        private void Open(string file)
        {
            Console.WriteLine("Open document " + file);
            docManager.Open(file);
        }

        public string Text
        {
            get { return "Open"; }
        }

        public string Image
        {
            get { return "open.png"; }
        }
    }
}
