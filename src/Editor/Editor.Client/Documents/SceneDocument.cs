namespace Flood.Editor.Documents
{
    public class SceneDocument : Document
    {
        public SceneDocument(IUndoManager undoManager, string path)
            : base(undoManager, path)
        {

        }

        public void InitGUI(Gwen.Control.Base rootGUI)
        {
            var label = new Gwen.Control.Label(rootGUI);
            label.SetText("YEAH");
            label.Height = 100;
        }
    }
}
