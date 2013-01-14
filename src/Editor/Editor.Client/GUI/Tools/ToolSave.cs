using System.ComponentModel.Composition;

namespace Flood.Editor.GUI.Tools
{
    class ToolSave : EditorTool, BarTool
    {
        [Import]
        DocumentManager docManager;

        public void OnSelect()
        {
            docManager.SaveCurrent();
        }

        public string Text
        {
            get { return "Save"; }
        }

        public string Image
        {
            get { return "save.png"; }
        }
    }
}
