
using Flood.Editor.Client;
using Flood.Editor.Client.Commands;
using Flood.Editor.Client.GUI;
using ICSharpCode.AvalonEdit.Document;
using TextAddin.Render;

namespace TextAddin.Commands
{

    class TextPane : Pane
    {
        public override Flood.GUI.Controls.Control CreateControl(Flood.GUI.Controls.Control parent)
        {
            var doc = new TextDocument();
            doc.Insert(0,"uiofyufghjfjhfj");
            return new TextView(parent,doc);
        }
    }

    public class OpenTextPanel : ICommand
    {
        public void Run()
        {
            var pane = new TextPane();
            pane.Title = "TextPane";
            PaneManager.AddPane(pane);
        }
    }
}
