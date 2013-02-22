using System;
using System.ComponentModel.Composition;
using Flood.GUI.Platform;

namespace Flood.Editor.Controls
{
    class ToolOpenDoc : EditorTool, BarTool
    {
        [Import]
        DocumentManager docManager;

        public void OnSelect()
        {
            Neutral.FileOpen("Open Document", "", "", Open);
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
