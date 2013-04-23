using System.Collections.Generic;
using Flood;
using System;
using Flood.Editor.Client;
using ICSharpCode.AvalonEdit.Document;
using ICSharpCode.AvalonEdit.Rendering;


namespace TextAddin.Render
{
    class TextLayer : Layer, ILineTracker
    {
        private readonly HeightTree heightTree;
        private readonly Dictionary<DocumentLine, TextLine> lines;
        private WeakLineTracker weakLineTracker;

        public TextLayer(TextView parent) : base(parent)
        {
            heightTree = new HeightTree(TextView.TextDocument,Skin.DefaultFont.Size);
            lines = new Dictionary<DocumentLine, TextLine>();

            weakLineTracker = WeakLineTracker.Register(TextView.TextDocument, this);
            RebuildDocument();
        }

        public Vector2 GetTextLocationPosition(TextLocation textLocation)
        {
            var documentLine = heightTree.GetLineByNumber(textLocation.Line);
            var line = lines[documentLine];
            return line.GetColumnBegining(textLocation.Column);
        }

        public TextLocation GetTextLocation(Vector2i position)
        {
            var documentLine = heightTree.GetLineByVisualPosition(position.Y);
            var line = lines[documentLine];
            return line.GetTextLocation(position.X);
        }

        public bool GetIsCollapsed(int lineNumber)
        {
            return heightTree.GetIsCollapsed(lineNumber);
        }

        protected override void Layout(Flood.GUI.Skins.Skin skin)
        {
            double y = 0;
            foreach (var control in Children)
            {
                if (!control.IsVisible)
                    continue;

                control.SetPosition(0,(int)y);
                y += heightTree.DefaultLineHeight;
            }
        }

        #region ILineTracker

        public void BeforeRemoveLine(DocumentLine documentLine)
        {
            var line = lines[documentLine];
            RemoveChild(line,true);
            lines.Remove(documentLine);
        }

        public void SetLineLength(DocumentLine line, int newTotalLength)
        {
        }

        public void LineInserted(DocumentLine insertionPos, DocumentLine newDocLine)
        {
            var prevLine = lines[insertionPos];
            var line = new TextLine(this, newDocLine);
            line.BringNextToControl(prevLine,true);
            lines.Add(newDocLine,line);
        }

        public void RebuildDocument()
        {
            DeleteAllChildren();
            foreach (var docLine in TextView.TextDocument.Lines)
            {
                var textLine = new TextLine(this, docLine);
                lines.Add(docLine,textLine);
            }
        }

        #endregion
    }
}
