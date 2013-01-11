using System.ComponentModel.Composition;

namespace FlushEditor.GUI.Tools
{
    class ToolClose : EditorTool, BarTool
    {
        [Import]
        DocumentManager docManager;

        public void OnSelect()
        {
            if (docManager.Current != null)
                docManager.Close(docManager.Current.Id);
        }

        public string Text
        {
            get { return "Close"; }
        }

        public string Image
        {
            get { return "close.png"; }
        }
    }
}
