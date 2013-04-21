using System.Collections.Generic;
using Flood;
using System;
using ICSharpCode.AvalonEdit.Document;
using ICSharpCode.AvalonEdit.Rendering;


namespace TextAddin.Render
{
    class TextLayer : Layer
    {
        private readonly HeightTree heightTree;
        private readonly Dictionary<DocumentLine, TextLine> lines;

        public TextLayer(TextView parent) : base(parent)
        {
            heightTree = new HeightTree(TextView.TextDocument,Skin.DefaultFont.Size);
            lines = new Dictionary<DocumentLine, TextLine>();

            foreach (var docLine in TextView.TextDocument.Lines)
            {
                var textLine = new TextLine(this, docLine);
                lines.Add(docLine,textLine);
            }
        }

        public Vector2 SnapPositionToText(Vector2 pos)
        {
            var documentLine = heightTree.GetLineByVisualPosition(pos.Y);
            var line = lines[documentLine];
            return line.SnapPositionToText(pos);
        }

    }
}
