
using Flood.Editor.Client;
using Flood.Editor.Client.Commands;
using Flood.Editor.Client.GUI;
using TextAddin.Document;
using TextAddin.Highlighting;
using TextAddin.Controls;

namespace TextAddin.Commands
{

    class TextPane : Pane
    {
        public override Flood.GUI.Controls.Control CreateControl(Flood.GUI.Controls.Control parent)
        {
            var doc = new TextDocument();
            doc.Insert(0,"using System;");
            var textView = new TextView(parent,doc);
            return textView;
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
