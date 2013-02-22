using Flood.GUI.Controls;

namespace Flood.Editor.Documents
{
    public class SceneDocument : Document
    {
        public SceneDocument(IUndoManager undoManager, string path)
            : base(undoManager, path)
        {

        }

        public void InitGUI(Control rootGUI)
        {
            var label = new Label(rootGUI);
            label.SetText("YEAH");
            label.Height = 100;
        }
    }
}
