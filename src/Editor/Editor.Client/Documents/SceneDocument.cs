namespace Flood.Editor.Documents
{
    public class SceneDocument : Document
    {
        public SceneDocument(IUndoManager undoManager, string path)
            : base(undoManager, path)
        {

        }

        public void InitGUI(EngineManaged.GUI.Controls.Base rootGUI)
        {
            var label = new EngineManaged.GUI.Controls.Label(rootGUI);
            label.SetText("YEAH");
            label.Height = 100;
        }
    }
}
